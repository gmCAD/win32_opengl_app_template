#include "language_layer.h"
#include "platform.h"
#include "opengl.h"
#include "language_layer.c"

typedef struct Core Core;
struct Core
{
    MemoryArena *permanent_arena;
    MemoryArena *frame_arena;
};
global Core *core = 0;

AppEntryPoint_PermanentLoad
{
    platform = platform_;
    LoadAllOpenGLProcedures();
    core = MemoryArenaAllocateAndZero(&platform->permanent_arena, sizeof(*core));
    core->permanent_arena = &platform->permanent_arena;
    core->frame_arena = &platform->scratch_arena;
}

AppEntryPoint_HotLoad
{
    platform = platform_;
    core = platform->permanent_arena.memory;
}

AppEntryPoint_HotUnload {}

AppEntryPoint_Update
{
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    platform->RefreshScreen();
}
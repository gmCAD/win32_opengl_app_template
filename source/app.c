#include "language_layer.h"
#include "platform.h"
#include "opengl.h"
#include "language_layer.c"

AppEntryPoint
AppEntryPoint_PermanentLoad(PermanentLoad)
{
    platform = platform_;
    LoadAllOpenGLProcedures();
}

AppEntryPoint AppEntryPoint_HotLoad(HotLoad)
{
    platform = platform_;
}

AppEntryPoint AppEntryPoint_HotUnload(HotUnload) {}

AppEntryPoint AppEntryPoint_Update(Update)
{
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    platform->RefreshScreen();
}
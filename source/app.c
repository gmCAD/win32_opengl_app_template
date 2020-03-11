#include "language_layer.h"
#include "opengl.h"

APP_PROC
APPLICATION_PERMANENT_LOAD(PermanentLoad)
{
    platform = platform_;
    LoadAllOpenGLProcedures();
}

APP_PROC
APPLICATION_HOT_LOAD(HotLoad)
{
    platform = platform_;
}

APP_PROC
APPLICATION_HOT_UNLOAD(HotUnload)
{}

APP_PROC
APPLICATION_UPDATE(Update)
{
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    platform->RefreshScreen();
}
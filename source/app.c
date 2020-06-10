#include "language_layer.h"
#include "memory.h"
#include "strings.h"
#include "os.h"
#include "opengl.h"

#include "language_layer.c"
#include "memory.c"
#include "strings.c"
#include "os.c"

APP_PERMANENT_LOAD
{
    os = os_;
    LoadAllOpenGLProcedures();
}

APP_HOT_LOAD
{
    os = os_;
}

APP_HOT_UNLOAD {}

APP_UPDATE
{
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    os->RefreshScreen();
}

#if BUILD_WIN32
#include <windows.h>
#include <gl/gl.h>
#include "ext/wglext.h"
#else
#error "OpenGL includes for platform not supported."
#endif
#include "ext/glext.h"

#define GLProc(name, type) PFNGL##type##PROC gl##name = 0;
#include "opengl_procedure_list.inc"

internal void
LoadAllOpenGLProcedures(void)
{
#define GLProc(name, type) gl##name = os->LoadOpenGLProcedure("gl" #name);
#include "opengl_procedure_list.inc"
}
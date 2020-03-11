
#define GLProc(name, type) PFNGL##type##PROC gl##name = 0;
#include "opengl_procedure_list.inc"

internal void
LoadAllOpenGLProcedures(void)
{
#define GLProc(name, type) gl##name = platform->LoadOpenGLProcedure("gl" #name);
#include "opengl_procedure_list.inc"
}
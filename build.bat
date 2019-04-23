@echo off

set application_name=app.exe
set build_options= -DBUILD_WIN32=1
set compile_flags= -nologo /Zi
set link_flags= gdi32.lib opengl32.lib user32.lib

if not exist build mkdir build
pushd build
start /b /wait "" "cl.exe"  %build_options% %compile_flags% ../source/win32_main.c /link %link_flags% /out:%application_name%
popd
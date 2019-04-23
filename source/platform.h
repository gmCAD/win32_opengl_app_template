#if BUILD_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include "ext/wglext.h"
#else
#error "OpenGL includes for platform not provided."
#endif
#include "ext/glext.h"

enum
{
#define Key(name, str) KEY_##name,
#include "platform_key_list.h"
    KEY_MAX
};

typedef struct Platform
{
    void *permanent_storage;
    u32   permanent_storage_size;
    void *scratch_storage;
    u32   scratch_storage_size;
    
    int window_width;
    int window_height;
    b32 quit;
    b32 fullscreen;
    b32 vsync;
    b32 initialized;
    f32 target_frames_per_second;
    f32 current_time;
    
    f32 mouse_x;
    f32 mouse_y;
    f32 last_mouse_x;
    f32 last_mouse_y;
    f32 mouse_scroll_x;
    f32 mouse_scroll_y;
    b32 left_mouse_down;
    b32 left_mouse_pressed;
    b32 right_mouse_down;
    b32 right_mouse_pressed;
    b8 key_down[KEY_MAX];
    b8 key_pressed[KEY_MAX];
    i32 last_key;
    char *target_text;
    u32 target_text_max_characters;
    u32 target_text_edit_pos;
    b32 keyboard_used;
    
    void (*OutputError)(const char *title, const char *format, ...);
    void *(*LoadOpenGLProcedure)(const char *name);
    void (*SwapBuffers)(void);
    b32 (*LoadEntireFile)(const char *filename, void **data, u32 *data_size);
    void (*FreeFileData)(void *data);
}
Platform;

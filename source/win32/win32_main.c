#include <windows.h>
#include <windowsx.h>
#include <xinput.h>
#include <objbase.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <audiopolicy.h>
#undef DeleteFile

// NOTE(rjf): OpenGL
#include <gl/gl.h>
#include "ext/wglext.h"
#include "ext/glext.h"

// NOTE(rjf): CRT
#include <stdio.h>

// NOTE(rjf): Headers
#include "program_options.h"
#include "language_layer.h"
#include "maths.h"
#include "memory.h"
#include "strings.h"
#include "os.h"
#include "win32_timer.h"
#include "language_layer.c"
#include "memory.c"
#include "strings.c"
#include "os.c"

// NOTE(rjf): Globals
global char global_executable_path[256];
global char global_executable_directory[256];
global char global_working_directory[256];
global char global_app_dll_path[256];
global char global_temp_app_dll_path[256];
global OS_State global_os;
global HDC global_device_context;
global HINSTANCE global_instance_handle;
global W32_Timer global_win32_timer = {0};
#define W32_MAX_GAMEPADS 16
typedef struct W32_GamepadInput W32_GamepadInput;
struct W32_GamepadInput
{
    b32 connected;
    v2 joystick_1;
    v2 joystick_2;
    f32 trigger_left;
    f32 trigger_right;
    i32 button_states[GamepadButton_Max];
};
W32_GamepadInput global_gamepads[W32_MAX_GAMEPADS];


// NOTE(rjf): Implementations
#include "win32_utilities.c"
#include "win32_timer.c"
#include "win32_file_io.c"
#include "win32_app_code.c"
#include "win32_xinput.c"
#include "win32_wasapi.c"
#include "win32_opengl.c"

//~

typedef enum W32_CursorStyle
{
    W32_CursorStyle_Normal,
    W32_CursorStyle_HorizontalResize,
    W32_CursorStyle_VerticalResize,
    W32_CursorStyle_IBar,
}
W32_CursorStyle;

global W32_CursorStyle global_cursor_style = 0;

internal v2
W32_GetMousePosition(HWND window)
{
    v2 result = {0};
    POINT mouse;
    GetCursorPos(&mouse);
    ScreenToClient(window, &mouse);
    result.x = (f32)(mouse.x);
    result.y = (f32)(mouse.y);
    return result;
}

internal LRESULT
W32_WindowProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param)
{
    LRESULT result = 0;
    
    local_persist b32 mouse_hover_active_because_windows_makes_me_cry = 0;
    
    KeyModifiers modifiers = 0;
    if(GetKeyState(VK_CONTROL) & 0x8000)
    {
        modifiers |= KeyModifier_Ctrl;
    }
    if(GetKeyState(VK_SHIFT) & 0x8000)
    {
        modifiers |= KeyModifier_Shift;
    }
    if(GetKeyState(VK_MENU) & 0x8000)
    {
        modifiers |= KeyModifier_Alt;
    }
    
    if(message == WM_CLOSE || message == WM_DESTROY || message == WM_QUIT)
    {
        global_os.quit = 1;
        result = 0;
    }
    else if(message == WM_LBUTTONDOWN)
    {
        OS_PushEvent(OS_MousePressEvent(MouseButton_Left, global_os.mouse_position));
    }
    else if(message == WM_LBUTTONUP)
    {
        OS_PushEvent(OS_MouseReleaseEvent(MouseButton_Left, global_os.mouse_position));
    }
    else if(message == WM_RBUTTONDOWN)
    {
        OS_PushEvent(OS_MousePressEvent(MouseButton_Right, global_os.mouse_position));
    }
    else if(message == WM_RBUTTONUP)
    {
        OS_PushEvent(OS_MouseReleaseEvent(MouseButton_Right, global_os.mouse_position));
    }
    else if(message == WM_MOUSEMOVE)
    {
        i16 x_position = LOWORD(l_param);
        i16 y_position = HIWORD(l_param);
        v2 last_mouse = global_os.mouse_position;
        global_os.mouse_position = W32_GetMousePosition(window_handle);
        OS_PushEvent(OS_MouseMoveEvent(global_os.mouse_position,
                                       v2(global_os.mouse_position.x - last_mouse.x,
                                          global_os.mouse_position.y - last_mouse.y)));
        
        if(mouse_hover_active_because_windows_makes_me_cry == 0)
        {
            mouse_hover_active_because_windows_makes_me_cry = 1;
            TRACKMOUSEEVENT track_mouse_event = {0};
            {
                track_mouse_event.cbSize = sizeof(track_mouse_event);
                track_mouse_event.dwFlags = TME_LEAVE;
                track_mouse_event.hwndTrack = window_handle;
                track_mouse_event.dwHoverTime = HOVER_DEFAULT;
            }
            TrackMouseEvent(&track_mouse_event);
        }
    }
    else if(message == WM_MOUSELEAVE)
    {
        mouse_hover_active_because_windows_makes_me_cry = 0;
    }
    else if(message == WM_MOUSEWHEEL)
    {
        i16 wheel_delta = HIWORD(w_param);
        OS_PushEvent(OS_MouseScrollEvent(v2(0, (f32)wheel_delta), modifiers));
    }
    else if(message == WM_MOUSEHWHEEL)
    {
        i16 wheel_delta = HIWORD(w_param);
        OS_PushEvent(OS_MouseScrollEvent(v2((f32)wheel_delta, 0), modifiers));
    }
    else if(message == WM_SETCURSOR)
    {
        
        if(global_os.mouse_position.x >= 1 && global_os.mouse_position.x <= global_os.window_size.x-1 &&
           global_os.mouse_position.y >= 1 && global_os.mouse_position.y <= global_os.window_size.y-1 && mouse_hover_active_because_windows_makes_me_cry)
        {
            switch(global_cursor_style)
            {
                case W32_CursorStyle_HorizontalResize:
                {
                    SetCursor(LoadCursorA(0, IDC_SIZEWE));
                    break;
                }
                case W32_CursorStyle_VerticalResize:
                {
                    SetCursor(LoadCursorA(0, IDC_SIZENS));
                    break;
                }
                case W32_CursorStyle_IBar:
                {
                    SetCursor(LoadCursorA(0, IDC_IBEAM));
                    break;
                }
                case W32_CursorStyle_Normal:
                {
                    SetCursor(LoadCursorA(0, IDC_ARROW));
                    break;
                }
                default: break;
            }
        }
        else
        {
            result = DefWindowProc(window_handle, message, w_param, l_param);
        }
    }
    else if(message == WM_SYSKEYDOWN || message == WM_SYSKEYUP ||
            message == WM_KEYDOWN || message == WM_KEYUP)
    {
        u64 vkey_code = w_param;
        i8 was_down = !!(l_param & (1 << 30));
        i8 is_down =   !(l_param & (1 << 31));
        
        u64 key_input = 0;
        
        if((vkey_code >= 'A' && vkey_code <= 'Z') ||
           (vkey_code >= '0' && vkey_code <= '9'))
        {
            // NOTE(rjf): Letter/number buttons
            key_input = (vkey_code >= 'A' && vkey_code <= 'Z') ? Key_A + (vkey_code-'A') : Key_0 + (vkey_code-'0');
        }
        else
        {
            if(vkey_code == VK_ESCAPE)
            {
                key_input = Key_Esc;
            }
            else if(vkey_code >= VK_F1 && vkey_code <= VK_F12)
            {
                key_input = Key_F1 + vkey_code - VK_F1;
            }
            else if(vkey_code == VK_OEM_3)
            {
                key_input = Key_GraveAccent;
            }
            else if(vkey_code == VK_OEM_MINUS)
            {
                key_input = Key_Minus;
            }
            else if(vkey_code == VK_OEM_PLUS)
            {
                key_input = Key_Equal;
            }
            else if(vkey_code == VK_BACK)
            {
                key_input = Key_Backspace;
            }
            else if(vkey_code == VK_TAB)
            {
                key_input = Key_Tab;
            }
            else if(vkey_code == VK_SPACE)
            {
                key_input = Key_Space;
            }
            else if(vkey_code == VK_RETURN)
            {
                key_input = Key_Enter;
            }
            else if(vkey_code == VK_CONTROL)
            {
                key_input = Key_Ctrl;
                modifiers &= ~KeyModifier_Ctrl;
            }
            else if(vkey_code == VK_SHIFT)
            {
                key_input = Key_Shift;
                modifiers &= ~KeyModifier_Shift;
            }
            else if(vkey_code == VK_MENU)
            {
                key_input = Key_Alt;
                modifiers &= ~KeyModifier_Alt;
            }
            else if(vkey_code == VK_UP)
            {
                key_input = Key_Up;
            }
            else if(vkey_code == VK_LEFT)
            {
                key_input = Key_Left;
            }
            else if(vkey_code == VK_DOWN)
            {
                key_input = Key_Down;
            }
            else if(vkey_code == VK_RIGHT)
            {
                key_input = Key_Right;
            }
            else if(vkey_code == VK_DELETE)
            {
                key_input = Key_Delete;
            }
            else if(vkey_code == VK_PRIOR)
            {
                key_input = Key_PageUp;
            }
            else if(vkey_code == VK_NEXT)
            {
                key_input = Key_PageDown;
            }
            else if(vkey_code == VK_HOME)
            {
                key_input = Key_Home;
            }
            else if(vkey_code == VK_END)
            {
                key_input = Key_End;
            }
            else if(vkey_code == VK_OEM_2)
            {
                key_input = Key_ForwardSlash;
            }
            else if(vkey_code == VK_OEM_PERIOD)
            {
                key_input = Key_Period;
            }
            else if(vkey_code == VK_OEM_COMMA)
            {
                key_input = Key_Comma;
            }
            else if(vkey_code == VK_OEM_7)
            {
                key_input = Key_Quote;
            }
            else if(vkey_code == VK_OEM_4)
            {
                key_input = Key_LeftBracket;
            }
            else if(vkey_code == VK_OEM_6)
            {
                key_input = Key_RightBracket;
            }
        }
        
        if(is_down)
        {
            OS_PushEvent(OS_KeyPressEvent(key_input, modifiers));
        }
        else
        {
            OS_PushEvent(OS_KeyReleaseEvent(key_input, modifiers));
        }
        
        result = DefWindowProc(window_handle, message, w_param, l_param);
    }
    else if(message == WM_CHAR)
    {
        u64 char_input = w_param;
        if(char_input >= 32 && char_input != VK_RETURN && char_input != VK_ESCAPE &&
           char_input != 127)
        {
            OS_PushEvent(OS_CharacterInputEvent(char_input));
        }
    }
    else
    {
        result = DefWindowProc(window_handle, message, w_param, l_param);
    }
    
    return result;
}

internal f32
W32_GetTime(void)
{
    W32_Timer *timer = &global_win32_timer;
    LARGE_INTEGER current_time;
    QueryPerformanceCounter(&current_time);
    return global_os.current_time + (f32)(current_time.QuadPart - timer->begin_frame.QuadPart) / (f32)timer->counts_per_second.QuadPart;
}

internal u64
W32_GetCycles(void)
{
    u64 result = __rdtsc();
    return result;
}

internal void
W32_ResetCursor(void)
{
    global_cursor_style = W32_CursorStyle_Normal;
}

internal void
W32_SetCursorToHorizontalResize(void)
{
    global_cursor_style = W32_CursorStyle_HorizontalResize;
}

internal void
W32_SetCursorToVerticalResize(void)
{
    global_cursor_style = W32_CursorStyle_VerticalResize;
}

int
WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR lp_cmd_line, int n_show_cmd)
{
    global_instance_handle = instance;
    
    W32_TimerInit(&global_win32_timer);
    W32_AppCode win32_game_code = {0};
    W32_SoundOutput win32_sound_output = {0};
    
    // NOTE(rjf): Calculate executable name and path to DLL
    {
        DWORD size_of_executable_path =
            GetModuleFileNameA(0, global_executable_path, sizeof(global_executable_path));
        
        // NOTE(rjf): Calculate executable directory
        {
            MemoryCopy(global_executable_directory, global_executable_path, size_of_executable_path);
            char *one_past_last_slash = global_executable_directory;
            for(i32 i = 0; global_executable_directory[i]; ++i)
            {
                if(global_executable_directory[i] == '\\')
                {
                    one_past_last_slash = global_executable_directory + i + 1;
                }
            }
            *one_past_last_slash = 0;
        }
        
        // NOTE(rjf): Create DLL filenames
        {
            wsprintf(global_app_dll_path, "%s%s.dll", global_executable_directory, PROGRAM_FILENAME);
            wsprintf(global_temp_app_dll_path, "%stemp_%s.dll", global_executable_directory, PROGRAM_FILENAME);
        }
        
        GetCurrentDirectory(sizeof(global_working_directory), global_working_directory);
    }
    
    WNDCLASS window_class = {0};
    {
        window_class.style = CS_HREDRAW | CS_VREDRAW;
        window_class.lpfnWndProc = W32_WindowProc;
        window_class.hInstance = instance;
        window_class.lpszClassName = "ApplicationWindowClass";
        window_class.hCursor = LoadCursor(0, IDC_ARROW);
    }
    
    if(!RegisterClass(&window_class))
    {
        // NOTE(rjf): ERROR: Window class registration failure
        W32_OutputError("Fatal Error", "Window class registration failure.");
        goto quit;
    }
    
    HWND window_handle = CreateWindow("ApplicationWindowClass", WINDOW_TITLE,
                                      WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                                      DEFAULT_WINDOW_WIDTH,
                                      DEFAULT_WINDOW_HEIGHT,
                                      0, 0, instance, 0);
    
    if(!window_handle)
    {
        // NOTE(rjf): ERROR: Window creation failure
        W32_OutputError("Fatal Error", "Window creation failure.");
        goto quit;
    }
    
    // NOTE(rjf): Load application code
    W32_AppCode win32_app_code = {0};
    {
        if(!W32_AppCodeLoad(&win32_app_code))
        {
            // NOTE(rjf): ERROR: Application code load failure
            W32_OutputError("Fatal Error", "Application code load failure.");
            goto quit;
        }
    }
    
    // NOTE(rjf): Sound initialization
    {
        win32_sound_output.channels = 2;
        win32_sound_output.samples_per_second = 48000;
        win32_sound_output.latency_frame_count = 48000;
        W32_LoadWASAPI();
        W32_InitWASAPI(&win32_sound_output);
    }
    
    // NOTE(rjf): Find refresh rate
    f32 refresh_rate = 60.f;
    {
        DEVMODEA device_mode = {0};
        if(EnumDisplaySettingsA(0, ENUM_CURRENT_SETTINGS, &device_mode))
        {
            refresh_rate = (float)device_mode.dmDisplayFrequency;
        }
    }
    
    // NOTE(rjf): Initialize platform
    {
        os = &global_os;
        
        global_os.executable_folder_absolute_path = String8FromCString(global_executable_directory);
        global_os.executable_absolute_path = String8FromCString(global_executable_path);
        global_os.working_directory_path = String8FromCString(global_working_directory);
        
        global_os.quit                      = 0;
        global_os.vsync                     = 1;
        global_os.fullscreen                = 0;
        global_os.window_size.x             = DEFAULT_WINDOW_WIDTH;
        global_os.window_size.y             = DEFAULT_WINDOW_HEIGHT;
        global_os.current_time              = 0.f;
        global_os.target_frames_per_second  = refresh_rate;
        
        global_os.sample_out = W32_HeapAlloc(win32_sound_output.samples_per_second * sizeof(f32) * 2);
        global_os.samples_per_second = win32_sound_output.samples_per_second;
        
        global_os.Reserve                        = W32_Reserve;
        global_os.Release                        = W32_Release;
        global_os.Commit                         = W32_Commit;
        global_os.Decommit                       = W32_Decommit;
        global_os.OutputError                    = W32_OutputError;
        global_os.SaveToFile                     = W32_SaveToFile;
        global_os.AppendToFile                   = W32_AppendToFile;
        global_os.LoadEntireFile                 = W32_LoadEntireFile;
        global_os.LoadEntireFileAndNullTerminate = W32_LoadEntireFileAndNullTerminate;
        global_os.DeleteFile                     = W32_DeleteFile;
        global_os.MakeDirectory                  = W32_MakeDirectory;
        global_os.DoesFileExist                  = W32_DoesFileExist;
        global_os.DoesDirectoryExist             = W32_DoesDirectoryExist;
        global_os.CopyFile                       = W32_CopyFile;
        global_os.ListDirectory                  = W32_DirectoryListLoad;
        global_os.GetTime                        = W32_GetTime;
        global_os.GetCycles                      = W32_GetCycles;
        global_os.ResetCursor                    = W32_ResetCursor;
        global_os.SetCursorToHorizontalResize    = W32_SetCursorToHorizontalResize;
        global_os.SetCursorToVerticalResize      = W32_SetCursorToVerticalResize;
        global_os.LoadOpenGLProcedure            = W32_LoadOpenGLProcedure;
        global_os.RefreshScreen                  = W32_OpenGLRefreshScreen;
        
        global_os.permanent_arena = M_ArenaInitialize();
        global_os.frame_arena = M_ArenaInitialize();
    }
    
    // NOTE(rjf): OpenGL initialization
    {
        global_device_context = GetDC(window_handle);
        if(!W32_InitOpenGL(&global_device_context, global_instance_handle))
        {
            W32_OutputError("Fatal Error", "OpenGL initialization failure.");
            goto quit;
        }
    }
    
    W32_LoadXInput();
    
    win32_app_code.PermanentLoad(&global_os);
    win32_app_code.HotLoad(&global_os);
    
    ShowWindow(window_handle, n_show_cmd);
    UpdateWindow(window_handle);
    
    while(!global_os.quit)
    {
        W32_TimerBeginFrame(&global_win32_timer);
        M_ArenaClear(&os->frame_arena);
        
        // NOTE(rjf): Update Windows events
        {
            MSG message;
            if(global_os.wait_for_events_to_update && !global_os.pump_events)
            {
                WaitMessage();
            }
            
            while(PeekMessage(&message, 0, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
        }
        
        // NOTE(rjf): Update window size
        {
            RECT client_rect;
            GetClientRect(window_handle, &client_rect);
            global_os.window_size.x = client_rect.right - client_rect.left;
            global_os.window_size.y = client_rect.bottom - client_rect.top;
        }
        
        // NOTE(rjf): Update input data (post-event)
        OS_BeginFrame();
        {
            POINT mouse;
            GetCursorPos(&mouse);
            ScreenToClient(window_handle, &mouse);
            W32_UpdateXInput();
            global_os.pump_events = 0;
        }
        
        // NOTE(rjf): Find how much sound to write and where
        if(win32_sound_output.initialized)
        {
            global_os.sample_count_to_output = 0;
            UINT32 sound_padding_size;
            if(SUCCEEDED(win32_sound_output.audio_client->lpVtbl->GetCurrentPadding(win32_sound_output.audio_client, &sound_padding_size)))
            {
                global_os.samples_per_second = win32_sound_output.samples_per_second;
                global_os.sample_count_to_output = (u32)(win32_sound_output.latency_frame_count - sound_padding_size);
                if(global_os.sample_count_to_output > win32_sound_output.latency_frame_count)
                {
                    global_os.sample_count_to_output = win32_sound_output.latency_frame_count;
                }
            }
            
            for(u32 i = 0; i < win32_sound_output.buffer_frame_count; ++i)
            {
                global_os.sample_out[i] = 0;
            }
        }
        
        // NOTE(rjf): Call into the app layer to update
        {
            b32 last_fullscreen = global_os.fullscreen;
            
            win32_app_code.Update();
            
            // NOTE(rjf): Update fullscreen if necessary
            if(last_fullscreen != global_os.fullscreen)
            {
                W32_ToggleFullscreen(window_handle);
            }
            
            // NOTE(rjf): Fill sound buffer with game sound
            if(win32_sound_output.initialized)
            {
                W32_FillSoundBuffer(global_os.sample_count_to_output, global_os.sample_out, &win32_sound_output);
            }
        }
        
        // NOTE(rjf): Post-update platform data update
        {
            OS_EndFrame();
        }
        
        W32_AppCodeUpdate(&win32_app_code);
        
        W32_TimerEndFrame(&global_win32_timer, 1000.0 * (1.0 / (f64)global_os.target_frames_per_second));
    }
    
    ShowWindow(window_handle, SW_HIDE);
    
    W32_AppCodeUnload(&win32_app_code);
    W32_CleanUpOpenGL(&global_device_context);
    
    quit:;
    
    return 0;
}

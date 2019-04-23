#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "program_options.h"
#include "language_layer.h"
#include "platform.h"
#include "win32_opengl.c"
#include "app.c"

static Platform global_platform;
static char global_executable_path[256];
static char global_executable_directory[256];
static HDC global_device_context;

internal void
Win32OutputError(const char *title, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    u32 required_characters = vsnprintf(0, 0, format, args)+1;
    va_end(args);
    
    local_persist char text[4096] = {0};
    
    if(required_characters > 4096) {
        required_characters = 4096;
    }
    
    va_start(args, format);
    vsnprintf(text, required_characters, format, args);
    va_end(args);
    
    text[required_characters-1] = 0;
    
    MessageBoxA(0, text, title, MB_OK);
}

internal void
Win32ToggleFullscreen(HWND hwnd) {
    local_persist WINDOWPLACEMENT last_window_placement = {
        sizeof(last_window_placement)
    };
    
    DWORD window_style = GetWindowLong(hwnd, GWL_STYLE);
    if(window_style & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO monitor_info = { sizeof(monitor_info) };
        if(GetWindowPlacement(hwnd, &last_window_placement) &&
           GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY),
                          &monitor_info))
        {
            
            SetWindowLong(hwnd, GWL_STYLE,
                          window_style & ~WS_OVERLAPPEDWINDOW);
            
            SetWindowPos(hwnd, HWND_TOP,
                         monitor_info.rcMonitor.left,
                         monitor_info.rcMonitor.top,
                         monitor_info.rcMonitor.right -
                         monitor_info.rcMonitor.left,
                         monitor_info.rcMonitor.bottom -
                         monitor_info.rcMonitor.top,
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    }
    else
    {
        SetWindowLong(hwnd, GWL_STYLE,
                      window_style | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(hwnd, &last_window_placement);
        SetWindowPos(hwnd, 0, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }
}

internal LRESULT
Win32WindowProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param)
{
    LRESULT result;
    if(message == WM_CLOSE || message == WM_DESTROY || message == WM_QUIT)
    {
        global_platform.quit = 1;
        result = 0;
    }
    else if(message == WM_LBUTTONDOWN)
    {
        if(!global_platform.left_mouse_down)
        {
            global_platform.left_mouse_pressed = 1;
        }
        global_platform.left_mouse_down = 1;
    }
    else if(message == WM_LBUTTONUP)
    {
        global_platform.left_mouse_down = 0;
        global_platform.left_mouse_pressed = 0;
    }
    else if(message == WM_RBUTTONDOWN)
    {
        if(!global_platform.right_mouse_down)
        {
            global_platform.right_mouse_pressed = 1;
        }
        global_platform.right_mouse_down = 1;
    }
    else if(message == WM_RBUTTONUP)
    {
        global_platform.right_mouse_down = 0;
        global_platform.right_mouse_pressed = 0;
    }
    else if(message == WM_MOUSEWHEEL)
    {
        i16 wheel_delta = HIWORD(w_param);
        global_platform.mouse_scroll_y = (f32)wheel_delta;
    }
    else if(message == WM_MOUSEHWHEEL)
    {
        i16 wheel_delta = HIWORD(w_param);
        global_platform.mouse_scroll_x = (f32)wheel_delta;
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
            key_input = (vkey_code >= 'A' && vkey_code <= 'Z') ? KEY_a + (vkey_code-'A') : KEY_0 + (vkey_code-'0');
        }
        else
        {
            if(vkey_code == VK_ESCAPE)
            {
                key_input = KEY_esc;
            }
            else if(vkey_code >= VK_F1 && vkey_code <= VK_F12)
            {
                key_input = KEY_f1 + vkey_code - VK_F1;
            }
            else if(vkey_code == VK_OEM_3)
            {
                key_input = KEY_grave_accent;
            }
            else if(vkey_code == VK_OEM_MINUS)
            {
                key_input = KEY_minus;
            }
            else if(vkey_code == VK_OEM_PLUS)
            {
                key_input = KEY_equal;
            }
            else if(vkey_code == VK_BACK)
            {
                key_input = KEY_backspace;
            }
            else if(vkey_code == VK_TAB)
            {
                key_input = KEY_tab;
            }
            else if(vkey_code == VK_SPACE)
            {
                key_input = KEY_space;
            }
            else if(vkey_code == VK_RETURN)
            {
                key_input = KEY_enter;
            }
            else if(vkey_code == VK_CONTROL)
            {
                key_input = KEY_ctrl;
            }
            else if(vkey_code == VK_SHIFT)
            {
                key_input = KEY_shift;
            }
            else if(vkey_code == VK_MENU)
            {
                key_input = KEY_alt;
            }
            else if(vkey_code == VK_UP)
            {
                key_input = KEY_up;
            }
            else if(vkey_code == VK_LEFT)
            {
                key_input = KEY_left;
            }
            else if(vkey_code == VK_DOWN)
            {
                key_input = KEY_down;
            }
            else if(vkey_code == VK_RIGHT)
            {
                key_input = KEY_right;
            }
            else if(vkey_code == VK_DELETE)
            {
                key_input = KEY_delete;
            }
            else if(vkey_code == VK_PRIOR)
            {
                key_input = KEY_page_up;
            }
            else if(vkey_code == VK_NEXT)
            {
                key_input = KEY_page_down;
            }
        }
        
        if(is_down) {
            if(!global_platform.key_down[key_input])
            {
                ++global_platform.key_pressed[key_input];
            }
            ++global_platform.key_down[key_input];
            global_platform.last_key = (i32)key_input;
            
            if(key_input == KEY_backspace && global_platform.target_text)
            {
                if(global_platform.key_down[KEY_ctrl])
                {
                    for(u32 i = global_platform.target_text_edit_pos-2;
                        i >= 0 && i < global_platform.target_text_max_characters;
                        --i)
                    {
                        if(!i || global_platform.target_text[i-1] == ' ')
                        {
                            global_platform.target_text[i] = 0;
                            global_platform.target_text_edit_pos = i;
                            break;
                        }
                    }
                }
                else
                {
                    if(global_platform.target_text_edit_pos)
                    {
                        // NOTE(rjf): This assumes editing only takes place at
                        //            the end of the string!!!
                        global_platform.target_text[--global_platform.target_text_edit_pos] = 0;
                    }
                }
            }
            else if(key_input == KEY_f4 && global_platform.key_down[KEY_alt])
            {
                global_platform.quit = 1;
            }
        }
        else
        {
            global_platform.key_down[key_input] = 0;
            global_platform.key_pressed[key_input] = 0;
        }
    }
    else
    {
        result = DefWindowProc(window_handle, message, w_param, l_param);
    }
    return result;
}

internal void
Win32SwapBuffers(void)
{
    Win32OpenGLSwapBuffers(global_device_context);
}

internal void *
Win32HeapAlloc(u32 size)
{
    return HeapAlloc(GetProcessHeap(), 0, size);
}

internal void
Win32HeapFree(void *data)
{
    HeapFree(GetProcessHeap(), 0, data);
}

internal b32
Win32LoadEntireFile(const char *filename, void **data, u32 *data_size)
{
    b32 read_successful = 0;
    
    *data = 0;
    *data_size = 0;
    
    HANDLE file = {0};
    
    {
        DWORD desired_access = GENERIC_READ | GENERIC_WRITE;
        DWORD share_mode = 0;
        SECURITY_ATTRIBUTES security_attributes = {
            (DWORD)sizeof(SECURITY_ATTRIBUTES),
            0,
            0,
        };
        DWORD creation_disposition = OPEN_EXISTING;
        DWORD flags_and_attributes = 0;
        HANDLE template_file = 0;
        
        if((file = CreateFile(filename, desired_access, share_mode, &security_attributes, creation_disposition, flags_and_attributes, template_file)) != INVALID_HANDLE_VALUE)
        {
            DWORD read_bytes = GetFileSize(file, 0);
            if(read_bytes)
            {
                void *read_data = Win32HeapAlloc(read_bytes+1);
                DWORD bytes_read = 0;
                OVERLAPPED overlapped = {0};
                
                ReadFile(file, read_data, read_bytes, &bytes_read, &overlapped);
                
                ((u8 *)read_data)[read_bytes] = 0;
                
                *data = read_data;
                *data_size = (u64)bytes_read;
                
                read_successful = 1;
            }
            CloseHandle(file);
        }
    }
    
    return read_successful;
}

internal void
Win32FreeFileData(void *data)
{
    Win32HeapFree(data);
}

int
WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR lp_cmd_line, int n_show_cmd)
{
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
    }
    
    WNDCLASS window_class = {0};
    {
        window_class.style = CS_HREDRAW | CS_VREDRAW;
        window_class.lpfnWndProc = Win32WindowProc;
        window_class.hInstance = instance;
        window_class.lpszClassName = WINDOW_TITLE "WindowClass";
        window_class.hCursor = LoadCursor(0, IDC_ARROW);
    }
    
    if(!RegisterClass(&window_class))
    {
        // NOTE(rjf): ERROR: Window class registration failure
        Win32OutputError("Fatal Error", "Window class registration failure.");
        goto quit;
    }
    
    HWND window_handle = CreateWindow(window_class.lpszClassName, WINDOW_TITLE,
                                      WS_OVERLAPPEDWINDOW,
                                      CW_USEDEFAULT, CW_USEDEFAULT,
                                      DEFAULT_WINDOW_WIDTH,
                                      DEFAULT_WINDOW_HEIGHT,
                                      0, 0, instance, 0);
    
    if(!window_handle)
    {
        // NOTE(rjf): ERROR: Window creation failure
        Win32OutputError("Fatal Error", "Window creation failure.");
        goto quit;
    }
    
    // NOTE(rjf): Initialize platform
    {
        global_platform.quit = 0;
        global_platform.initialized = 0;
        global_platform.window_width = DEFAULT_WINDOW_WIDTH;
        global_platform.window_height = DEFAULT_WINDOW_HEIGHT;
        global_platform.target_frames_per_second = 60.f;
        global_platform.current_time = 0.f;
        
        global_platform.permanent_storage_size = PERMANENT_STORAGE_SIZE;
        global_platform.scratch_storage_size = SCRATCH_STORAGE_SIZE;
        
        global_platform.permanent_storage = VirtualAlloc(0, global_platform.permanent_storage_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        global_platform.scratch_storage = VirtualAlloc(0, global_platform.scratch_storage_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        
        global_platform.OutputError = Win32OutputError;
        global_platform.LoadOpenGLProcedure = Win32LoadOpenGLProcedure;
        global_platform.SwapBuffers = Win32SwapBuffers;
        global_platform.LoadEntireFile = Win32LoadEntireFile;
        global_platform.FreeFileData = Win32FreeFileData;
    }
    
    global_device_context = GetDC(window_handle);
    
    if(!Win32InitOpenGL(&global_device_context, instance))
    {
        // NOTE(rjf): ERROR: OpenGL initialization error
        Win32OutputError("Fatal Error", "OpenGL initialization failure.");
        goto quit;
    }
    
    LARGE_INTEGER performance_counter_frequency;
    LARGE_INTEGER begin_frame_time_data;
    LARGE_INTEGER end_frame_time_data;
    
    QueryPerformanceFrequency(&performance_counter_frequency);
    
    ShowWindow(window_handle, n_show_cmd);
    UpdateWindow(window_handle);
    
    while(!global_platform.quit)
    {
        QueryPerformanceCounter(&begin_frame_time_data);
        
        MSG message;
        while(PeekMessage(&message, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        
        // NOTE(rjf): Update input data (post-event)
        {
            POINT mouse;
            GetCursorPos(&mouse);
            ScreenToClient(window_handle, &mouse);
            global_platform.mouse_x = (f32)(mouse.x);
            global_platform.mouse_y = (f32)(mouse.y);
        }
        
        // NOTE(rjf): Update window size
        {
            RECT client_rect;
            GetClientRect(window_handle, &client_rect);
            global_platform.window_width = client_rect.right - client_rect.left;
            global_platform.window_height = client_rect.bottom - client_rect.top;
        }
        
        b32 last_vsync = global_platform.vsync;
        b32 last_fullscreen = global_platform.fullscreen;
        
        Update(&global_platform);
        
        // NOTE(rjf): Update vsync if necessary
        if(last_vsync != global_platform.vsync)
        {
            Win32OpenGLSetVerticalSync(global_platform.vsync);
        }
        
        // NOTE(rjf): Update fullscreen if necessary
        if(last_fullscreen != global_platform.fullscreen)
        {
            Win32ToggleFullscreen(window_handle);
        }
        
        QueryPerformanceCounter(&end_frame_time_data);
        
        // NOTE(rjf): Wait if necessary.
        {
            i64 desired_frame_counts = (i64)(performance_counter_frequency.QuadPart / global_platform.target_frames_per_second);
            i64 frame_count = end_frame_time_data.QuadPart - begin_frame_time_data.QuadPart;
            i64 counts_to_wait = desired_frame_counts - frame_count;
            
            LARGE_INTEGER begin_wait_time_data;
            LARGE_INTEGER end_wait_time_data;
            
            QueryPerformanceCounter(&begin_wait_time_data);
            
            while(counts_to_wait > 0)
            {
                QueryPerformanceCounter(&end_wait_time_data);
                counts_to_wait -= end_wait_time_data.QuadPart - begin_wait_time_data.QuadPart;
                begin_wait_time_data = end_wait_time_data;
            }
        }
        
        global_platform.current_time += 1.f / global_platform.target_frames_per_second;
    }
    
    Win32CleanUpOpenGL(&global_device_context);
    
    quit:;
    
    return 0;
}

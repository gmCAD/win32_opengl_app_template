
//~ NOTE(rjf): Keys

typedef enum Key
{
#define Key(name, str) Key_##name,
#include "platform_key_list.inc"
#undef Key
    Key_Max
}
Key;

typedef u32 KeyModifiers;
#define KeyModifier_Ctrl  (1<<0)
#define KeyModifier_Shift (1<<1)
#define KeyModifier_Alt   (1<<2)

internal String8
KeyName(Key index)
{
    local_persist char *strings[Key_Max] =
    {
#define Key(name, str) str,
#include "platform_key_list.inc"
#undef Key
    };
    
    char *string = "(Invalid Key)";
    if(index >= 0 && index < ArrayCount(strings))
    {
        string = strings[index];
    }
    String8 result;
    result.str = string;
    result.size = CalculateCStringLength(result.str);
    return result;
}

//~ NOTE(rjf): Mouse

typedef enum MouseButton
{
    MouseButton_Left,
    MouseButton_Right,
    MouseButton_Middle,
}
MouseButton;

//~ NOTE(rjf): Gamepads

typedef enum GamepadButton
{
#define GamepadButton(name, str) GamepadButton_##name,
#include "platform_gamepad_button_list.inc"
#undef GamepadButton
    GamepadButton_Max
}
GamepadButton;

internal String8
GamepadButtonName(GamepadButton index)
{
    local_persist char *strings[GamepadButton_Max] =
    {
#define GamepadButton(name, str) str,
#include "platform_gamepad_button_list.inc"
#undef GamepadButton
    };
    
    char *string = "(Invalid Gamepad Button)";
    if(index >= 0 && index < ArrayCount(strings))
    {
        string = strings[index];
    }
    String8 result;
    result.str = string;
    result.size = CalculateCStringLength(result.str);
    return result;
}

//~ NOTE(rjf): Platform Directory Listing

#define PlatformDirectoryList_IncludeDirectories (1<<0)
#define PlatformDirectoryList_IncludeExtensions  (1<<1)
#define PlatformDirectoryList_SearchRecursively  (1<<2)
#define PlatformDirectoryItem_IsDirectory        (1<<0)

typedef struct PlatformDirectoryItem PlatformDirectoryItem;
struct PlatformDirectoryItem
{
    String8 string;
    u64 flags;
};

typedef struct PlatformDirectoryItemChunk PlatformDirectoryItemChunk;
struct PlatformDirectoryItemChunk
{
    PlatformDirectoryItem items[32];
    PlatformDirectoryItemChunk *next;
    u32 item_count;
};

typedef struct PlatformDirectoryList PlatformDirectoryList;
struct PlatformDirectoryList
{
    u32 flags;
    u32 item_count;
    PlatformDirectoryItemChunk *first_chunk;
};

//~ NOTE(rjf): Events

typedef enum PlatformEventType
{
    PlatformEventType_Null,
    
    // NOTE(rjf): Keyboard
    PlatformEventType_KeyStart,
    PlatformEventType_CharacterInput,
    PlatformEventType_KeyPress,
    PlatformEventType_KeyRelease,
    PlatformEventType_KeyEnd,
    
    // NOTE(rjf): Mouse
    PlatformEventType_MouseStart,
    PlatformEventType_MousePress,
    PlatformEventType_MouseRelease,
    PlatformEventType_MouseMove,
    PlatformEventType_MouseScroll,
    PlatformEventType_MouseEnd,
    
    // NOTE(rjf): Gamepads
    PlatformEventType_GamepadStart,
    PlatformEventType_GamepadConnect,
    PlatformEventType_GamepadDisconnect,
    PlatformEventType_GamepadButtonPress,
    PlatformEventType_GamepadButtonRelease,
    PlatformEventType_GamepadJoystickMove,
    PlatformEventType_GamepadTrigger,
    PlatformEventType_GamepadEnd,
    
    PlatformEventType_Max,
}
PlatformEventType;

typedef struct PlatformEvent PlatformEvent;
struct PlatformEvent
{
    PlatformEventType type;
    Key key;
    GamepadButton gamepad_button;
    MouseButton mouse_button;
    KeyModifiers modifiers;
    i32 gamepad_index;
    u64 character;
    v2 position;
    v2 delta;
    v2 scroll;
};


internal b32
PlatformEventIsMouse(PlatformEvent *event)
{
    return event->type > PlatformEventType_MouseStart && event->type < PlatformEventType_MouseEnd;
}

internal b32
ComparePlatformEvents(PlatformEvent a, PlatformEvent b)
{
    b32 result = 0;
    if(a.type == b.type &&
       a.key == b.key &&
       a.mouse_button == b.mouse_button &&
       a.gamepad_button == b.gamepad_button &&
       a.character == b.character &&
       a.modifiers == b.modifiers)
    {
        result = 1;
    }
    return result;
}

internal PlatformEvent
KeyPressEvent(Key key, KeyModifiers modifiers)
{
    PlatformEvent event =
    {
        .type = PlatformEventType_KeyPress,
    };
    event.key = key;
    event.modifiers = modifiers;
    return event;
}

internal PlatformEvent
KeyReleaseEvent(Key key, KeyModifiers modifiers)
{
    PlatformEvent event =
    {
        .type = PlatformEventType_KeyRelease,
    };
    event.key = key;
    event.modifiers = modifiers;
    return event;
}

internal PlatformEvent
CharacterInputEvent(u64 character)
{
    PlatformEvent event =
    {
        .type = PlatformEventType_CharacterInput,
    };
    event.character = character;
    return event;
}

internal PlatformEvent
MouseMoveEvent(v2 position, v2 delta)
{
    PlatformEvent event =
    {
        .type = PlatformEventType_MouseMove,
    };
    event.position = position;
    event.delta = delta;
    return event;
}

internal PlatformEvent
MousePressEvent(MouseButton button, v2 position)
{
    PlatformEvent event =
    {
        .type = PlatformEventType_MousePress,
    };
    event.mouse_button = button;
    event.position = position;
    return event;
}

internal PlatformEvent
MouseReleaseEvent(MouseButton mouse_button, v2 position)
{
    PlatformEvent event =
    {
        .type = PlatformEventType_MouseRelease,
    };
    event.mouse_button = mouse_button;
    event.position = position;
    return event;
}

internal PlatformEvent
MouseScrollEvent(v2 delta, KeyModifiers modifiers)
{
    PlatformEvent event =
    {
        .type = PlatformEventType_MouseScroll,
    };
    event.scroll = delta;
    event.modifiers = modifiers;
    return event;
}

//~ NOTE(rjf): Platform Data

typedef struct Platform Platform;
struct Platform
{
    // NOTE(rjf): Application Metadata
    String8 executable_folder_absolute_path;
    String8 executable_absolute_path;
    String8 working_directory_path;
    
    // NOTE(rjf): Application Memory
    MemoryArena permanent_arena;
    MemoryArena scratch_arena;
    
    // NOTE(rjf): Options
    volatile b32 quit;
    b32 vsync;
    b32 fullscreen;
    iv2 window_size;
    f32 current_time;
    f32 target_frames_per_second;
    b32 wait_for_events_to_update;
    b32 pump_events;
    
    // NOTE(rjf): Event Queue and Input Data
    v2 mouse_position;
    u64 event_count;
    PlatformEvent events[4096];
    
    // NOTE(rjf): Audio Output Data
    f32 *sample_out;
    u32 sample_count_to_output;
    u32 samples_per_second;
    
    // NOTE(rjf): Functions
    void (*OutputError)(char *error_type, char *error_format, ...);
    void (*SaveToFile)(char *path, void *data, u32 data_len);
    void (*AppendToFile)(char *path, void *data, u32 data_len);
    void (*LoadEntireFile)(MemoryArena *arena, char *path, void **data, u32 *data_len);
    char *(*LoadEntireFileAndNullTerminate)(MemoryArena *arena, char *path);
    void (*DeleteFile)(char *path);
    b32 (*MakeDirectory)(char *path);
    b32 (*DoesFileExist)(char *path);
    b32 (*DoesDirectoryExist)(char *path);
    b32 (*CopyFile)(char *dest, char *source);
    PlatformDirectoryList (*ListDirectory)(MemoryArena *arena, char *path, i32 flags);
    f32 (*GetTime)(void);
    u64 (*GetCycles)(void);
    void (*ResetCursor)(void);
    void (*SetCursorToHorizontalResize)(void);
    void (*SetCursorToVerticalResize)(void);
    void (*SetCursorToIBar)(void);
    void (*RefreshScreen)(void);
    void *(*LoadOpenGLProcedure)(char *name);
};

global Platform *platform = 0;

internal b32
GetNextPlatformEvent(PlatformEvent **event)
{
    b32 result = 0;
    HardAssert(platform != 0);
    u32 start_index = 0;
    PlatformEvent *new_event = 0;
    if(*event)
    {
        start_index = (*event - platform->events) + 1;
    }
    for(u32 i = start_index; i < platform->event_count; ++i)
    {
        if(platform->events[i].type != PlatformEventType_Null)
        {
            new_event = platform->events+i;
            break;
        }
    }
    *event = new_event;
    result = new_event != 0;
    return result;
}

internal void
EatPlatformEvent(PlatformEvent *event)
{
    event->type = PlatformEventType_Null;
}

internal b32
GetMouseMove(PlatformEvent **event_ptr)
{
    b32 result = 0;
    if(event_ptr)
    {
        *event_ptr = 0;
    }
    for(PlatformEvent *event = 0; GetNextPlatformEvent(&event);)
    {
        if(event->type == PlatformEventType_MouseMove)
        {
            if(event_ptr)
            {
                *event_ptr = event;
            }
            result = 1;
            break;
        }
    }
    return result;
}

internal b32
GetMouseMoveRect(PlatformEvent **event_ptr, v4 rect)
{
    b32 result = 0;
    if(event_ptr)
    {
        *event_ptr = 0;
    }
    for(PlatformEvent *event = 0; GetNextPlatformEvent(&event);)
    {
        if(event->type == PlatformEventType_MouseMove &&
           V4RectHasPoint(rect, event->position))
        {
            if(event_ptr)
            {
                *event_ptr = event;
            }
            result = 1;
            break;
        }
    }
    return result;
}

internal b32
GetMousePressRect(PlatformEvent **event_ptr, v4 rect)
{
    b32 result = 0;
    for(PlatformEvent *event = 0; GetNextPlatformEvent(&event);)
    {
        if(event->type == PlatformEventType_MousePress &&
           V4RectHasPoint(rect, event->position))
        {
            if(event_ptr)
            {
                *event_ptr = event;
            }
            result = 1;
            break;
        }
    }
    return result;
}

internal b32
GetMouseRelease(PlatformEvent **event_ptr)
{
    b32 result = 0;
    for(PlatformEvent *event = 0; GetNextPlatformEvent(&event);)
    {
        if(event->type == PlatformEventType_MouseRelease)
        {
            if(event_ptr)
            {
                *event_ptr = event;
            }
            result = 1;
            break;
        }
    }
    return result;
}

internal b32
GetMouseReleaseRect(PlatformEvent **event_ptr, v4 rect)
{
    b32 result = 0;
    for(PlatformEvent *event = 0; GetNextPlatformEvent(&event);)
    {
        if(event->type == PlatformEventType_MouseRelease &&
           V4RectHasPoint(rect, event->position))
        {
            if(event_ptr)
            {
                *event_ptr = event;
            }
            result = 1;
            break;
        }
    }
    return result;
}

internal b32
GetKeyPress(PlatformEvent **event_ptr, Key key)
{
    b32 result = 0;
    for(PlatformEvent *event = 0; GetNextPlatformEvent(&event);)
    {
        if(event->type == PlatformEventType_KeyPress &&
           event->key == key)
        {
            if(event_ptr)
            {
                *event_ptr = event;
            }
            result = 1;
            break;
        }
    }
    return result;
}

// NOTE(rjf): Only called by platform layers. Do not call in app.
internal void
PlatformBeginFrame(void)
{
    platform->pump_events = 0;
}

// NOTE(rjf): Only called by platform layers. Do not call in app.
internal void
PlatformEndFrame(void)
{
    platform->current_time += 1.f / platform->target_frames_per_second;
}

internal void
PlatformPushEvent(PlatformEvent event)
{
    HardAssert(platform != 0);
    if(platform->event_count < ArrayCount(platform->events))
    {
        platform->events[platform->event_count++] = event;
    }
}

#ifdef _MSC_VER
#define APP_ENTRY_POINT __declspec(dllexport)
#else
#define APP_ENTRY_POINT
#endif

/* Loaded as "PermanentLoad" */
#define AppEntryPoint_PermanentLoad APP_ENTRY_POINT void PermanentLoad(Platform *platform_)
typedef void ApplicationPermanentLoadCallback(Platform *);
internal void ApplicationPermanentLoadStub(Platform *platform) {}

/* Loaded as "HotLoad" */
#define AppEntryPoint_HotLoad APP_ENTRY_POINT void HotLoad(Platform *platform_)
typedef void ApplicationHotLoadCallback(Platform *);
internal void ApplicationHotLoadStub(Platform *platform) {}

/* Loaded as "HotUnload" */
#define AppEntryPoint_HotUnload APP_ENTRY_POINT void HotUnload(void)
typedef void ApplicationHotUnloadCallback(void);
internal void ApplicationHotUnloadStub(void) {}

/* Loaded as "Update" */
#define AppEntryPoint_Update APP_ENTRY_POINT void Update(void)
typedef void ApplicationUpdateCallback(void);
internal void ApplicationUpdateStub(void) {}

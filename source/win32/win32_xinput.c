
#define XINPUT_GET_STATE(name) DWORD WINAPI name(DWORD dw_user_index, XINPUT_STATE *p_state)
typedef XINPUT_GET_STATE(XInputGetStateProc);
XINPUT_GET_STATE(XInputGetStateStub)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}
global XInputGetStateProc *XInputGetStateProcPtr = XInputGetStateStub;

#define XINPUT_SET_STATE(name) DWORD WINAPI name(DWORD dw_user_index, XINPUT_VIBRATION *p_vibration)
typedef XINPUT_SET_STATE(XInputSetStateProc);
XINPUT_SET_STATE(XInputSetStateStub)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}
global XInputSetStateProc *XInputSetStateProcPtr = XInputSetStateStub;

internal void
Win32LoadXInput(void)
{
    XInputSetStateProcPtr = XInputSetStateStub;
    XInputGetStateProcPtr = XInputGetStateStub;
    
    HMODULE x_input_lib = LoadLibraryA("xinput1_4.dll");
    if(!x_input_lib)
    {
        x_input_lib = LoadLibraryA("xinput9_1_0.dll");
    }
    if(!x_input_lib)
    {
        x_input_lib = LoadLibraryA("xinput1_3.dll");
    }
    if(x_input_lib)
    {
        XInputGetStateProcPtr = (XInputGetStateProc *)GetProcAddress(x_input_lib, "XInputGetState");
        XInputSetStateProcPtr = (XInputSetStateProc *)GetProcAddress(x_input_lib, "XInputSetState");
    }
}

internal void
Win32UpdateXInput(Platform *platform)
{
    for(u32 i = 0; i < MAX_GAMEPAD_COUNT; ++i)
    {
        if(i < XUSER_MAX_COUNT)
        {
            XINPUT_STATE controller_state = {0};
            if(XInputGetStateProcPtr(0, &controller_state) == ERROR_SUCCESS)
            {
                // NOTE(Ryan): Controller is connected
                platform->gamepads[i].connected = 1;
                XINPUT_GAMEPAD *pad = &controller_state.Gamepad;
                
                for(u32 j = 0; j < GAMEPAD_BUTTON_MAX; ++j)
                {
                    platform->gamepads[i].button_states[j] = 0;
                }
                
                platform->gamepads[i].button_states[GAMEPAD_BUTTON_dpad_up] |= !!(pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
                platform->gamepads[i].button_states[GAMEPAD_BUTTON_dpad_down] |= !!(pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
                platform->gamepads[i].button_states[GAMEPAD_BUTTON_dpad_left] |= !!(pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
                platform->gamepads[i].button_states[GAMEPAD_BUTTON_dpad_right] |= !!(pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
                platform->gamepads[i].button_states[GAMEPAD_BUTTON_start] |= !!(pad->wButtons & XINPUT_GAMEPAD_START);
                platform->gamepads[i].button_states[GAMEPAD_BUTTON_back] |= !!(pad->wButtons & XINPUT_GAMEPAD_BACK);
                platform->gamepads[i].button_states[GAMEPAD_BUTTON_left_thumb] |= !!(pad->wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
                platform->gamepads[i].button_states[GAMEPAD_BUTTON_right_thumb] |= !!(pad->wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
                platform->gamepads[i].button_states[GAMEPAD_BUTTON_left_shoulder] |= !!(pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
                platform->gamepads[i].button_states[GAMEPAD_BUTTON_right_shoulder] |= !!(pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
                platform->gamepads[i].button_states[GAMEPAD_BUTTON_a] |= !!(pad->wButtons & XINPUT_GAMEPAD_A);
                platform->gamepads[i].button_states[GAMEPAD_BUTTON_b] |= !!(pad->wButtons & XINPUT_GAMEPAD_B);
                platform->gamepads[i].button_states[GAMEPAD_BUTTON_x] |= !!(pad->wButtons & XINPUT_GAMEPAD_X);
                platform->gamepads[i].button_states[GAMEPAD_BUTTON_y] |= !!(pad->wButtons & XINPUT_GAMEPAD_Y);
                
                platform->gamepads[i].joystick_1.x = pad->sThumbLX / 32768.f;
                platform->gamepads[i].joystick_1.y = pad->sThumbLY / 32768.f;
                platform->gamepads[i].joystick_2.x = pad->sThumbRX / 32768.f;
                platform->gamepads[i].joystick_2.y = pad->sThumbRY / 32768.f;
            }
            else
            {
                // NOTE(rjf): Controller is not connected
                platform->gamepads[i].connected = 0;
            }
        }
    }
}

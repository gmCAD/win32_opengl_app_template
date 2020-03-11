
typedef struct Win32AppCode Win32AppCode;
struct Win32AppCode
{
    ApplicationPermanentLoadCallback  *PermanentLoad;
    ApplicationHotLoadCallback        *HotLoad;
    ApplicationHotUnloadCallback      *HotUnload;
    ApplicationUpdateCallback         *Update;
    HMODULE dll;
    FILETIME last_dll_write_time;
};

internal b32
Win32AppCodeLoad(Win32AppCode *app_code)
{
    b32 result = 1;
    
    CopyFile(global_app_dll_path, global_temp_app_dll_path, FALSE);
    app_code->dll = LoadLibraryA(global_temp_app_dll_path);
    
    app_code->last_dll_write_time = Win32GetLastWriteTime(global_app_dll_path);
    
    if(!app_code->dll)
    {
        result = 0;
        goto end;
    }
    
    app_code->PermanentLoad    = (void *)GetProcAddress(app_code->dll, "PermanentLoad");
    app_code->HotLoad          = (void *)GetProcAddress(app_code->dll, "HotLoad");
    app_code->HotUnload        = (void *)GetProcAddress(app_code->dll, "HotUnload");
    app_code->Update           = (void *)GetProcAddress(app_code->dll, "Update");
    
    if(!app_code->PermanentLoad || !app_code->HotLoad || !app_code->HotUnload || !app_code->Update)
    {
        app_code->PermanentLoad = ApplicationPermanentLoadStub;
        app_code->HotLoad = ApplicationHotLoadStub;
        app_code->HotUnload = ApplicationHotUnloadStub;
        app_code->Update = ApplicationUpdateStub;
        result = 0;
        goto end;
    }
    
    end:;
    return result;
}

internal void
Win32AppCodeUnload(Win32AppCode *app_code)
{
    if(app_code->dll)
    {
        FreeLibrary(app_code->dll);
    }
    app_code->dll = 0;
    app_code->PermanentLoad = ApplicationPermanentLoadStub;
    app_code->HotLoad = ApplicationHotLoadStub;
    app_code->HotUnload = ApplicationHotUnloadStub;
    app_code->Update = ApplicationUpdateStub;
}

internal void
Win32AppCodeUpdate(Win32AppCode *app_code)
{
    FILETIME last_write_time = Win32GetLastWriteTime(global_app_dll_path);
    if(CompareFileTime(&last_write_time, &app_code->last_dll_write_time))
    {
        app_code->HotUnload();
        Win32AppCodeUnload(app_code);
        Win32AppCodeLoad(app_code);
        app_code->HotLoad(&global_platform);
    }
}

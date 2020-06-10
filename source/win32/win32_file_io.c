
internal void
W32_SaveToFile(char *path, void *data, u32 data_len)
{
    HANDLE file = {0};
    {
        DWORD desired_access = GENERIC_READ | GENERIC_WRITE;
        DWORD share_mode = 0;
        SECURITY_ATTRIBUTES security_attributes = {
            (DWORD)sizeof(SECURITY_ATTRIBUTES),
            0,
            0,
        };
        DWORD creation_disposition = CREATE_ALWAYS;
        DWORD flags_and_attributes = 0;
        HANDLE template_file = 0;
        
        if((file = CreateFile(path,
                              desired_access,
                              share_mode,
                              &security_attributes,
                              creation_disposition,
                              flags_and_attributes,
                              template_file)) != INVALID_HANDLE_VALUE)
        {
            
            void *data_to_write = data;
            DWORD data_to_write_size = (DWORD)data_len;
            DWORD bytes_written = 0;
            
            WriteFile(file, data_to_write, data_to_write_size, &bytes_written, 0);
            
            CloseHandle(file);
        }
        else
        {
            W32_OutputError("File I/O Error", "Could not save to \"%s\"", path);
        }
    }
}

internal void
W32_AppendToFile(char *path, void *data, u32 data_len)
{
    HANDLE file = {0};
    {
        DWORD desired_access = FILE_APPEND_DATA;
        DWORD share_mode = 0;
        SECURITY_ATTRIBUTES security_attributes = {
            (DWORD)sizeof(SECURITY_ATTRIBUTES),
            0,
            0,
        };
        DWORD creation_disposition = OPEN_ALWAYS;
        DWORD flags_and_attributes = 0;
        HANDLE template_file = 0;
        
        if((file = CreateFileA(path,
                               desired_access,
                               share_mode,
                               &security_attributes,
                               creation_disposition,
                               flags_and_attributes,
                               template_file)) != INVALID_HANDLE_VALUE)
        {
            
            void *data_to_write = data;
            DWORD data_to_write_size = (DWORD)data_len;
            DWORD bytes_written = 0;
            
            SetFilePointer(file, 0, 0, FILE_END);
            WriteFile(file, data_to_write, data_to_write_size, &bytes_written, 0);
            
            CloseHandle(file);
        }
        else
        {
            W32_OutputError("File I/O Error", "Could not save to \"%s\"", path);
        }
    }
}

internal void
W32_LoadEntireFile(M_Arena *arena, char *path, void **data, u32 *data_len)
{
    *data = 0;
    *data_len = 0;
    
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
        
        if((file = CreateFile(path, desired_access, share_mode, &security_attributes, creation_disposition, flags_and_attributes, template_file)) != INVALID_HANDLE_VALUE)
        {
            
            DWORD read_bytes = GetFileSize(file, 0);
            if(read_bytes)
            {
                void *read_data = M_ArenaPush(arena, read_bytes+1);
                DWORD bytes_read = 0;
                OVERLAPPED overlapped = {0};
                
                ReadFile(file, read_data, read_bytes, &bytes_read, &overlapped);
                
                ((u8 *)read_data)[read_bytes] = 0;
                
                *data = read_data;
                *data_len = (u32)bytes_read;
            }
            CloseHandle(file);
        }
    }
}

internal char *
W32_LoadEntireFileAndNullTerminate(M_Arena *arena, char *path)
{
    char *result = 0;
    
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
        
        if((file = CreateFile(path, desired_access, share_mode, &security_attributes, creation_disposition, flags_and_attributes, template_file)) != INVALID_HANDLE_VALUE)
        {
            
            DWORD read_bytes = GetFileSize(file, 0);
            if(read_bytes)
            {
                result = M_ArenaPush(arena, read_bytes+1);
                DWORD bytes_read = 0;
                OVERLAPPED overlapped = {0};
                
                ReadFile(file, result, read_bytes, &bytes_read, &overlapped);
                
                result[read_bytes] = 0;
            }
            CloseHandle(file);
        }
        else
        {
            W32_OutputError("File I/O Error", "Could not read from \"%s\"", path);
        }
    }
    
    return result;
}

internal void
W32_FreeFileMemory(void *data)
{
    W32_HeapFree(data);
}

internal void
W32_DeleteFile(char *path)
{
    DeleteFileA(path);
}

internal b32
W32_MakeDirectory(char *path)
{
    b32 result = 1;
    if(!CreateDirectoryA(path, 0))
    {
        result = 0;
    }
    return result;
}

internal b32
W32_DoesFileExist(char *path)
{
    b32 found = GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES;
    return found;
}

internal b32
W32_DoesDirectoryExist(char *path)
{
    DWORD file_attributes = GetFileAttributesA(path);
    b32 found = (file_attributes != INVALID_FILE_ATTRIBUTES &&
                 !!(file_attributes & FILE_ATTRIBUTE_DIRECTORY));
    return found;
}

internal b32
W32_CopyFile(char *dest, char *source)
{
    b32 success = 0;
    success = CopyFile(source, dest, 0);
    return success;
}

internal OS_DirectoryList
W32_DirectoryListLoad(M_Arena *arena, char *path, i32 flags)
{
    OS_DirectoryList list = {0};
    
    return list;
}

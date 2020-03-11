
internal void
Win32SaveToFile(char *path, void *data, u32 data_len)
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
            Win32OutputError("File I/O Error", "Could not save to \"%s\"", path);
        }
    }
}

internal void
Win32AppendToFile(char *path, void *data, u32 data_len)
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
            Win32OutputError("File I/O Error", "Could not save to \"%s\"", path);
        }
    }
}

internal void
Win32LoadEntireFile(char *path, void **data, u32 *data_len, b32 error_on_non_existence)
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
                void *read_data = Win32HeapAlloc(read_bytes + 1);
                DWORD bytes_read = 0;
                OVERLAPPED overlapped = {0};
                
                ReadFile(file, read_data, read_bytes, &bytes_read, &overlapped);
                
                ((u8 *)read_data)[read_bytes] = 0;
                
                *data = read_data;
                *data_len = (u32)bytes_read;
            }
            CloseHandle(file);
        }
        else
        {
            if(error_on_non_existence)
            {
                Win32OutputError("File I/O Error", "Could not read from \"%s\"", path);
            }
        }
    }
}

internal char *
Win32LoadEntireFileAndNullTerminate(char *path)
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
                result = (char *)Win32HeapAlloc(read_bytes + 1);
                DWORD bytes_read = 0;
                OVERLAPPED overlapped = {0};
                
                ReadFile(file, result, read_bytes, &bytes_read, &overlapped);
                
                result[read_bytes] = 0;
            }
            CloseHandle(file);
        }
        else
        {
            Win32OutputError("File I/O Error", "Could not read from \"%s\"", path);
        }
    }
    
    return result;
}

internal void
Win32FreeFileMemory(void *data)
{
    Win32HeapFree(data);
}

internal void
Win32DeleteFile(char *path)
{
    DeleteFileA(path);
}

internal b32
Win32MakeDirectory(char *path)
{
    b32 result = 1;
    if(!CreateDirectoryA(path, 0))
    {
        result = 0;
    }
    return result;
}

internal b32
Win32DoesFileExist(char *path)
{
    b32 found = GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES;
    return found;
}

internal b32
Win32DoesDirectoryExist(char *path)
{
    DWORD file_attributes = GetFileAttributesA(path);
    b32 found = (file_attributes != INVALID_FILE_ATTRIBUTES &&
                 !!(file_attributes & FILE_ATTRIBUTE_DIRECTORY));
    return found;
}

internal b32
Win32CopyFile(char *dest, char *source)
{
    b32 success = 0;
    success = CopyFile(source, dest, 0);
    return success;
}

internal b32
Win32CopyDirectoryRecursively(char *dest, char *source)
{
    b32 success = 0;
    
    Win32MakeDirectory(dest);
    
    char task_memory[Kilobytes(32)];
    MemoryArena arena_ = MemoryArenaInit(task_memory, sizeof(task_memory));
    MemoryArena *arena = &arena_;
    
    typedef struct DirectorySearchTask DirectorySearchTask;
    struct DirectorySearchTask
    {
        char *sub_search_pattern;
        char *copy_pattern;
        DirectorySearchTask *next;
    };
    DirectorySearchTask *first_search_task = 0;
    DirectorySearchTask **target_search_task = &first_search_task;
    
#define QueueTask(path) {\
DirectorySearchTask *new_task = MemoryArenaAllocate(arena, sizeof(*new_task));\
if(new_task != 0)\
{\
new_task->sub_search_pattern = path;\
new_task->copy_pattern = path;\
new_task->next = 0;\
*target_search_task = new_task;\
target_search_task = &(*target_search_task)->next;\
}\
else\
{\
LogWarning("[File I/O] Directory search task space exhausted.");\
}\
}
    
    QueueTask("");
    
    for(DirectorySearchTask *task = first_search_task; task; task = task->next)
    {
        // NOTE(rjf): Load file list
        {
            WIN32_FIND_DATAA file_find_data;
            DWORD error = 0;
            char search_pattern[MAX_PATH] = {0};
            snprintf(search_pattern, MAX_PATH, "%s\\%s*", source, task->sub_search_pattern);
            HANDLE file_handle = INVALID_HANDLE_VALUE;
            file_handle = FindFirstFileA(search_pattern, &file_find_data);
            Log("[File I/O] Copying \"%s\" to \"%s\".", source, dest);
            
            if(file_handle != INVALID_HANDLE_VALUE)
            {
                do
                {
                    if(file_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    {
                        if(!CStringMatchCaseSensitive(file_find_data.cFileName, ".") &&
                           !CStringMatchCaseSensitive(file_find_data.cFileName, ".."))
                        {
                            char directory_name[MAX_PATH] = {0};
                            MemoryCopy(directory_name, file_find_data.cFileName, sizeof(directory_name));
                            QueueTask(MakeCStringOnMemoryArena(arena, "%s%s\\", task->sub_search_pattern, directory_name));
                            
                            char copy_directory_name[MAX_PATH] = {0};
                            snprintf(copy_directory_name, sizeof(copy_directory_name), "%s\\%s\\%s",
                                     dest, task->sub_search_pattern, directory_name);
                            
                            Log("[File I/O] Copying \"%s\".", copy_directory_name);
                            Win32MakeDirectory(copy_directory_name);
                        }
                    }
                    else
                    {
                        // NOTE(rjf): Copy file.
                        {
                            char item_name[MAX_PATH] = {0};
                            MemoryCopy(item_name, file_find_data.cFileName, sizeof(item_name));
                            
                            char *source_path = MakeCStringOnMemoryArena(arena, "%s\\%s%s", source,
                                                                         task->sub_search_pattern,
                                                                         item_name);
                            
                            char *dest_path = MakeCStringOnMemoryArena(arena, "%s\\%s%s", dest,
                                                                       task->sub_search_pattern,
                                                                       item_name);
                            
                            Log("[File I/O] Copying \"%s\" to \"%s\".", source_path, dest_path);
                            Win32CopyFile(dest_path, source_path);
                        }
                    }
                }
                while(FindNextFileA(file_handle, &file_find_data));
            }
        }
    }
    
#undef QueueTask
    
    return success;
}

// NOTE(rjf): This only needs to be implemented for a development platform.
internal PlatformDirectoryList
Win32PlatformDirectoryListLoad(char *path, i32 flags)
{
    int item_count = 0;
    int item_capacity = 0;
    char **items = 0;
    i32 *item_flags = 0;
    
    char task_memory[Kilobytes(32)];
    MemoryArena arena_ = MemoryArenaInit(task_memory, sizeof(task_memory));
    MemoryArena *arena = &arena_;
    
    typedef struct DirectorySearchTask DirectorySearchTask;
    struct DirectorySearchTask
    {
        char *sub_search_pattern;
        DirectorySearchTask *next;
    };
    DirectorySearchTask *first_search_task = 0;
    DirectorySearchTask **target_search_task = &first_search_task;
    
#define QueueTask(path) {\
DirectorySearchTask *new_task = MemoryArenaAllocate(arena, sizeof(*new_task));\
if(new_task != 0)\
{\
new_task->sub_search_pattern = path;\
new_task->next = 0;\
*target_search_task = new_task;\
target_search_task = &(*target_search_task)->next;\
}\
else\
{\
LogWarning("[File I/O] Directory search task space exhausted.");\
}\
}
    
    QueueTask("");
    
    for(DirectorySearchTask *task = first_search_task; task; task = task->next)
    {
        // NOTE(rjf): Load file list
        {
            WIN32_FIND_DATAA file_find_data;
            DWORD error = 0;
            char search_pattern[MAX_PATH] = {0};
            char find_file_path[MAX_PATH] = {0};
            snprintf(search_pattern, MAX_PATH, "%s\\%s*", path, task->sub_search_pattern);
            HANDLE file_handle = INVALID_HANDLE_VALUE;
            file_handle = FindFirstFileA(search_pattern, &file_find_data);
            Log("[File I/O] Searching \"%s\" directory%s.", find_file_path, (flags & PLATFORM_DIRECTORY_LIST_RECURSIVE) ? " recursively" : "");
            
            if(file_handle != INVALID_HANDLE_VALUE)
            {
                do
                {
                    
                    if(flags & PLATFORM_DIRECTORY_LIST_RECURSIVE &&
                       file_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    {
                        if(!CStringMatchCaseSensitive(file_find_data.cFileName, ".") &&
                           !CStringMatchCaseSensitive(file_find_data.cFileName, ".."))
                        {
                            char directory_name[MAX_PATH] = {0};
                            MemoryCopy(directory_name, file_find_data.cFileName, sizeof(directory_name));
                            QueueTask(MakeCStringOnMemoryArena(arena, "%s%s\\", task->sub_search_pattern, directory_name));
                        }
                    }
                    
                    if(!(file_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ||
                       (flags & PLATFORM_DIRECTORY_LIST_DIRECTORIES))
                    {
                        // NOTE(rjf): Make room for new item if we need to.
                        {
                            if(item_count + 1 >= item_capacity)
                            {
                                if(item_capacity == 0)
                                {
                                    item_capacity = 64;
                                }
                                else
                                {
                                    item_capacity <<= 1;
                                }
                                char **new_items = Win32HeapAlloc(item_capacity * sizeof(char *));
                                i32 *new_item_flags = Win32HeapAlloc(item_capacity * sizeof(i32));
                                MemoryCopy(new_items, items, sizeof(char *) * item_count);
                                MemoryCopy(new_item_flags, item_flags, sizeof(i32) * item_count);
                                if(items)
                                {
                                    Win32HeapFree(items);
                                    Win32HeapFree(item_flags);
                                }
                                items = new_items;
                                item_flags = new_item_flags;
                            }
                        }
                        
                        // NOTE(rjf): Allocate new item.
                        char *new_item = 0;
                        i32 new_item_flag = 0;
                        {
                            MemoryCopy(find_file_path, file_find_data.cFileName, sizeof(find_file_path));
                            unsigned int needed_bytes = CalculateCStringLength(find_file_path) + CalculateCStringLength(task->sub_search_pattern) + 1;
                            new_item = Win32HeapAlloc(needed_bytes + 1);
                            snprintf(new_item, needed_bytes, "%s%s", task->sub_search_pattern, find_file_path);
                            new_item[needed_bytes] = 0;
                            
                            if(file_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                            {
                                new_item_flag |= PLATFORM_DIRECTORY_ITEM_DIRECTORY;
                            }
                        }
                        
                        // NOTE(rjf): Remove extension from path.
                        if(!(flags & PLATFORM_DIRECTORY_LIST_EXTENSIONS))
                        {
                            char *last_period = new_item;
                            for(int i = 0; new_item[i]; ++i)
                            {
                                if(new_item[i] == '.')
                                {
                                    last_period = new_item + i;
                                }
                            }
                            *last_period = 0;
                        }
                        
                        // NOTE(rjf): Replace backslashes with forward slashes.
                        {
                            for(int i = 0; new_item[i]; ++i)
                            {
                                if(new_item[i] == '\\')
                                {
                                    new_item[i] = '/';
                                }
                            }
                        }
                        
                        // NOTE(rjf): Add item.
                        {
                            items[item_count] = new_item;
                            item_flags[item_count] = new_item_flag;
                            ++item_count;
                            Log("[File I/O] Found directory item \"%s\".", new_item);
                        }
                    }
                }
                while(FindNextFileA(file_handle, &file_find_data));
            }
        }
    }
    
#undef QueueTask
    
    PlatformDirectoryList list = {0};
    list.flags = flags;
    list.item_count = item_count;
    list.items = items;
    list.item_flags = item_flags;
    
    return list;
}

internal void
Win32PlatformDirectoryListCleanUp(PlatformDirectoryList *list)
{
    for(u32 i = 0; i < list->item_count; ++i)
    {
        Win32HeapFree(list->items[i]);
    }
    Win32HeapFree(list->items);
    Win32HeapFree(list->item_flags);
    list->item_count = 0;
    list->items = 0;
}

typedef struct App App;
struct App
{
    int app_data_goes_here;
};

global App *app = 0;
global Platform *platform = 0;

internal void
Update(Platform *platform_)
{
    platform = platform_;
    app = platform->permanent_storage;
    
    if(platform->initialized == 0)
    {
        platform->initialized = 1;
    }
    
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    platform->SwapBuffers();
}
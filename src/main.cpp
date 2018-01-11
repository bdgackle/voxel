// External Headers
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// C Standard Headers
#include <stdio.h>

// CPP Standard Headers
#include <string>

using namespace std;

//Screen dimension constants
static const int s_screen_width = 640;
static const int s_screen_height = 480;

SDL_Window* s_window;
SDL_GLContext s_context;

static bool _sdl_init(void)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

static void _sdl_cleanup()
{
    SDL_DestroyWindow(s_window);

    //Quit SDL subsystems
    SDL_Quit();
}

static bool _sdl_opengl_init(void)
{
    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3) != 0) {
        printf("Error setting SDL_GL_CONTEXT_MAJOR_VERSION\n");
        return false;
    }

    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3) != 0) {
        printf("Error setting SDL_GL_CONTEXT_MINOR_VERSION\n");
        return false;
    }

    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) != 0) {
        printf("Error setting SDL_GL_CONTEXT_PROFILE_MASK\n");
        return false;
    }

    return true;
}

static bool _sdl_create_main_window(void)
{
    // Create main window
    s_window = SDL_CreateWindow("OpenGL Tutorial",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                s_screen_width,
                                s_screen_height,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (s_window == NULL) {
        printf("Error creating OpenGL window: %s.\n", SDL_GetError());
        return false;
    }

    return true;
}

static bool _sdl_create_opengl_context(void)
{
    s_context = SDL_GL_CreateContext(s_window);
    if (s_context == NULL) {
        printf("Error creating OpenGL context: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

static bool _sdl_set_swap_interval(void)
{
    if (SDL_GL_SetSwapInterval(1) < 0) {
        printf("Setting swap intervale not supported.\n");
        return false;
    }

    return true;
}

static bool _init(void)
{
    if (!_sdl_init()) {
        printf("Init failed.\n");
        return false;
    }

    if (!_sdl_opengl_init()) {
        printf("OpenGL init failed.\n");
        return false;
    }

    if (!_sdl_create_main_window()) {
        printf("Window creation failed.\n");
        return false;
    }

    if (!_sdl_create_opengl_context()) {
        printf("OpenGL context creation failed.\n");
        return false;
    }

    if (!_sdl_set_swap_interval()) {
        printf("VSync init failed.\n");
        return false;
    }

    return true;
}

int main(int argc, char** argv)
{
    if (!_init()) {
        printf("Exiting due to failed initialization.\n");
        return 1;
    }

    bool quit = false;
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_GL_SwapWindow(s_window);
    }

    //Free resources and close SDL
    _sdl_cleanup();

    return 0;
}

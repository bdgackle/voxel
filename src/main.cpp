// External Headers
#include <glad/glad.h>
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
    if (s_window != nullptr) {
        SDL_DestroyWindow(s_window);
        s_window = nullptr;
    }

    if (s_context != nullptr) {
        SDL_GL_DeleteContext(s_context);
        s_context = nullptr;
    }

    //Quit SDL subsystems
    SDL_Quit();
}

static bool _sdl_create_main_window(void)
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

    // Create main window
    s_window = SDL_CreateWindow("OpenGL Tutorial",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                s_screen_width,
                                s_screen_height,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (s_window == nullptr) {
        printf("Error creating OpenGL window: %s.\n", SDL_GetError());
        return false;
    }

    return true;
}

static bool _sdl_create_opengl_context(void)
{
    s_context = SDL_GL_CreateContext(s_window);
    if (s_context == nullptr) {
        printf("Error creating OpenGL context: %s\n", SDL_GetError());
        return false;
    }

    if (SDL_GL_SetSwapInterval(1) < 0) {
        printf("Setting swap intervale not supported.\n");
        return false;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return false;
    }

    return true;
}

static bool _init(void)
{
    if (!_sdl_init()) {
        printf("SDL init failed.\n");
        return false;
    }

    if (!_sdl_create_main_window()) {
        printf("SDL window creation failed.\n");
        return false;
    }

    if (!_sdl_create_opengl_context()) {
        printf("OpenGL context creation failed.\n");
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

    // Initial viewport call (should be tied to SDL window resize callback)
    glViewport(0, 0, s_screen_width, s_screen_height);

    // Set background color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

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

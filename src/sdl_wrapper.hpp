#ifndef SDL_WRAPPER_HPP
#define SDL_WRAPPER_HPP

// External Headers
#include <SDL2/SDL.h>

// C Standard Headers
#include <cstdint>

class sdl_sdk {
    friend class sdl_wrapper;

 private:
    sdl_sdk();
    ~sdl_sdk();
};

class sdl_window {
    friend class sdl_opengl_context;
    friend class sdl_wrapper;

 private:
    sdl_window();
    ~sdl_window();

    SDL_Window* m_window;
};

class sdl_opengl_context {
    friend class sdl_wrapper;

 private:
    sdl_opengl_context();
    ~sdl_opengl_context();

    sdl_window m_gl_window;
    SDL_GLContext m_context;
};

class sdl_wrapper {
 public:
    sdl_wrapper();

    SDL_Window* sdl_window();

 private:
    sdl_sdk m_sdk;
    sdl_opengl_context m_context;
};

#endif // SDL_WRAPPER_HPP

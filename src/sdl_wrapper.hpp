#ifndef SDL_WRAPPER_HPP
#define SDL_WRAPPER_HPP

// External Headers
#include <SDL2/SDL.h>

// C Standard Headers
#include <cstdint>

namespace sdl_wrapper {

class sdk {
    friend class wrapper;

 private:
    sdk();
    ~sdk();
};

class sdl_window {
    friend class opengl_context;
    friend class wrapper;

 private:
    sdl_window(int width, int height);
    ~sdl_window();

    SDL_Window* m_window;
};

class opengl_context {
    friend class wrapper;

 private:
    opengl_context(SDL_Window* w);
    ~opengl_context();

    SDL_GLContext m_context;
};

class wrapper {
 public:
    wrapper(int width, int height);
    SDL_Window* window();

 private:
    sdk m_sdk;
    sdl_window m_window;
    opengl_context m_context;
};

} // namespace sdl_wrapper

#endif // SDL_WRAPPER_HPP

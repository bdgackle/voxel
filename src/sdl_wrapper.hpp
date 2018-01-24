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

class window {
    friend class opengl_context;
    friend class wrapper;

 private:
    window();
    ~window();

    SDL_Window* m_window;
};

class opengl_context {
    friend class wrapper;

 private:
    opengl_context();
    ~opengl_context();

    window m_gl_window;
    SDL_GLContext m_context;
};

class wrapper {
 public:
    wrapper();

    SDL_Window* window();

 private:
    sdk m_sdk;
    opengl_context m_context;
};

} // namespace sdl_wrapper

#endif // SDL_WRAPPER_HPP

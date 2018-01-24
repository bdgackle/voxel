// Local Headers
#include "glad/glad.h"
#include "sdl_wrapper.hpp"

// External Headers
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// C++ Standard Headers
#include <stdexcept>

// C Standard Headers
#include <cassert>
#include <cstddef>
#include <cstdint>

using namespace std;

namespace sdl_wrapper {

class init_exception : public exception {
    virtual const char* what() const throw()
    {
        return "SDL initialization failed.";
    }
} init_ex;

class window_exception : public exception {
    virtual const char* what() const throw()
    {
        return "SDL OpenGL window creation failed.";
    }
} window_ex;

class gl_context_exception : public exception {
    virtual const char* what() const throw()
    {
        return "SDL OpenGL context creation failed.";
    }
} context_ex;

class gl_swap_exception : public exception {
    virtual const char* what() const throw()
    {
        return "Failed to set SDL swap interval.";
    }
} swap_ex;

class glad_exception: public exception {
    virtual const char* what() const throw()
    {
        return "Failed to initialized GLAD.";
    }
} glad_ex;

sdk::sdk()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw init_ex;
    }
}

sdk::~sdk()
{
    SDL_Quit();
}

window::window()
{
    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3) < 0) {
        throw window_ex;
    }

    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3) < 0) {
        throw window_ex;
    }

    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) < 0) {
        throw window_ex;
    }

    // Create main window
    m_window = SDL_CreateWindow("OpenGL Tutorial",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                640,
                                480,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (m_window == nullptr) {
        throw window_ex;
    }
}

window::~window()
{
    assert(m_window != nullptr);
    SDL_DestroyWindow(m_window);
}

opengl_context::opengl_context()
{
    assert(m_gl_window.m_window != nullptr);
    m_context = SDL_GL_CreateContext(m_gl_window.m_window);
    if (m_context == nullptr) {
        throw context_ex;
    }
}

opengl_context::~opengl_context()
{
    assert(m_context != NULL);
    SDL_GL_DeleteContext(m_context);
}

wrapper::wrapper()
{
    if (SDL_GL_SetSwapInterval(1) < 0) {
        throw swap_ex;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        throw swap_ex;
    }
}

SDL_Window* wrapper::window()
{
    assert(m_context.m_gl_window.m_window != nullptr);
    return m_context.m_gl_window.m_window;
}

} // namespace sdl_wrapper

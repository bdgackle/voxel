// Local Headers
#include "gl_wrapper.hpp"

// C Standard Headers
#include <cassert>

// C++ Standard Headers
#include <stdexcept>

using namespace std;

class gl_shader_type_exception : public exception {
    virtual const char* what() const throw()
    {
        return "Unsupported shader type.";
    }
} type_ex;

class gl_shader_creation_exception : public exception {
    virtual const char* what() const throw()
    {
        return "Error creating shader.";
    }
} shader_ex;

gl_wrapper_shader::gl_wrapper_shader(GLenum shader_type)
{
    switch (shader_type) {
        case GL_VERTEX_SHADER:   // Fall through
        case GL_FRAGMENT_SHADER:
            break;

        default:
            throw type_ex;
    }

    m_handle = glCreateShader(shader_type);
    if (m_handle == 0) {
        throw shader_ex;
    }
}

gl_wrapper_shader::~gl_wrapper_shader()
{
    assert(m_handle != 0);
    glDeleteShader(m_handle);
}

// Local Headers
#include "gl_wrapper.hpp"

// C Standard Headers
#include <cassert>
#include <cstddef>
#include <cstdio>

// C++ Standard Headers
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

namespace gl_wrapper {

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

class gl_program_creation_exception : public exception {
    virtual const char* what() const throw()
    {
        return "Error creating shader.";
    }
} program_ex;

class gl_shader_compile_exception: public exception {
    virtual const char* what() const throw()
    {
        return "Error compiling shader.";
    }
} compile_ex;

class gl_shader_link_exception: public exception {
    virtual const char* what() const throw()
    {
        return "Error linking shader.";
    }
} link_ex;

shader::shader(GLenum shader_type)
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

shader::~shader()
{
    assert(m_handle != 0);

    glDeleteShader(m_handle);
}

void shader::compile(string filename)
{
    assert(m_handle != 0);

    string source = read_shader_source(filename);
    const char* source_str = source.c_str();
    glShaderSource(m_handle, 1, &source_str, NULL);
    glCompileShader(m_handle);

    int status;
    glGetShaderiv(m_handle, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        throw compile_ex;
    }
}

program::program()
{
    m_handle = glCreateProgram();
    if (m_handle == 0) {
        throw program_ex;
    }
}

program::~program()
{
    assert(m_handle != 0);
    glDeleteProgram(m_handle);
}

void program::link(GLuint vertex_shader, GLuint fragment_shader)
{
    glAttachShader(m_handle, vertex_shader);
    glAttachShader(m_handle, fragment_shader);
    glLinkProgram(m_handle);

    int status;
    glGetProgramiv(m_handle, GL_LINK_STATUS, &status);
    if (status == 0) {
        char log[512];
        glGetProgramInfoLog(m_handle, 512, NULL, log);
        printf("%s\n", log);
        throw link_ex;
    }
}

shader_program::shader_program(string vertex_filename, string fragment_filename) :
    m_vertex_shader(GL_VERTEX_SHADER),
    m_fragment_shader(GL_FRAGMENT_SHADER)
{
    m_vertex_shader.compile(vertex_filename);
    m_fragment_shader.compile(fragment_filename);
    m_program.link(m_vertex_shader.m_handle, m_fragment_shader.m_handle);
}

GLuint shader_program::handle()
{
    return m_program.m_handle;
}

string read_shader_source(string filename)
{
    ifstream source_file;
    source_file.open(filename);

    auto ss = std::ostringstream{};
    ss << source_file.rdbuf();
    return ss.str();
}

} // namespace gl_wrapper

// Local Headers
#include "gl_wrapper.hpp"
#include "stb_image.h"

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

class gl_image_open_exception: public exception {
    virtual const char* what() const throw()
    {
        return "Error opening shader image file.";
    }
} image_ex;

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

    if (!compile_success()) {
        print_compile_msg();
        throw compile_ex;
    }
}

bool shader::compile_success()
{
    assert(m_handle != 0);

    int status;
    glGetShaderiv(m_handle, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        return false;
    }

    return true;
}

void shader::print_compile_msg()
{
    assert(m_handle != 0);

    char msg_buf[512];

    glGetProgramInfoLog(m_handle, 512, NULL, msg_buf);
    printf("%s\n", msg_buf);
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

GLuint program::handle() const
{
    return m_handle;
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

void shader_program::use()
{
    glUseProgram(m_program.handle());
}

image::image(string image_filename)
{
    m_image_data = stbi_load(image_filename.c_str(),
                             &m_width,
                             &m_height,
                             &m_channels,
                             0);

    if (m_image_data == nullptr) {
        throw image_ex;
    }
}

image::~image()
{
    stbi_image_free(m_image_data);
}

unsigned char *image::data()
{
    return m_image_data;
}

int image::width() const
{
    return m_width;
}

int image::height() const
{
    return m_height;
}

int image::channels() const
{
    return m_channels;
}

texture::texture(string image_filename) :
    m_image(image_filename)
{
    glGenTextures(1, &m_handle);
    bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 m_image.width(),
                 m_image.height(),
                 0, GL_RGB, GL_UNSIGNED_BYTE,
                 m_image.data());
    glGenerateMipmap(GL_TEXTURE_2D);
}

void texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, m_handle);
}

string read_shader_source(string filename)
{
    ifstream source_file;
    source_file.open(filename);

    auto ss = std::ostringstream{};
    ss << source_file.rdbuf();
    return ss.str();
}

void clear_screen()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

} // namespace gl_wrapper

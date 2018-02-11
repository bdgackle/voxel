// Local Headers
#include "gl_wrapper.hpp"
#include "sdl_wrapper.hpp"

// External Headers
#include <glad/glad.h>
#include <SDL2/SDL.h>

// C Standard Headers
#include <cmath>
#include <cstdint>
#include <cstdio>

// CPP Standard Headers
#include <string>

using namespace std;

//Screen dimension constants
static int s_screen_width = 640;
static int s_screen_height = 480;

static float s_vertices[] = {
     0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
     0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
    -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 0.0f,    0.0f, 1.0f
};

static unsigned int s_indices[] = {
    0, 1, 3,
    1, 2, 3
};

static void enable_vertex_attrib()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

static void enable_color_attrib()
{
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

static void enable_texture_attrib()
{
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

int main(int argc, char** argv)
{
    sdl_wrapper::wrapper sdk(s_screen_width, s_screen_height);
    SDL_Window* window = sdk.window();

    gl_wrapper::vao box_vao;
    gl_wrapper::vbo box_vbo;
    gl_wrapper::ebo box_ebo;

    box_vao.bind();
    box_vbo.load(s_vertices, sizeof(s_vertices));
    box_ebo.load(s_indices, sizeof(s_indices));

    enable_vertex_attrib();
    enable_color_attrib();
    enable_texture_attrib();

    gl_wrapper::shader_program program("vertex.glsl", "fragment.glsl");

    gl_wrapper::texture texture0("container.jpg");

    bool quit = false;
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    glViewport(0, 0, e.window.data1, e.window.data2);
                }
            }
        }

        gl_wrapper::clear_screen();
        program.use();

        glActiveTexture(GL_TEXTURE0);
        texture0.bind();

        box_vao.bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        SDL_GL_SwapWindow(window);
    }

    return 0;
}

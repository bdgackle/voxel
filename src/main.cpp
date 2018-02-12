// Local Headers
#include "gl_wrapper.hpp"
#include "sdl_wrapper.hpp"

// External Headers
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL.h>

// C Standard Headers
#include <cmath>
#include <cstdint>
#include <cstdio>

// CPP Standard Headers
#include <string>

using namespace std;

//Screen dimension constants
static int s_screen_width = 3200;
static int s_screen_height = 1800;

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

static float time()
{
    uint32_t ticks = SDL_GetTicks();
    return float(ticks) / 1000;
}



static float s_trans = 0.2;

static void increase_trans()
{
    if (s_trans <= 0.9) {
        s_trans += 0.1;
    }
}

static void decrease_trans()
{
    if (s_trans >= 0.1) {
        s_trans -= 0.1;
    }
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

    gl_wrapper::texture texture0("container.jpg", false);
    gl_wrapper::texture texture1("awesomeface.png", true);

    program.use();
    program.set_uniformi("texture0", 0);
    program.set_uniformi("texture1", 1);
    program.set_uniformf("trans", (float)0.2);

    bool quit = false;
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;

                case SDL_WINDOWEVENT:
                    if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                        glViewport(0, 0, e.window.data1, e.window.data2);
                    }
                    break;

                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym) {
                        case SDLK_j:
                            increase_trans();
                            program.set_uniformf("trans", s_trans);
                            break;
                        case SDLK_k:
                            decrease_trans();
                            program.set_uniformf("trans", s_trans);
                            break;
                    }
            }
        }

        glm::mat4 trans(1.0f);
        trans = glm::rotate(trans, time(), glm::vec3(0.0, 1.0, 0.0));
        trans = glm::rotate(trans, (float)(time() * 0.5), glm::vec3(1.0, 0.0, 0.0));
        trans = glm::rotate(trans, (float)(time() * 0.3), glm::vec3(0.0, 0.0, 1.0));
        trans = glm::translate(trans, glm::vec3(0.5, -0.5, 0.0));
        trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
        program.set_uniform4fv("transform", glm::value_ptr(trans));

        gl_wrapper::clear_screen();
        program.use();

        glActiveTexture(GL_TEXTURE0);
        texture0.bind();

        glActiveTexture(GL_TEXTURE1);
        texture1.bind();

        box_vao.bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        SDL_GL_SwapWindow(window);
    }

    return 0;
}

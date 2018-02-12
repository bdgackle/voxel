// Local Headers
#include "cube.hpp"
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

int main(int argc, char** argv)
{
    sdl_wrapper::wrapper sdk(s_screen_width, s_screen_height);
    SDL_Window* window = sdk.window();

    cube voxel_cube;


    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f),
                                  (float)s_screen_width / (float)s_screen_height,
                                  0.1f, 100.0f);

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
            }
        }

        gl_wrapper::clear_screen();

        voxel_cube.draw(0, 0, 0, projection, view);

        SDL_GL_SwapWindow(window);
    }

    return 0;
}

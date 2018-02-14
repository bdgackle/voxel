// Local Headers
#include "camera.hpp"
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
static int s_screen_width = 640;
static int s_screen_height = 480;

int main(int argc, char** argv)
{
    sdl_wrapper::wrapper sdk(s_screen_width, s_screen_height);
    SDL_Window* window = sdk.window();

    glm::mat4 proj = glm::perspective(glm::radians(45.0f),
                                      (float)s_screen_width / (float)s_screen_height,
                                      0.1f, 100.0f);

    cube voxel_cube;
    camera cam;

    voxel_cube.set_projection(proj);

    uint32_t frames = 0;
    float total_time = 0;
    bool quit = false;
    uint32_t last_frame = SDL_GetTicks();

    glm::mat4 model_vectors[100][20][20];
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 20; j++) {
            for (int k = 0; k < 20; k++) {
                glm::mat4 model(1.0f);
                model_vectors[i][j][k] = glm::translate(model, glm::vec3((float)i, (float)j, (float)k));
            }
        }
    }

    while (!quit) {
        uint32_t current_frame = SDL_GetTicks();
        float delta = current_frame - last_frame;
        last_frame = current_frame;

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
                        case SDLK_w:        cam.move_forward(delta); break;
                        case SDLK_s:        cam.move_back(delta);    break;
                        case SDLK_a:        cam.move_left(delta);    break;
                        case SDLK_d:        cam.move_right(delta);   break;
                        case SDLK_UP:       cam.pitch_up(delta);     break;
                        case SDLK_DOWN:     cam.pitch_down(delta);   break;
                        case SDLK_LEFT:     cam.yaw_left(delta);     break;
                        case SDLK_RIGHT:    cam.yaw_right(delta);    break;
                        default: /* No action */                     break;
                    }
            }
        }

        gl_wrapper::clear_screen();

        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 20; j++) {
                for (int k = 0; k < 20; k++) {
                    voxel_cube.draw(model_vectors[i][j][k], cam.view());
                }
            }
        }

        // Hack in an FPS counter
        frames++;
        total_time += delta;
        if (frames >= 100) {
            float frame_time = (float)total_time / (float)frames;
            printf("100 frames in %.2f ms.\n", total_time);
            printf("%.2f fps\n\n", 1000.0f / frame_time);
            frames = 0;
            total_time = 0;
        }

        SDL_GL_SwapWindow(window);
    }

    return 0;
}

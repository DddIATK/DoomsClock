#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <iostream>

#include "UI/UISystem.hpp"

SDL_Window* window = nullptr;

SDL_GLContext glContext = nullptr; 

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    window = SDL_CreateWindow(
        "Dooms Clock", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        1920, 1080,
        SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN |SDL_WINDOW_OPENGL
    );
    if (window == nullptr)
    {
        std::cerr << "Fail to create window!" << std::endl;
        SDL_Quit();
        return -1;
    }

    glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr)
    {
        std::cerr << "Fail to create OpenGL context!" << std::endl;
        SDL_Quit();
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress))
    {
        std::cerr << "Fail to load GLAD!" << std::endl;
        SDL_Quit();
        return -1;
    }

    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);

    UISystem ui(window, glContext);

    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYUP) 
            {
                if (event.key.keysym.sym == SDLK_ESCAPE) 
                    running = false;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.3f, 0.4f, 0.4f, 1.0f);

        ui.loop(event);

        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

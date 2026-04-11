#include "GLWindow.h"
#include <cstdio>

GLWindow::GLWindow(const char* title, int width, int height)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "SDL Init Error: %s\n", SDL_GetError());
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        fprintf(stderr, "SDL Create Window Error: %s\n", SDL_GetError());
    }

    context = SDL_GL_CreateContext(window);

    if (!context)
    {
        fprintf(stderr, "SDL Create GL Context Error: %s\n", SDL_GetError());
    }

    SDL_GL_MakeCurrent(window, context);
}

GLWindow::~GLWindow()
{
    if (context)
    {
        SDL_GL_DeleteContext(context);
    }

    if (window)
    {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
}

void GLWindow::SwapBuffers() const
{
    SDL_GL_SwapWindow(window);
}

void GLWindow::SetVSync(bool enabled) const
{
    if (SDL_GL_SetSwapInterval(enabled ? 1 : 0) != 0)
    {
        fprintf(stderr, "VSync not supported");
    }
}

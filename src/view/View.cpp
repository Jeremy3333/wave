#include <iostream>

#include "View.hpp"
#include "Const.hpp"

View::View(Controller * p_controller): _controller(p_controller), _window(nullptr), _renderer(nullptr), _event(), lastFrameTime(0), frameDelay(1000 / FRAME_RATE) {
    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        throw std::runtime_error("SDL initialization failed");
    }

    // Create a window
    _window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if(!_window){
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        throw std::runtime_error("Window creation failed");
    }

    // Create a renderer
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!_renderer){
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(_window);
        SDL_Quit();
        throw std::runtime_error("Renderer creation failed");
    }
}

View::~View() {
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

bool View::input(void) {
    bool isRunning = true;
    while (SDL_PollEvent(&_event))
    {
        switch (_event.type)
        {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (_event.key.keysym.sym == SDLK_ESCAPE) {
                    isRunning = false;
                }
                break;
        }
    }
    return isRunning;
}

void View::draw(void) {
    _drawBackground();
    SDL_RenderPresent(_renderer);
}

void View::frameManagement(void) {
    // Calculate the time taken to render this frame
    Uint32 currentTime = SDL_GetTicks();
    Uint32 renderTime = currentTime - lastFrameTime;

    // Calculate required delay to maintain frame rate (rounded to nearest integer)
    const Uint32 targetFrameDelay = (1000 + FRAME_RATE / 2) / FRAME_RATE; // 17 ms for 60 FPS

    if (renderTime < targetFrameDelay) {
        Uint32 waitTime = targetFrameDelay - renderTime;
        SDL_Delay(waitTime);
    }

    // Update frame timing reference point
    lastFrameTime = SDL_GetTicks();
}

void View::_drawBackground(void){
    // Clear the renderer with a black color
    SDL_SetRenderDrawColor(_renderer, 166, 243, 249, 255);
    SDL_RenderClear(_renderer);
}
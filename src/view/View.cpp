#include <iostream>

#include "View.hpp"
#include "Const.hpp"

View::View(Controller * p_controller):
    _controller(p_controller),
    _window(nullptr),
    _renderer(nullptr),
    _event(),
    lastFrameTime(0),
    frameDelay(1000 / FRAME_RATE) {
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
    _drawThickLine(10, 10, 200, 200, 5, (SDL_Color){0, 0, 0, 255});
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

void View::_drawThickLine(float x1, float y1, float x2, float y2, float thickness, SDL_Color color) {
    // Calculate direction vector
    float dx = x2 - x1;
    float dy = y2 - y1;
    float length = sqrtf(dx * dx + dy * dy);

    // Handle zero-length lines
    if (length == 0) return;

    // Normalize direction
    dx /= length;
    dy /= length;
    
    // Calculate perpendicular vector (scaled by half-thickness)
    float px = -dy * thickness / 2;
    float py = dx * thickness / 2;
    
    // Calculate four corners of the thiccolork line
    SDL_FPoint points[4] = {
        { x1 + px, y1 + py }, // P1: top-left
        { x1 - px, y1 - py }, // P2: bottom-left
        { x2 - px, y2 - py }, // P3: bottom-right
        { x2 + px, y2 + py }  // P4: top-right
    };
    
    // Vertices for the quad (position + color)
    SDL_Vertex vertices[4];
    for (int i = 0; i < 4; ++i) {
        vertices[i] = (SDL_Vertex){
            .position = points[i],
            .color = color,
            .tex_coord = { 0, 0 }
        };
    }
    
    // Indices for two triangles (P1->P2->P3 and P1->P3->P4)
    const int indices[6] = { 0, 1, 2, 0, 2, 3 };
    
    // Render the thick line as a quad
    SDL_RenderGeometry(_renderer, NULL, vertices, 4, indices, 6);
}
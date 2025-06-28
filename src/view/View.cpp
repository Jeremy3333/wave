#include <iostream>
#include <list>

#include "View.hpp"
#include "Const.hpp"

View::View(Model & p_model):
    _Model(p_model),
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
                switch (_event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        isRunning = false;
                        break;
                    case SDLK_q:
                        _Model.addRotation((deltaTime / 1000) * M_PI/2);
                        break;
                    case SDLK_d:
                        _Model.addRotation(-(deltaTime / 1000) * M_PI/2);
                        break;
                    case SDLK_z:
                        _Model.addIsoAlpha((deltaTime / 1000) * M_PI/5);
                        break;
                    case SDLK_s:
                        _Model.addIsoAlpha(-(deltaTime / 1000) * M_PI/5);
                        break;
                }
                break;
        }
    }
    return isRunning;
}

void View::draw(void) {
    _drawBackground();
    _draw3DHexagon(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
    _fillCircle(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 2);
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

    // Update deltaTime for the next frame
    deltaTime = SDL_GetTicks() - lastFrameTime;

    // Update frame timing reference point
    lastFrameTime = SDL_GetTicks();
}

void View::_drawBackground(void){
    // Clear the renderer
    SDL_SetRenderDrawColor(_renderer, 15, 131, 247, 255);
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

void View::_fillCircle(float x, float y, float r) {
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = r;
    d = r -1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderDrawLine(_renderer, x - offsety, y + offsetx,
                                     x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(_renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(_renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(_renderer, x - offsety, y - offsetx,
                                     x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (r - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }
}

void View::_drawThickRoundLine(float x1, float y1, float x2, float y2, float thickness, SDL_Color color) {
    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
    _drawThickLine(x1, y1, x2, y2, thickness, color);
    _fillCircle(x1, y1 - 1, thickness / 2.0f);
    _fillCircle(x2, y2 - 1, thickness / 2.0f);
}

void View::_draw3DHexagon(float x, float y){
    const float alpha = _Model.getIsoAlpha();
    const float rotation = _Model.getRotation();
    constexpr int r = 30;
    const double h = r * 1.5 * (1 - alpha / M_PI_2);
    float x1 = r * std::cos(rotation) + x;
    float y1 = r * std::sin(rotation) * std::sin(alpha) + y;
    std::list<std::pair<float, float>> visible;
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    for (int i = 1; i <= 6; i++){
        float x2 = r * std::cos((i * M_PI / 3) + rotation) + x;
        float y2 = r * std::sin((i * M_PI / 3) + rotation) * std::sin(alpha) + y;
        if(_isFaceIsometricallyVisible(y1, y2, y)){
            _drawThickLine(x1, y1, x2, y2, 2,{255, 255, 255, 255});
            visible.push_front({x1, y1});
            visible.push_front({x2, y2});
            _drawThickLine(x1, y1 + h, x2, y2 + h, 2,{0, 0, 0, 255});
        }
        else
            _drawThickLine(x1, y1, x2, y2, 2,{0, 0, 0, 255});
        x1 = x2;
        y1 = y2;
    }
    visible.unique();
    visible.sort(View::_compareFirstOfPair);
    std::pair<float, float> borderLine = visible.front();
    visible.pop_front();
    _drawThickLine(borderLine.first, borderLine.second, borderLine.first, borderLine.second + h, 2,{0, 0, 0, 255});
    borderLine = visible.back();
    visible.pop_back();
    _drawThickLine(borderLine.first, borderLine.second, borderLine.first, borderLine.second + h, 2,{0, 0, 0, 255});
    for(auto e: visible){
        _drawThickLine(e.first, e.second, e.first, e.second + h, 2,{255, 255, 255, 255});
    }
}

bool View::_isFaceIsometricallyVisible(float y1, float y2, float y) {
    float faceY = (y1 + y2) / 2;
    return faceY >= y;
}

bool View::_compareFirstOfPair(std::pair<float, float>& first, std::pair<float, float>& second) {
    return first.first < second.first;
}
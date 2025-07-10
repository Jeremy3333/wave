#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

#include "View.hpp"
#include "ViewConstants.hpp"

View::View(Model & p_model):
    _Model(p_model),
    _window(nullptr),
    _renderer(nullptr),
    _event(),
    lastFrameTime(0),
    frameDelay(1000 / ViewConstants::FRAME_RATE) {
    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        throw std::runtime_error("SDL initialization failed");
    }

    // Create a window
    _window = SDL_CreateWindow(ViewConstants::WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ViewConstants::WINDOW_WIDTH, ViewConstants::WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

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
    bool shouldContinueRunning = true;
    while (SDL_PollEvent(&_event)) {
        switch (_event.type)
        {
            case SDL_QUIT:
                shouldContinueRunning = false;
                break;
            case SDL_KEYDOWN:
                shouldContinueRunning = _handleKeyPress(_event.key.keysym.sym, _deltaTime);
            default:
                break;
        }
    }
    return shouldContinueRunning;
}

void View::draw(void) {
    _drawBackground();
    _drawGrid(ViewConstants::WINDOW_WIDTH / 2, ViewConstants::WINDOW_HEIGHT / 2);
    SDL_RenderPresent(_renderer);
}

void View::frameManagement(void) {
    // Calculate the time taken to render this frame
    Uint32 currentTime = SDL_GetTicks();
    Uint32 renderTime = currentTime - lastFrameTime;

    // Calculate required delay to maintain frame rate (rounded to nearest integer)
    const Uint32 targetFrameDelay = (1000 + ViewConstants::FRAME_RATE / 2) / ViewConstants::FRAME_RATE; // 17 ms for 60 FPS

    if (renderTime < targetFrameDelay) {
        Uint32 waitTime = targetFrameDelay - renderTime;
        SDL_Delay(waitTime);
    }

    // Update deltaTime for the next frame
    _deltaTime = SDL_GetTicks() - lastFrameTime;

    // Update frame timing reference point
    lastFrameTime = SDL_GetTicks();
}



bool View::_handleKeyPress(SDL_Keycode keyCode, float deltaTime) {
    switch (keyCode) {
        case SDLK_ESCAPE:
            return false;
            break;
        case SDLK_q:
            _Model.addRotation((deltaTime / 1000.0f) * M_PI / 2.0f);
            break;
        case SDLK_d:
            _Model.addRotation(-(deltaTime / 1000.0f) * M_PI / 2.0f);
            break;
        case SDLK_z:
            _Model.addIsoAlpha((deltaTime / 1000.0f) * M_PI / 5.0f);
            break;
        case SDLK_s:
            _Model.addIsoAlpha(-(deltaTime / 1000.0f) * M_PI / 5.0f);
            break;
        case SDLK_r:
            _Model.addGridSize(1);
            break;
        case SDLK_f:
            _Model.addGridSize(-1);
            break;
        default:
            break;
    }
    return true;
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

void View::_draw3DHexagon(const float x, const float y, const float alpha, const float sinAlpha, const float rotation, const int radius) {
    // 1. Préparation des paramètres de base
    const float cosAlpha = std::cos(alpha);
    const float height = radius * 1.5f * cosAlpha;

    // 2. Calcul des sommets de l'hexagone supérieur
    float vertexX[6], vertexY[6];
    for (int i = 0; i < 6; i++) {
        const float angle = i * M_PI / 3 + rotation;
        vertexX[i] = radius * std::cos(angle) + x;
        vertexY[i] = radius * std::sin(angle) * sinAlpha + y;
    }

    // 3. Détection de visibilité des faces latérales
    bool faceVisible[6];
    for (int i = 0; i < 6; i++) {
        const int next_i = (i + 1) % 6;
        faceVisible[i] = _isFaceIsometricallyVisible(vertexY[i], vertexY[next_i], y);
    }

    // 4. Dessin des faces latérales (premier plan arrière)
    for (int i = 0; i < 6; i++) {
        if (!faceVisible[i]) continue;

        const int next_i = (i + 1) % 6;
        const float ratio = ((vertexX[i] + vertexX[next_i]) / 2 - (x - radius)) / (radius * 2);

        const SDL_Color faceColor = {
            0,
            static_cast<Uint8>(150 + ratio * 100),
            static_cast<Uint8>(112 + ratio * 76),
            255
        };

        const SDL_Vertex faceVertices[4] = {
            {{vertexX[i], vertexY[i]}, faceColor, {0,0}},
            {{vertexX[i], vertexY[i] + height}, faceColor, {0,0}},
            {{vertexX[next_i], vertexY[next_i]}, faceColor, {0,0}},
            {{vertexX[next_i], vertexY[next_i] + height}, faceColor, {0,0}}
        };

        constexpr int indices[] = {0, 1, 2, 1, 2, 3};
        SDL_RenderGeometry(_renderer, nullptr, faceVertices, 4, indices, 6);
    }

    // 5. Dessin de la face supérieure (plan intermédiaire)
    SDL_Vertex topVertices[6];
    for (int i = 0; i < 6; i++) {
        topVertices[i] = {{vertexX[i], vertexY[i]}, {0, 200, 150, 255}, {0,0}};
    }
    constexpr int topIndices[] = {0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5};
    SDL_RenderGeometry(_renderer, nullptr, topVertices, 6, topIndices, 12);

    // 6. Dessin des arêtes SUPÉRIEURES (contour de la face du haut)
    for (int i = 0; i < 6; i++) {
        const int next_i = (i + 1) % 6;
        const bool visible = faceVisible[i];
        _drawThickLine(
            vertexX[i], vertexY[i],
            vertexX[next_i], vertexY[next_i],
            1,
            visible ? SDL_Color{230, 230, 230, 255} : SDL_Color{0, 0, 0, 255}
        );
    }

    // 7. Dessin des lignes VERTICALES (qui descendent des sommets)
    std::vector<std::pair<float, float>> verticalPoints;
    for (int i = 0; i < 6; i++) {
        // Sélectionne uniquement les points connectés à une face visible
        if (faceVisible[i] || faceVisible[(i + 5) % 6]) {
            verticalPoints.emplace_back(vertexX[i], vertexY[i]);
        }
    }

    // Trie et supprime les doublons
    std::sort(verticalPoints.begin(), verticalPoints.end());
    auto last = std::unique(verticalPoints.begin(), verticalPoints.end());
    verticalPoints.erase(last, verticalPoints.end());

    // Dessin des lignes verticales avec les bords en noir et l'intérieur en blanc
    if (!verticalPoints.empty()) {
        const float minX = verticalPoints.front().first;
        const float maxX = verticalPoints.back().first;

        for (const auto& point : verticalPoints) {
            const bool isEdge = (point.first == minX) || (point.first == maxX);
            _drawThickLine(
                point.first, point.second,
                point.first, point.second + height,
                1,
                isEdge ? SDL_Color{0, 0, 0, 255} : SDL_Color{255, 255, 255, 255}
            );
        }
    }

    // 8. Dessin des arêtes INFÉRIEURES (dernier plan, premier plan)
    for (int i = 0; i < 6; i++) {
        const int next_i = (i + 1) % 6;
        const bool visible = faceVisible[i];

        // Ne dessine que les arêtes des faces visibles
        if (visible) {
            _drawThickLine(
                vertexX[i], vertexY[i] + height,
                vertexX[next_i], vertexY[next_i] + height,
                1,
                {0, 0, 0, 255}
            );
        }
    }
}

void View::_drawGrid(const float x, const float y) {
    // 1. prépare les variable
    const float alpha = _Model.getIsoAlpha();
    const float rotation = _Model.getRotation();
    const int gridSize = _Model.getGridSize();
    constexpr int hexRadius = 30;
    std::vector<std::pair<int, int>> hexagones;

    // 2. précalculation
    const float sinAlpha = std::sin(alpha);
    const int gridRadius =std::sqrt(3) * hexRadius;

    // 3. Rajoute l'hexagone central
    hexagones.push_back({x, y});

    // 4. calcule le reste de la grille
    for(int i = 0; i < gridSize; i++) {
        // séparee en 6 ligne
        std::pair<int, int> lastCoor = {(gridRadius * (i + 1)) * std::cos(5 * M_PI / 3 + rotation + M_PI / 6) + x, (gridRadius * (i + 1)) * std::sin(5 * M_PI / 3 + rotation + M_PI / 6) * sinAlpha + y};
        for(int j = 0; j < 6; j++) {
            const float angle = j * M_PI / 3 + rotation + M_PI / 6;
            std::pair<int, int> currCoor = {(gridRadius * (i + 1)) * std::cos(angle) + x, (gridRadius * (i + 1)) * std::sin(angle) * sinAlpha + y};
            hexagones.push_back(currCoor);
            // longeur entre les deux points
            float distance = std::sqrt(std::pow(currCoor.first - lastCoor.first, 2) + std::pow(currCoor.second - lastCoor.second, 2));
            // la normale du vecteur
            float normalX = (currCoor.first - lastCoor.first) / distance;
            float normalY = (currCoor.second - lastCoor.second) / distance;
            // ajoute i hexagone entre les deux points
            for(int k = 1; k < i + 1; k++) {
                std::pair<int, int> newCoor = {static_cast<int>(lastCoor.first + k * normalX * distance / (i + 1)),
                                               static_cast<int>(lastCoor.second + k * normalY * distance / (i + 1))};
                hexagones.push_back(newCoor);
            }
            lastCoor = currCoor;
        }
    }

    std::sort(hexagones.begin(), hexagones.end(), _compareSecondOfPair);

    for(auto hexagone : hexagones){
        _draw3DHexagon(hexagone.first, hexagone.second, alpha, sinAlpha, rotation, hexRadius);
    }
}

bool View::_isFaceIsometricallyVisible(float y1, float y2, float y) {
    float faceY = (y1 + y2) / 2;
    return faceY >= y;
}

bool View::_compareSecondOfPair(std::pair<int, int>& first, std::pair<int, int>& second) {
    return first.second < second.second;
}
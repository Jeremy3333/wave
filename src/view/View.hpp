#pragma once
#include <SDL2/SDL.h>

#include "Model.hpp"

/**
 * View class for handling user input and rendering.
 */
class View {
public:
    /**
     * Constructor for the View class.
     * @param p_model Pointer to the Model instance.
     * Initializes SDL and creates a window.
     * @throws std::runtime_error if SDL initialization or window creation fails.
     */
    View(Model & p_model);
    /**
     * Destructor for the View class.
     * Cleans up SDL resources.
     */
    ~View();

    /**
     * Handle user input.
     * @return true if the program should continue running, false if it should exit.
     */
    bool input(void);

    /**
     * Render the view.
     * This function should be implemented to draw the current state of the game.
     */
    void draw(void);

    /**
     * Manage the frame rate.
     */
    void frameManagement(void);
private:
    /**
     * Pointer to the Controller instance.
     */
    Model & _Model;

    /**
     * Pointer to the SDL window.
     */
    SDL_Window * _window;
    /**
     * Pointer to the SDL renderer.
     */
    SDL_Renderer * _renderer;
    /**
     * SDL event structure for handling events.
     */
    SDL_Event _event;

    /**
     * Last frame time in milliseconds.
     */
    Uint32 lastFrameTime;
    /**
     * Frame delay in milliseconds to control the frame rate.
     */
    Uint32 frameDelay;

    /**
     * time of the last frame in milliseconds.
     */
    float deltaTime;

    /**
     * Draw the background.
     */
    void _drawBackground(void);

    /**
     * Draw a thick line between two points.
     * @param x1 The x-coordinate of the first point.
     * @param y1 The y-coordinate of the first point.
     * @param x2 The x-coordinate of the second point.
     * @param y2 The y-coordinate of the second point.
     * @param thickness The thickness of the line.
     * @param color The color of the line. (mandatory because of SDL_RenderGeometry)
     */
    void _drawThickLine(float x1, float y1, float x2, float y2, float thickness, SDL_Color color);

    /**
     * Fill a circle.
     * @param x The x-coordinate of the center of the circle.
     * @param y The y-coordinate of the center of the circle.
     * @param r The radius of the circle.
     */
    void _fillCircle(float x, float y, float r);

    /**
     * Draw a thick round line between two points.
     * @param x1 The x-coordinate of the first point.
     * @param y1 The y-coordinate of the first point.
     * @param x2 The x-coordinate of the second point.
     * @param y2 The y-coordinate of the second point.
     * @param thickness The thickness of the line.
     * @param color The color of the line. (mandatory because of SDL_RenderGeometry)
     */
    void _drawThickRoundLine(float x1, float y1, float x2, float y2, float thickness, SDL_Color color);

    void _draw3DHexagon(const float x, const float y, const float alpha, const float sinAlpha, const float rotation, const int radius);

    /**
     * @brief Draw a grid at the specified coordinates.
     *
     * @param x The x-coordinate of the center of the grid.
     * @param y The y-coordinate of the center of the grid.
     */
    void _drawGrid(const float x, const float y);

    /**
     * @brief true if the face y1, y2 of the object place at y is visible
     *
     * @param y1 height of the first side of the face
     * @param y2 height of the second side of the face
     * @param y height if the centre of the object
     * @return true the face is visible
     * @return false the face is hide
     */
    bool _isFaceIsometricallyVisible(float y1, float y2, float y);

    /**
     * @brief Compare the second element of two pairs.
     *
     * @param first The first pair to compare.
     * @param second The second pair to compare.
     * @return true if the second element of the first pair is less than the second element of the second pair.
     * @return false otherwise.
     */
    static bool _compareSecondOfPair(std::pair<int, int>& first, std::pair<int, int>& second);
};
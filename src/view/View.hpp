#pragma once
#include <SDL2/SDL.h>

/**
 * pre-declaration of Controller class to avoid circular dependency.
 */
class Controller;

/**
 * View class for handling user input and rendering.
 */
class View {
public:
    /**
     * Constructor for the View class.
     * @param p_controller Pointer to the Controller instance.
     * Initializes SDL and creates a window.
     * @throws std::runtime_error if SDL initialization or window creation fails.
     */
    View(Controller * p_controller);
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
    Controller * _controller;
    
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

    void _drawBackground(void);
};
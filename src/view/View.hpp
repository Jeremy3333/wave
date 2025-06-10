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
     * SDL event structure for handling events.
     */
    SDL_Event _event;
};
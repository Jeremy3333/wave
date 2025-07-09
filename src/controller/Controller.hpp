#pragma once
#include "Model.hpp"
#include "View.hpp"

/**
 * Controller class for managing the interaction between the Model and View
 * and handling the main application loop.
 */
class Controller
{
public:
    /**
     * Constructor for the Controller class.
     * Initializes the Model and View, and starts the main loop.
     * @throws std::runtime_error if the View initialization fails.
     */
    Controller();
private:
    /**
     * Model instance
     */
    Model _model;
    /**
     * View instance
     */
    View _view;
    /**
     * Flag to indicate if the application is running.
     */
    bool _isRunning;

    /**
     * Main loop of the application.
     * Handles user input and updates the model and view.
     */
    void _mainLoop();
};
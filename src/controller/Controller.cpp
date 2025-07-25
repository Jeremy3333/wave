#include "Controller.hpp"

Controller::Controller() : _model(), _view(_model), _isRunning(true) {
    _mainLoop();
}

void Controller::_mainLoop() {
    // Main loop of the application
    while (_isRunning)
    {
        _isRunning = _view.input();
        _view.draw();
        _view.frameManagement();
    }
}
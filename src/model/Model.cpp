#include <cmath>
#include <algorithm>

#include "Model.hpp"

Model::Model():_isoAlpha(M_PI / 4), _rotation(0), _gridSize(0) {}

void Model::addIsoAlpha(float p_v) {
    _isoAlpha = std::clamp(_isoAlpha + p_v, 0.05f, static_cast<float>(M_PI / 2));
}

void Model::addRotation(float p_v) {
    _rotation = std::fmod(_rotation + p_v, static_cast<float>(2 * M_PI));
    if (_rotation < 0.0f) {
        _rotation += static_cast<float>(2 * M_PI);
    }
}

void Model::addGridSize(int p_v) {
    _gridSize = std::clamp(_gridSize + p_v, 0, 5);
}

float Model::getIsoAlpha(void) const {
    return _isoAlpha;
}

float Model::getRotation(void) const {
    return _rotation;
}

int Model::getGridSize(void) const {
    return _gridSize;
}
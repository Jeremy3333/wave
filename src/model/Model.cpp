#include <cmath>
#include <algorithm>
#include <stdexcept>

#include "Model.hpp"
#include "ModelConstants.hpp"

Model::Model():_isoAlphaAngle(M_PI / 4), _rotationAngle(0), _gridSize(0) {}

void Model::addIsoAlpha(float updateIsoAlpha) {
    if(updateIsoAlpha < -ModelConstants::kMaxIsoAlphaAngle || updateIsoAlpha > ModelConstants::kMaxIsoAlphaAngle)
        throw std::invalid_argument("updateIsoAlpha must be between -pi/2 and pi/2");
    _isoAlphaAngle = std::clamp(_isoAlphaAngle + updateIsoAlpha, ModelConstants::kMinIsoAlphaAngle, ModelConstants::kMaxIsoAlphaAngle);
}

void Model::addRotation(float updateRotationAngle) {
    if(updateRotationAngle < -ModelConstants::kMaxRotationAngle || updateRotationAngle > ModelConstants::kMaxRotationAngle)
        throw std::invalid_argument("updateRotationAngle must be between -2pi and 2pi");
    _rotationAngle = std::fmod(_rotationAngle + updateRotationAngle, ModelConstants::kMaxRotationAngle);
    if (_rotationAngle < ModelConstants::kMinRotationAngle) {
        _rotationAngle += ModelConstants::kMaxRotationAngle;
    }
}

void Model::addGridSize(int updateGridSize) {
    if(updateGridSize < -ModelConstants::kMaxGridSize || updateGridSize > ModelConstants::kMaxGridSize)
        throw std::invalid_argument("updateGridSize must be between -5 and 5");
    _gridSize = std::clamp(_gridSize + updateGridSize, ModelConstants::kMinGridSize, ModelConstants::kMaxGridSize);
}

float Model::getIsoAlpha(void) const {
    return _isoAlphaAngle;
}

float Model::getRotation(void) const {
    return _rotationAngle;
}

int Model::getGridSize(void) const {
    return _gridSize;
}
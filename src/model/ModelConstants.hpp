#pragma once

namespace ModelConstants {
    constexpr float kMinIsoAlphaAngle = 0.05f;
    constexpr float kMaxIsoAlphaAngle = static_cast<float>(M_PI / 2);
    constexpr float kMinRotationAngle = 0.0f;
    constexpr float kMaxRotationAngle = static_cast<float>(2 * M_PI);
    constexpr int kMinGridSize = 0;
    constexpr int kMaxGridSize = 5;
}
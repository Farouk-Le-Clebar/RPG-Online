#pragma once

#include "Component.hpp"

struct Velocity : public Component {
    float x = 0.0f;
    float y = 0.0f;

    Velocity() = default;
    Velocity(float x, float y) : x(x), y(y) {}
};

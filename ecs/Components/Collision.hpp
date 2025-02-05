#pragma once

#include "Component.hpp"

struct Collision : public Component {
    bool isColliding = false;

    Collision() = default;
    Collision(bool isColliding) : isColliding(isColliding) {}
};

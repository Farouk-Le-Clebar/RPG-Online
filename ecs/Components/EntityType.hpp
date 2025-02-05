#pragma once

#include "Component.hpp"

enum class Entities { PLAYER, ENEMY, BULLET, WALL, POWERUP, NONE };

struct EntityType : public Component {
  Entities type;

  EntityType() = default;
  EntityType(Entities type) : type(type) {}
};
#pragma once

#include "Components/Collision.hpp"
#include "Components/Component.hpp"
#include "Components/EntityType.hpp"
#include "Components/Position.hpp"
#include "Components/Velocity.hpp"

class ECS {
public:

  template <typename T> void registerComponent() {
    auto type = std::type_index(typeid(T));
    if (components.find(type) == components.end()) {
      components[type] = std::make_shared<ComponentContainer<T>>();
    }
  }

  template <typename T> void addComponent(Entity entity, T component) {
    auto type = std::type_index(typeid(T));
    auto container =
        std::static_pointer_cast<ComponentContainer<T>>(components[type]);
    container->data[entity] = component;
  }

  template <typename T> T *getComponent(Entity entity) {
    auto type = std::type_index(typeid(T));
    auto container =
        std::static_pointer_cast<ComponentContainer<T>>(components[type]);
    if (container->data.find(entity) != container->data.end()) {
      return &container->data[entity];
    }
    return nullptr;
  }

  Entity createEntity(int id = -1);

  std::vector<Entity> getEntityByType(Entities type);

private:
  std::unordered_map<std::type_index, std::shared_ptr<void>> components;
  std::vector<Entity> entities;
};

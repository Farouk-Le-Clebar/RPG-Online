#include "ECS.hpp"

Entity ECS::createEntity(int id) {
  if (id == -1) {
    entities.push_back(entities.size());
    return entities.size() - 1;
  } else {
    entities.push_back(id);
    return id;
  }
}

std::vector<Entity> ECS::getEntityByType(Entities type) {
  std::vector<Entity> result;
  auto typeIndex = std::type_index(typeid(EntityType));
  auto container =
      std::static_pointer_cast<ComponentContainer<EntityType>>(components[typeIndex]);
  for (auto &entity : container->data) {
    if (entity.second.type == type) {
      result.push_back(entity.first);
    }
  }
  return result;
}

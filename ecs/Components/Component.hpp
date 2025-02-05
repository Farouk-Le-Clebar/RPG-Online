#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <vector>

using Entity = std::size_t;

struct Component {
    virtual ~Component() = default;
};

template <typename T>
struct ComponentContainer {
    std::unordered_map<Entity, T> data;
};

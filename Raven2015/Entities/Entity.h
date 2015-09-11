#pragma once

#include "../entityx/Entity.h"

namespace ex = entityx;

namespace Raven {

    class Entity : public ex::Entity {
    public:
        Entity() {

        }

        Entity(const ex::Entity &other) {
            *this = other;
        }

        Entity &operator =(const ex::Entity &other) {
            id_ = other.id();
            manager_ = other.manager();
            return *this;
        }
    };

}
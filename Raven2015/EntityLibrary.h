#pragma once

#include "Common.h"
#include "entityx\Entity.h"
#include "ComponentLibrary.h"

namespace ex = entityx;

namespace Raven {

    /*
     * Base class for creating pre-defined entities.
     * Will later try to adopt Unity-like Prefab architecture if there's time.
     */
    class Entity : public ex::Entity {
    public:
        Entity() {}

        Entity(ex::Entity &entity) {
            *this = entity;
        }

        Entity &operator=(const ex::Entity &entity) {
            id_ = entity.id();
            manager_ = entity.manager();
        }

    };



}

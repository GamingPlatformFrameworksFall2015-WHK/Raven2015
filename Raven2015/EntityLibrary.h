#pragma once
#include "Common.h"
#include "ComponentLibrary.h"

namespace Raven {

    struct EntityLibrary {

        // Turns the first entity into a replica of the second entity before returning the first entity.
        // Simply combines the use of clearEntity and copyComponents with the full set of template parameters.
        static ex::Entity copyEntity(ex::Entity toReturn, ex::Entity toCopy);

        // Clears the last component from an entity. Base case for the parameter pack version to work recursively
        template <typename T>
        static void clearEntity(ex::Entity e, T last);

        // Clears all components from the given entity
        // Usage:
        // /* Entity e1 with Data, Transform, and Rigidbody */
        // clearEntity<Data, Transform, Rigibody>(e1); // OR
        // clearEntity<Data, Transform, Rigibody, BoxCollider, etc.>(e1);
        template <typename T, typename... Args>
        static void clearEntity(ex::Entity e, T first, Args... args);

        // Copies the last component from one entity to another. Base case for the parameter pack version to work recursively
        template <typename T>
        static ex::Entity copyEntityComponents(ex::Entity toReturn, ex::Entity toCopy, T last);

        // Copies all possible components from the given toCopy entity into the toReturn entity
        // By the end, the following 4 conditions will be true:
        // 1. If toReturn has a component that toCopy has, toReturn's version will be replaced with a copy of toCopy's
        // 2. If toReturn does not have a component that toCopy has, it will not have a copy of toCopy's
        // 3. Any components that toReturn has that toCopy doesn't will remain untouched
        // 4. Component pointer members will have deep copies made
        // Usage:
        // /* Entity e1 with some set of components, Entity e2 with some different set of components */
        // copyComponents<Data, Transform, etc.>(e1, e2, *e2.component<Data>().get(), *e2.component<Transform>().get(), etc.); // OR
        // copyComponents<COMPONENT_TYPE_LIST>(e1, e2, COMPONENTS_OF_ENTITY(toCopy); // captures all possible components
        template <typename T, typename... Args>
        static ex::Entity copyEntityComponents(ex::Entity toReturn, ex::Entity toCopy, T first, Args... args);

        struct Create {

            static ex::Entity Entity(std::string entityName = "Default Entity") {
                ex::Entity e = cmn::entities->create();
                e.assign<Data>()->name = entityName + " " + std::to_string(counter++);
                e.assign<Transform>();
                e.assign<Rigidbody>();
                return e;
            }

            static ex::Entity Player(std::string playerName = "Default Player") {
                ex::Entity e = Entity(playerName);
                e.assign<Pawn>();
                e.assign<BoxCollider>();
                e.assign<Renderer>();
                // Assign a RenderableSprite to the Player character here
            }

        private:
            static unsigned int counter;

        };
    };
}
#include "EntityLibrary.h"

namespace Raven {

    unsigned int counter = 0;

    ex::Entity EntityLibrary::copyEntity(ex::Entity toReturn, ex::Entity toCopy) {
        clearEntity<COMPONENT_TYPE_LIST>(toReturn, COMPONENTS_OF_ENTITY(toReturn));
        return copyEntityComponents<COMPONENT_TYPE_LIST>(toReturn, toCopy, COMPONENTS_OF_ENTITY(toCopy));
    }

    template <typename T>
    void EntityLibrary::clearEntity(ex::Entity e, T last) {
        if (e.has_component<T>()) {
            e.remove<T>();
        }
    }
    template <typename T, typename... Args>
    void EntityLibrary::clearEntity(ex::Entity e, T first, Args... args) {
        if (e.has_component<T>()) {
            e.remove<T>();
        }
        clearEntity(e, args...);
    }
    template <typename T>
    ex::Entity EntityLibrary::copyEntityComponents(ex::Entity toReturn, ex::Entity toCopy, T last) {
        if (toCopy.has_component<T>()) {
            toReturn.assign_from_copy<T>(*toCopy.component<T>().get());
        }
        return toReturn;
    }
    template <typename T, typename... Args>
    ex::Entity EntityLibrary::copyEntityComponents(ex::Entity toReturn, ex::Entity toCopy, T first, Args... args) {
        if (toCopy.has_component<T>()) {
            toReturn.assign_from_copy<T>(*toCopy.component<T>().get());
        }
        return copyEntityComponents(toReturn, toCopy, args...);
    }
}
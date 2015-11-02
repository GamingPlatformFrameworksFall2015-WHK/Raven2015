#include "EntityLibrary.h"

namespace Raven {

    unsigned int EntityLibrary::Create::counter = 0;

    ex::Entity EntityLibrary::copyEntity(ex::Entity toReturn, ex::Entity toCopy) {
        clearEntity<COMPONENT_TYPE_LIST>(toReturn, COMPONENT_TYPES(::getNullPtrToType()));
        return copyEntityComponents<COMPONENT_TYPE_LIST>(toReturn, toCopy, COMPONENT_TYPES(::getNullPtrToType()));
    }

    template <typename C>
    void EntityLibrary::clearEntity(ex::Entity e, C* c) {
        if (e.has_component<C>()) {
            e.remove<C>();
        }
    }

    template <typename C, typename... Components>
    void EntityLibrary::clearEntity(ex::Entity e, C* c, Components*... components) {
        if (e.has_component<C>()) {
            e.remove<C>();
        }
        clearEntity<Components...>(e, components...);
    }

    void EntityLibrary::clearCoreComponents(ex::Entity e) {
        e.remove<Data>();
        e.remove<Transform>();
        e.remove<Rigidbody>();
    }

    template <typename C>
    ex::Entity EntityLibrary::copyEntityComponents(ex::Entity toReturn, ex::Entity toCopy, C* c) {
        if (toCopy.has_component<C>()) {
            toReturn.assign_from_copy<C>(*toCopy.component<C>().get());
        }
        return toReturn;
    }

    template <typename C, typename... Components>
    ex::Entity EntityLibrary::copyEntityComponents(ex::Entity toReturn, ex::Entity toCopy, C* c, Components*... components) {
        if (toCopy.has_component<C>()) {
            toReturn.assign_from_copy<C>(*toCopy.component<C>().get());
        }
        return copyEntityComponents<Components...>(toReturn, toCopy, components...);
    }
}
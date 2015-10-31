/* 
 * Classname:   Gaming Platform Frameworks
 * Project:     Raven
 * Version:     1.0
 *
 * Copyright:   The contents of this document are the property of its creators.
 *              Reproduction or usage of it without permission is prohibited.
 *
 * Owners:      Will Nations
 *              Hailee Ammons
 *              Kevin Wang
 */
#pragma once 

#include "Common.h"         // For Common::EAudioType
#include "entityx\Event.h"  // For entityx::Event
#include "entityx\Entity.h" // For entityx::Entity
#include "SFML/System.hpp"
#include "ComponentLibrary.h"
#include "SFGUI/Widgets.hpp"

using namespace sfg;

namespace Raven {

#pragma region AudioEvents

    struct AudioEvent : public ex::Event<AudioEvent> {

        AudioEvent(std::string audioFileName = "", AudioMaker *maker = nullptr,
            cmn::EAudioType type = cmn::EAudioType::NO_TYPE, cmn::EAudioOperation operation = cmn::EAudioOperation::NO_AUDIO_OPERATION,
            cmn::EAudioLoop loop = cmn::EAudioLoop::LOOP_FALSE) : audioFileName(audioFileName), maker(maker), type(type), operation(operation),
            loop(loop) {}

        std::string audioFileName;

        AudioMaker *maker;

        cmn::EAudioType type;

        cmn::EAudioOperation operation;

        cmn::EAudioLoop loop;
    };

#pragma endregion //AudioEvents

#pragma region CollisionEvents

    /*
     * An event that stores the identities of two colliding entities.
     */
    struct CollisionEvent : public ex::Event<CollisionEvent> {

        CollisionEvent() : events(nullptr) {

        }

        /*
         * Custom constructor. Accepts two entities assumed to be colliding,
         * their point of impact, and a reference to the EventManager in case
         * response events need to be emitted.
         */
        CollisionEvent(ex::Entity leftEntity,
            ex::Entity rightEntity,
            sf::Vector2f collisionPoint,
            ex::EventManager *events = nullptr)
            : leftEntity(leftEntity), rightEntity(rightEntity),
            collisionPoint(collisionPoint), events(events) {

            leftTransform = leftEntity.component<Transform>();
            leftRigidbody = leftEntity.component<Rigidbody>();
            leftBoxCollider = leftEntity.component <BoxCollider>();
            rightTransform = rightEntity.component<Transform>();
            rightRigidbody = rightEntity.component<Rigidbody>();
            rightBoxCollider = rightEntity.component <BoxCollider>();
        }

        // The transform of the "left" entity in the collision.
        ex::ComponentHandle<Transform> leftTransform;

        // The rigidbody of the "left" entity in the collision.
        ex::ComponentHandle<Rigidbody> leftRigidbody;

        // The boxcollider of the "left" entity in the collision.
        ex::ComponentHandle<BoxCollider> leftBoxCollider;

        // The transform of the "right" entity in the collision.
        ex::ComponentHandle<Transform> rightTransform;

        // The rigidbody of the "right" entity in the collision.
        ex::ComponentHandle<Rigidbody> rightRigidbody;

        // The boxcollider of the "right" entity in the collision.
        ex::ComponentHandle<BoxCollider> rightBoxCollider;

        // The colliding left entity
        ex::Entity leftEntity;

        // The colliding right entity
        ex::Entity rightEntity;

        // The point of impact between the two colliding entities.
        sf::Vector2f collisionPoint;

        ex::EventManager *events;
    };

#pragma endregion

#pragma region InputEvents

    struct InputEvent : public ex::Event<InputEvent> {

        InputEvent(std::string action = "") :
            action(action) {}

        std::string action;
    };

    struct KeyboardEvent : public InputEvent {
        KeyboardEvent(std::string action = "")
            : InputEvent(action) {}
    };

#pragma endregion

#pragma region TimerEvents

    struct TimerEvent : public ex::Event<TimerEvent> {

        TimerEvent(
            std::string timerName = "", cmn::ETimerOperation op =
            cmn::ETimerOperation::NO_TIMER_OPERATION, ex::TimeDelta = 0.0)
            : timerName(timerName), timerOperation(op), scanTime(scanTime) {}

        ex::TimeDelta scanTime;
        cmn::ETimerOperation timerOperation;
        std::string timerName;
    };

#pragma endregion

#pragma region GUIEvents

    // The PanelType is not explicitly used, but because it is there, a unique version of the function can be made for
    // A given typename of panel. All panel typenames are detailed at the top of WidgetLibrary.h
    template <typename PanelType, typename... Widgets>
    struct GUIWidgetListEvent : public ex::Event<GUIWidgetListEvent<PanelType, Widgets...>> {

        enum Operation {
            POPULATE,
            ADD,
            REMOVE
        };

        GUIWidgetListEvent(Box::Ptr box = nullptr, void (*listItemFormatter)(Box::Ptr) = nullptr, 
            Operation op = Operation::ADD, std::string itemName = "") : 
            box(box), formatter(listItemFormatter), itemName(itemName), op(op) {}

        Box::Ptr box;
        std::string itemName;
        void(*formatter)(Box::Ptr);
        Operation op;
        
    };

    struct GUIDisplayParticularComponent : public ex::Event<GUIDisplayParticularComponent> {

        // Required for compilation
        GUIDisplayParticularComponent() {}

        GUIDisplayParticularComponent(ex::Entity entity, ComponentType type) :
            entity(entity), type(type) {}

        ex::Entity entity;
        ComponentType type;
    };

#pragma endregion

#pragma region XMLEvents

    struct XMLLoadEvent : public ex::Event<XMLLoadEvent> {
        XMLLoadEvent() {}
    };

    struct XMLSaveEvent : public ex::Event<XMLSaveEvent> {
        XMLSaveEvent() {}
    };

    struct XMLUpdateEntityNameEvent : public ex::Event<XMLUpdateEntityNameEvent> {

        XMLUpdateEntityNameEvent(ex::Entity entity, std::string newName, bool isPrefab) : 
            entity(entity), newName(newName), isPrefab(isPrefab) {}

        ex::Entity entity;

        std::string newName;

        bool isPrefab;
    };

    /*
    template <typename RenderableAsset>
    struct XMLDeserializeRendererAsset : public ex::Event<XMLDeserializeRendererAsset<RenderableAsset>> {

        XMLDeserializeRendererAsset(std::string& assetName, Renderer* renderer) :
            assetName(assetName), renderer(renderer) {}

        ex::ComponentHandle<Renderer> renderer;
        std::string assetName;
    };
    */

    struct XMLSerializeParticularComponent : public ex::Event<XMLSerializeParticularComponent> {

        XMLSerializeParticularComponent(ex::Entity entity, ComponentType type) :
            entity(entity), type(type) {}

        ex::Entity entity;
        ComponentType type;
    };

    struct XMLDeserializeParticularComponent : public ex::Event<XMLSerializeParticularComponent> {

        // Required for compilation
        XMLDeserializeParticularComponent() {}

        XMLDeserializeParticularComponent(ex::Entity entity, ComponentType type, bool forDisplay) :
            entity(entity), type(type), forDisplay(forDisplay) {}

        ex::Entity entity;
        ComponentType type;
        bool forDisplay;
    };

#pragma endregion


}
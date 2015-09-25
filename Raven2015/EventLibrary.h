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

namespace Raven {

#pragma region AudioEvents

    struct AudioEvent : public ex::Event<AudioEvent> {

        AudioEvent(std::string audioFileName = "", AudioMaker *maker = nullptr,
            cmn::EAudioType type = cmn::EAudioType::NO_TYPE, cmn::EAudioOperation operation = cmn::EAudioOperation::NO_OPERATION,
            cmn::EAudioLoop loop = cmn::EAudioLoop::FALSE) : audioFileName(audioFileName), maker(maker), type(type), operation(operation),
            loop(loop) {}

        std::string audioFileName;

<<<<<<< HEAD
        std::shared_ptr<MusicMaker> maker;
    };

    struct SoundEvent : public ex::Event<SoundEvent> {
=======
        AudioMaker *maker;
>>>>>>> master

        cmn::EAudioType type;

        cmn::EAudioOperation operation;

<<<<<<< HEAD
        std::shared_ptr<SoundMaker> maker;
=======
        cmn::EAudioLoop loop;
>>>>>>> master
    };

#pragma endregion

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

#pragma region TimerEvents

    struct TimerEvent : public ex::Event<TimerEvent> {

        TimerEvent(std::shared_ptr<TimeTable> timeTable = nullptr ,
            std:: string timerName = "", cmn::ETimerOperation op = 
            cmn::ETimerOperation::NO_OPERATION, ex::TimeDelta scanTime = 0.0) 
            : timeTable(timeTable), timerName(timerName), timerOperation(op), scanTime(scanTime){}

        ex::TimeDelta scanTime;
        cmn::ETimerOperation timerOperation;
        std::string timerName;
        std::shared_ptr<TimeTable> timeTable;
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

#pragma endregion //InputEvents

<<<<<<< HEAD
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

#pragma endregion //InputEvents
}
=======
}
>>>>>>> master

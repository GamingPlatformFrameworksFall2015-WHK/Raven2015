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

    struct MusicEvent : public ex::Event<MusicEvent> {

        MusicEvent(std::string audioFileName = "", MusicMaker *maker = nullptr) :
            audioFileName(audioFileName), maker(maker) {}

        std::string audioFileName;

        std::unique_ptr<MusicMaker> maker;
    };

    struct SoundEvent : public ex::Event<SoundEvent> {

        SoundEvent(std::string audioFileName = "", SoundMaker *maker = nullptr) :
            audioFileName(audioFileName), maker(maker) {}

        std::string audioFileName;

        std::unique_ptr<SoundMaker> maker;
    };

    struct MusicLoadEvent : public MusicEvent {
        MusicLoadEvent(std::string audioFileName = "", MusicMaker *maker = nullptr)
            : MusicEvent(audioFileName, maker) {}
    };
    struct MusicUnloadEvent : public MusicEvent {
        MusicUnloadEvent(std::string audioFileName = "", MusicMaker *maker = nullptr)
            : MusicEvent(audioFileName, maker) {}
    };
    struct MusicPlayEvent : public MusicEvent {
        MusicPlayEvent(std::string audioFileName = "", MusicMaker *maker = nullptr)
            : MusicEvent(audioFileName, maker) {}
    };
    struct MusicPauseEvent : public MusicEvent {
        MusicPauseEvent(std::string audioFileName = "", MusicMaker *maker = nullptr)
            : MusicEvent(audioFileName, maker) {}
    };
    struct MusicStopEvent : public MusicEvent {
        MusicStopEvent(std::string audioFileName = "", MusicMaker *maker = nullptr)
            : MusicEvent(audioFileName, maker) {}
    };
    struct MusicCustomEvent : public MusicEvent {
        MusicCustomEvent(void(*func) (std::string audioFileName, MusicMaker *maker)) :
            func(func) {}

        // Function pointer. Plugin a C++11 lambda function for custom behavior
        void(*func) (std::string audioFileName, MusicMaker *maker);
    };

    struct SoundLoadEvent : public SoundEvent {
        SoundLoadEvent(std::string audioFileName = "", SoundMaker *maker = nullptr)
            : SoundEvent(audioFileName, maker) {}
    };
    struct SoundUnloadEvent : public SoundEvent {
        SoundUnloadEvent(std::string audioFileName = "", SoundMaker *maker = nullptr)
            : SoundEvent(audioFileName, maker) {}
    };
    struct SoundPlayEvent : public SoundEvent {
        SoundPlayEvent(std::string audioFileName = "", SoundMaker *maker = nullptr)
            : SoundEvent(audioFileName, maker) {}
    };
    struct SoundPauseEvent : public SoundEvent {
        SoundPauseEvent(std::string audioFileName = "", SoundMaker *maker = nullptr)
            : SoundEvent(audioFileName, maker) {}
    };
    struct SoundStopEvent : public SoundEvent {
        SoundStopEvent(std::string audioFileName = "", SoundMaker *maker = nullptr)
            : SoundEvent(audioFileName, maker) {}
    };
    struct SoundCustomEvent : public SoundEvent {
        SoundCustomEvent(void(*func) (std::string audioFileName, SoundMaker *maker)) :
            func(func) {}

        // Function pointer. Plugin a C++11 lambda function for custom behavior
        void(*func) (std::string audioFileName, SoundMaker *maker);
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

}
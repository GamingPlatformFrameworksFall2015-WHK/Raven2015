#pragma once 

#include "Common.h"         // For Common::EAudioType
#include "entityx\Event.h"  // For entityx::Event
#include "entityx\Entity.h" // For entityx::Entity
#include "SFML/System.hpp"
#include "ComponentLibrary.h"

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
    void (*func) (std::string audioFileName, MusicMaker *maker);
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
    void (*func) (std::string audioFileName, SoundMaker *maker);
};

#pragma endregion //AudioEvents

/*
 * An event that stores the identities of two colliding entities.
 */
struct CollisionEvent : public ex::Event<CollisionEvent> {

    /* 
     * Default constructor. Accepts two entities assumed to be colliding,
     * their point of impact, and a reference to the EventManager in case
     * response events need to be emitted.
     */
    CollisionEvent(ex::Entity *leftEntity = nullptr, 
        ex::Entity *rightEntity = nullptr,
        sf::Vector2f *collisionPoint = nullptr, 
        ex::EventManager *events = nullptr)
        : leftEntity(*leftEntity), rightEntity(*rightEntity),
        collisionPoint(collisionPoint), events(events) {

        if (!leftEntity || !rightEntity || !collisionPoint || !events) {
            cerr << "Error: CollisionEvent constructor called with invalid " <<
                "parameter." << endl;
        }

        leftTransform = leftEntity->component<Transform>();
        leftRigidbody = leftEntity->component<Rigidbody>();
        rightTransform = rightEntity->component<Transform>();
        rightRigidbody = rightEntity->component<Rigidbody>();
    }

    // The transform of the "left" entity in the collision.
    ex::ComponentHandle<Transform> leftTransform;

    // The rigidbody of the "left" entity in the collision.
    ex::ComponentHandle<Rigidbody> leftRigidbody;

    // The transform of the "right" entity in the collision.
    ex::ComponentHandle<Transform> rightTransform;

    // The rigidbody of the "right" entity in the collision.
    ex::ComponentHandle<Rigidbody> rightRigidbody;

    // The colliding left entity
    ex::Entity leftEntity;

    // The colliding right entity
    ex::Entity rightEntity;

    // The point of impact between the two colliding entities.
    std::unique_ptr<sf::Vector2f> collisionPoint;

    ex::EventManager *events;
};
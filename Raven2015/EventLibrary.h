#pragma once 

#include "Common.h"         // For Common::EAudioType
#include "entityx\Event.h"  // For entityx::Event
#include "entityx\Entity.h" // For entityx::Entity
#include "SFML/System.hpp"
#include "ComponentLibrary.h"

#pragma region AudioEvents
/*
 * An event that holds data regarding an operation involving an audio resource.
 * Operations include...
 * - LoadAudio      (prepare the file for usage)
 * - PlayAudio      (begin playing the audio file)
 * - PauseAudio     (stop playing. If play again, play at same location)
 * - StopAudio      (Pause / Restart combo. If play again, play at beginning)
 * - ScanAudio      (future feature? move through an audio file. rename?)
 * 
 */
struct AudioEvent : public ex::Event<AudioEvent> {

    // Primary custom constructor. Initializes to zero values.
    AudioEvent(ex::Entity *owner = nullptr,
        std::string audioFileName = "",
        cmn::EAudioType audioType = cmn::EAudioType::NO_TYPE,
        cmn::EAudioOperation audioOperation =
        cmn::EAudioOperation::NO_OPERATION,
        cmn::EAudioLoop audioLoop = cmn::EAudioLoop::UNCHANGED) :
        owner(owner), audioFileName(audioFileName), audioType(audioType),
        audioOperation(audioOperation), audioLoop(audioLoop) {}

    // Abstract destructor
    virtual ~AudioEvent() = 0;

    // The name of the audio file to perform the operation on.
    std::string audioFileName;

    // The type of the audio file to be modified.
    cmn::EAudioType audioType;

    // The operation to be performed on the audio file.
    cmn::EAudioOperation audioOperation;

    // Whether or not an audio resource should alter its loop behavior.
    cmn::EAudioLoop audioLoop;

    // The entity claiming ownership of the audio operation.
    std::unique_ptr<ex::Entity> owner;
};

/*
 * A base class for various types of sound operation events.
 * Sounds must be loaded into memory and are expected to be
 * well under a minute for efficiency reasons.
 */
struct SoundEvent : public AudioEvent {

    // Default constructor
    SoundEvent(std::string soundFileName = "") : 
        AudioEvent(nullptr,"",cmn::EAudioType::SOUND,
            cmn::EAudioOperation::NO_OPERATION,cmn::EAudioLoop::UNCHANGED) {}
};

/*
 * A base class for various types of music operation events.
 * Music is streamed in real time due to the stress loading a minute or more of
 * audio would have on the memory in the system.
 */
struct MusicEvent : public AudioEvent {

    // Default constructor
    MusicEvent(std::string soundFileName = "") : 
        AudioEvent(nullptr,"",cmn::EAudioType::MUSIC,
            cmn::EAudioOperation::NO_OPERATION,cmn::EAudioLoop::UNCHANGED) {}
};

#pragma endregion //AudioEvents

/*
 * An event that stores the identities of two colliding entities.
 */
struct CollisionEvent : public ex::Event<CollisionEvent> {

    /* 
     * Default constructor. Accepts two entities assumed to be colliding and
     * their point of impact.
     */
    CollisionEvent(ex::Entity leftEntity, ex::Entity rightEntity,
        sf::Vector2f collisionPoint)
        : leftEntity(leftEntity), rightEntity(rightEntity),
        collisionPoint(collisionPoint) {

        leftTransform = leftEntity.component<Transform>();
        leftRigidbody = leftEntity.component<Rigidbody>();
        rightTransform = rightEntity.component<Transform>();
        rightRigidbody = rightEntity.component<Rigidbody>();
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
    sf::Vector2f collisionPoint;
};
#pragma once 

#include "Common.h"         // For Common::EAudioType
#include "entityx\Event.h"  // For entityx::Event
#include "entityx\Entity.h" // For entityx::Entity
#include "SFML/System.hpp"
#include "ComponentLibrary.h"

namespace Raven {

#pragma region AudioEvents
    /*
     * An event that holds data regarding an operation involving an audio resource.
     * Operations include...
     * - LOAD      (prepare the file for usage)
     * - PLAY      (begin playing the audio file)
     * - PAUSE     (stop playing. If play again, play at same location)
     * - STOP      (Pause / Restart combo. Playing starts at beginning)
     * 
     * For the future...
     * - *SCAN      (future feature? move through an audio file. rename?)
     */
    struct SoundEvent : public ex::Event<SoundEvent> {

        // Primary custom constructor. Initializes to zero values.
        SoundEvent(sf::SoundBuffer &buffer,
            cmn::EAudioOperation audioOperation =
            cmn::EAudioOperation::NO_OPERATION,
            cmn::EAudioLoop audioLoop = cmn::EAudioLoop::UNCHANGED) :
            buffer(buffer), audioOperation(audioOperation), 
            audioLoop(audioLoop) {}

        // The sound maker responsible for the sound
        sf::SoundBuffer buffer;

        // The operation to be performed on the audio file.
        cmn::EAudioOperation audioOperation;

        // Whether or not an audio resource should alter its loop behavior.
        cmn::EAudioLoop audioLoop;
    };

    /*
     * A base class for various types of music operation events.
     * Music is streamed in real time due to the stress loading a minute or
     * more of audio would have on the memory in the system.
     */
    struct MusicEvent : public ex::Event<MusicEvent> {

        // Primary custom constructor. Initializes to zero values.
        MusicEvent(MusicMaker musicMaker,
            cmn::EAudioOperation audioOperation =
            cmn::EAudioOperation::NO_OPERATION,
            cmn::EAudioLoop audioLoop = cmn::EAudioLoop::UNCHANGED) :
            musicMaker(musicMaker),
            audioOperation(audioOperation), audioLoop(audioLoop) {}

        // The sound maker responsible for the sound
        MusicMaker musicMaker;

        // The operation to be performed on the audio file.
        cmn::EAudioOperation audioOperation;

        // Whether or not an audio resource should alter its loop behavior.
        cmn::EAudioLoop audioLoop;
    };

#pragma endregion //AudioEvents

    /*
     * An event that stores the identities of two colliding entities.
     */
    struct CollisionEvent : public ex::Event<CollisionEvent> {

        /*
         * Default constructor. Accepts two entities assumed to be colliding
         * and their point of impact.
         */
        CollisionEvent(ex::Entity leftEntity, ex::Entity rightEntity,
            sf::Vector2f collisionPoint, ex::EventManager &events)
            : leftEntity(leftEntity), rightEntity(rightEntity),
            collisionPoint(collisionPoint), events(&events) {

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

        // An EventManager reference to permit response events
        ex::EventManager *events;
    };
}
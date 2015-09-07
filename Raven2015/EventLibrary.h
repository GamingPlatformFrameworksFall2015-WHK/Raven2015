#pragma once 

#include "Common.h"         // For Common::EAudioType
#include "entityx\Event.h"  // For entityx::Event
#include "entityx\Entity.h" // For entityx::Entity
#include <string>           // For std::string

namespace ex = entityx;
namespace cmn = Common;

#pragma region AudioEvents
/*
 * An event that holds data regarding an operation involving an audio resource.
 * Operations include...
 * - LoadAudio      (prepare the file for usage)
 * - PlayAudio      (begin playing the audio file)
 * - PauseAudio     (stop playing. If play again, play at same location)
 * - StopAudio      (Pause / Restart combo. If play again, play at beginning)
 * - RestartAudio   (Return to beginning of audio file)
 * - ScanAudio      (future feature? move through an audio file. rename?)
 * 
 * Idea: future feature may include looping audio files
 */
struct AudioEvent : public ex::Event<AudioEvent> {

    // Primary custom constructor. Initializes to zero values.
    AudioEvent(std::string audioFileName = "", 
        cmn::EAudioType audioType = cmn::EAudioType::NONE) : 
        audioType(audioType), audioFileName(audioFileName) {}

    // Abstract destructor
    virtual ~AudioEvent() = 0;

    // The name of the audio file to perform the operation on
    std::string audioFileName;

protected:
    // The type of the audio file to be modified
    cmn::EAudioType audioType;
};

// A base class for various types of sound operation events
struct SoundEvent : public AudioEvent {

    // Default constructor
    SoundEvent(std::string soundFileName = "") {

        audioFileName = soundFileName;
        audioType = cmn::EAudioType::SOUND;
    }
};

// A base class for various types of music operation events
struct MusicEvent : public AudioEvent {

    // Default constructor
    MusicEvent(std::string soundFileName = "") {

        audioFileName = soundFileName;
        audioType = cmn::EAudioType::MUSIC;
    }
};

#pragma region SoundEvents
// An event notifying the AudioSystem of the need to load a sound resource.
struct SoundLoadEvent : SoundEvent {};

// An event notifying the AudioSystem of the need to play a sound resource.
struct SoundPlayEvent : SoundEvent {};

// An event notifying the AudioSystem of the need to pause a sound resource.
struct SoundPauseEvent : SoundEvent {};

// An event notifying the AudioSystem of the need to stop a sound resource.
struct SoundStopEvent : SoundEvent {};

// An event notifying the AudioSystem of the need to restart a sound resource.
struct SoundRestartEvent : SoundEvent {};

// An event notifying the AudioSystem of the need to scan a sound resource.
struct SoundScanEvent : SoundEvent {};
#pragma endregion

#pragma region MusicEvents
// An event notifying the AudioSystem of the need to load a music resource.
struct MusicLoadEvent : MusicEvent {};

// An event notifying the AudioSystem of the need to play a music resource.
struct MusicPlayEvent : MusicEvent {};

// An event notifying the AudioSystem of the need to pause a music resource.
struct MusicPauseEvent : MusicEvent {};

// An event notifying the AudioSystem of the need to stop a music resource.
struct MusicStopEvent : MusicEvent {};

// An event notifying the AudioSystem of the need to restart a music resource.
struct MusicRestartEvent : MusicEvent {};

// An event notifying the AudioSystem of the need to scan a music resource.
struct MusicScanEvent : MusicEvent {};
#pragma endregion //MusicEvents

#pragma endregion //AudioEvents

/*
 * An event that stores the identities of two colliding entities.
 */
struct CollisionEvent : public ex::Event<CollisionEvent> {

    // Default constructor. Accepts to entities assumed to be colliding.
    CollisionEvent(ex::Entity &left, ex::Entity &right)
        : leftEntity(left), rightEntity(right) {}

    // The entity on the former side of the collision
    ex::Entity leftEntity;

    // The entity on the latter side of the collision
    ex::Entity rightEntity;
};
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

#include <SFML/Graphics.hpp>    // For sf::Vector2f
#include <SFML/Audio/Sound.hpp> // For sf::Sound
#include <SFML/Audio/Music.hpp> // For sf::Music
#include <map>                  // For std::map
#include <string>               // For std::string
#include "entityx\Entity.h"     // For entityx::Component
#include "Common.h"             // For EAudioType

#pragma region Physics

/*
 * A component enabling a passive physical state. Required for placement.
 */
struct Transform : public ex::Component<Transform> {

    // Default constructor
    Transform(const float transformX = 0.0f, const float transformY = 0.0f,
        const float rotation = 0.0f) : rotation(rotation) {

        transform.x = transformX;
        transform.y = transformY;
    }

    // Primary custom constructor. Requires placement with optional rotation.
    Transform(const sf::Vector2f &transform, float rotation = 0.0)
        : transform(transform), rotation(rotation) {}

    // The x and y coordinates of the entity's Origin.
    sf::Vector2f transform;

    /* 
     * The orientation of the entity, measured in degrees. 
     * Assumes that 0 begins at the right, running counterclockwise.
     */
    float rotation;
};

/*
 * A component enabling a dynamic physical state. Required for movement.
 */
struct Rigidbody : public ex::Component<Rigidbody> {

    // Default Constructor. All fields initialized to zero.
    Rigidbody(const float velocityX = 0.0f, const float velocityY = 0.0f,
        const float accelerationX = 0.0f, const float accelerationY = 0.0f, 
        const float radialVelocity = 0.0f) : radialVelocity(radialVelocity) {
    
        velocity.x = velocityX;
        velocity.y = velocityY;
        acceleration.x = accelerationX;
        acceleration.y = accelerationY;
    }

    /*
     * Custom constructor. Requires velocity and acceleration as sf::Vector2f 
     * objects with an optional radialVelocity.
     */
    Rigidbody(const sf::Vector2f &velocity, const sf::Vector2f &acceleration,
        const float radialVelocity = 0.0f) : velocity(velocity),
        acceleration(acceleration), radialVelocity(radialVelocity) {}

    // The x and y components of the entity's current velocity.
    sf::Vector2f velocity;

    // The x and y components of the entity's current acceleration.
    sf::Vector2f acceleration;

    // The turning rate of the entity in degrees per second, counterclockwise.
    float radialVelocity;
};

/*
 * An abstract component used to identify collision areas
 * The origin's x and y values are relative to its Transform.
 */
struct Collider : public ex::Component<Collider> {

    // Default constructor
    Collider() {}

    // Abstract deconstructor
    virtual ~Collider() = 0;

    // The x-y coordinate of the collider's center relative to its transform.
    sf::Vector2f origin;
};

// Empty implementation of pure abstract destructor
inline Collider::~Collider() {}

/*
 * A Collider with a box-shaped collision area
 */
struct BoxCollider : Collider {

    /*
     * Default constructor
     * 
     * Parameters:
     * 1. length: desired length. Defaults to Common::STD_UNITX
     * 2. height: desired height. Defaults to Common::STD_UNITY
     * 3. x: x-axis relative offset from Transform's transform
     * 4. y: y-axis relative offset from Transform's transform
     */
    BoxCollider(const float length = cmn::STD_UNITX, 
        const float height = cmn::STD_UNITY, 
        const float x = 0.0f, const float y = 0.0f) 
        : length(length), height(height) {
    
        origin.x = x;
        origin.y = y;
    }

    // Custom constructor (scales)
    BoxCollider(const float scale, const float x = 0.0f, 
        const float y = 0.0f) 
        : BoxCollider(cmn::STD_UNITX,cmn::STD_UNITY,x,y) {
        length *= scale;
        height *= scale;
    }

    // Deconstructor
    virtual ~BoxCollider() override {}

    // The range of the x-axis of the collider. Origin in the middle.
    float length;

    // The range of the y-axis of the collider. Origin in the middle.
    float height;
};

/*
 * A Collider with a circle-shaped collision area.
 */
struct CircleCollider : Collider {

    // Default constructor. Defaults radius to 1/2 the standard unit in Common
    CircleCollider(const float radius = 0.5*(cmn::STD_UNITX),
        const float x = 0.0f, const float y = 0.0f)
        : radius(radius) {
    
        origin.x = x;
        origin.y = y;
    }

    // Deconstructor
    virtual ~CircleCollider() override {}

    // The radius of the circular collision area
    float radius;
};

#pragma endregion

#pragma region Audio

/*
 * An abstract component to track the names of playable audio files.
 */
struct BaseAudioMaker : public ex::Component<BaseAudioMaker> {

    // Default constructor
    BaseAudioMaker(int numFiles = 0) {
        if (numFiles > 0) {
            filenames.resize(numFiles);
        }
    };

    // Custom constructor requiring an std::string with an optional start size.
    BaseAudioMaker(std::string &soundFile, int numFiles = 1) {
        filenames.push_back(soundFile);
        if (numFiles > 1) {
            filenames.resize(numFiles);
        }
    }

    virtual ~BaseAudioMaker() = 0;

    // A list of the names of the playable audio files. 
    std::vector<std::string> filenames;
};

/*
 * A component that stores the names of tracks for sf::Sound.
 * sf::Sound is optimized for small audio tracks (a few seconds).
 *
 * TODO: Restrict types playable with these filenames to sf::Sound objects.
 */
struct SoundMaker : public BaseAudioMaker { 
    
    // Default constructor
    SoundMaker() {} 

    // Label identifying the types of audio files permitted to be played
    static const cmn::EAudioType audioType = cmn::SOUND;
};

/*
 * A component that stores the names of tracks for sf::Music.
 * sf::Music is optimized for large audio tracks (~30 seconds+).
 *
 * TODO: Restrict types playable with these filenames to sf::Music objects.
 */
struct MusicMaker : public BaseAudioMaker { 
    
    // Default constructor
    MusicMaker() {} 

    // Label identifying the types of audio files permitted to be played
    static const cmn::EAudioType audioType = cmn::MUSIC;
};

#pragma endregion


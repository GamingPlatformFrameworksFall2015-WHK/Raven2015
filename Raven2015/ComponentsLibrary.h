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
#include <SFML/System.hpp>      // For sf::String
#include <map>                  // For std::map
#include <string>               // For std::string
#include "entityx\Entity.h"     // For entityx::Component

namespace ex = entityx;

/*
 * A component enabling a passive physical state. Required for placement.
 */
struct Transform : public ex::Component<Transform> {

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
 * An abstract component to track the names of playable audio files.
 */
struct BaseAudioMaker : public ex::Component<BaseAudioMaker> {

    // Default constructor
    BaseAudioMaker(int numFiles = 0) {
        if (numFiles > 0) {
            files.resize(numFiles);
        }
    };

    // Custom constructor requiring an sf::String with an optional start size.
    BaseAudioMaker(sf::String &soundFile, int numFiles = 1) {
        files.push_back(soundFile);
        if (numFiles > 1) {
            files.resize(numFiles);
        }
    }

    virtual ~BaseAudioMaker() = 0;

    // A list of the names of the playable audio files. 
    std::vector<sf::String> files;
};

/*
 * A component that stores the names of tracks for sf::Sound.
 * sf::Sound is optimized for small audio tracks (a few seconds).
 *
 * TODO: Restrict types playable with these filenames to sf::Sound objects.
 */
struct SoundMaker : public BaseAudioMaker { SoundMaker() {} };

/*
 * A component that stores the names of tracks for sf::Music.
 * sf::Music is optimized for large audio tracks (~30 seconds+).
 *
 * TODO: Restrict types playable with these filenames to sf::Music objects.
 */
struct MusicMaker : public BaseAudioMaker { MusicMaker() {} };
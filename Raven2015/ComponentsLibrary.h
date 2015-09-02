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
 * A lightweight component allowing for a single sound to be played.
 * sf::Sound is optimized for small audio tracks (a few seconds).
 */
struct SimpleSoundMaker : public ex::Component<SimpleSoundMaker> {

    // Default constructor
    SimpleSoundMaker() {};

    // Custom constructor requiring an sf::Sound for initialization.
    SimpleSoundMaker(const sf::Sound &sound) : sound(sound) {}
    
    // The sound playable by the owning entity.
    sf::Sound sound;
};

/*
 * A lightweight component allowing for a single song to be played.
 * sf::Music is optimized for large audio tracks (~30 seconds+).
 */
struct SimpleMusicMaker : public ex::Component<SimpleMusicMaker> {

    // Default constructor
    SimpleMusicMaker() {};

    /* copy constructor for sf::Music is deleted, so there is no comparative
     * custom constructor for sf::Music based components.
     */

    // The music playable by the owning entity.
    sf::Music music;
};

/*
 * A heavyweight component allowing for several sounds to be maintained.
 * sf::Sound is optimized for small audio tracks (a few seconds).
 */
struct MultiSoundMaker : public ex::Component<MultiSoundMaker> {

    // Default constructor
    MultiSoundMaker() {}

    // Custom constructor requiring an std::map<sf::String, sf::Sound> for initialization
    MultiSoundMaker(std::map<sf::String, sf::Sound> aMap) : musicMap(aMap) {}
    
    // A map of the sounds playable by the owning entity.
    std::map<sf::String,sf::Sound> musicMap;
};

/*
 * A heavyweight component allowing for several songs to be maintained.
 * sf::Music is optimized for large audio tracks (~30 seconds+).
 */
struct MultiMusicMaker : public ex::Component<MultiMusicMaker> {

    // Default constructor
    MultiMusicMaker() {}

    /* copy constructor for sf::Music is deleted, so there is no comparative
     * custom constructor for sf::Music based components.
     */

    // A map of the music playable by the owning entity.
    std::map<sf::String,sf::Music> musicMap;
};

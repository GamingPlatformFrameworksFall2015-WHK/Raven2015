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

#include <SFML/Graphics.hpp>            // For sf::Vector2f
#include <SFML/Audio/Sound.hpp>         // For sf::Sound
#include <SFML/Audio/Music.hpp>         // For sf::Music
#include <SFML/Audio/SoundBuffer.hpp>   // For sf::SoundBuffer
#include <map>                          // For std::map
#include <string>                       // For std::string
#include "entityx\Entity.h"             // For entityx::Component
#include "Common.h"                     // For EAudioType

namespace Raven {

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
     * The originOffset's x and y values are relative to its Transform.
     */
    struct Collider : public ex::Component<Collider> {

        // Default constructor
        Collider() {}

        // Abstract deconstructor
        virtual ~Collider() = 0;

        // The x-y coordinate of the collider's center relative to its transform.
        sf::Vector2f originOffset;
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
         * 1. width: desired width. Defaults to Common::STD_UNITX
         * 2. height: desired height. Defaults to Common::STD_UNITY
         * 3. x: x-axis relative offset from Transform's transform
         * 4. y: y-axis relative offset from Transform's transform
         */
        BoxCollider(const float width = cmn::STD_UNITX,
            const float height = cmn::STD_UNITY,
            const float x = 0.0f, const float y = 0.0f)
            : width(width), height(height) {

            originOffset.x = x;
            originOffset.y = y;
        }

        // Custom constructor (scales)
        BoxCollider(const float scale, const float x = 0.0f,
            const float y = 0.0f)
            : BoxCollider(cmn::STD_UNITX, cmn::STD_UNITY, x, y) {
            width *= scale;
            height *= scale;
        }

        // Deconstructor
        virtual ~BoxCollider() override {}

        // The range of the x-axis of the collider. Origin in the middle.
        float width;

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

            originOffset.x = x;
            originOffset.y = y;
        }

        // Deconstructor
        virtual ~CircleCollider() override {}

        // The radius of the circular collision area
        float radius;
    };

#pragma endregion

#pragma region Audio

    struct AudioMaker : public ex::Component<AudioMaker> {

    };

    /*
     * A component that stores the names of tracks for sf::Sound.
     * sf::Sound is optimized for small audio tracks (a few seconds).
     *
     * TODO: Restrict types playable with these filenames to sf::Sound objects.
     */
    struct SoundMaker : public AudioMaker {

        // Default constructor
        SoundMaker()  {}

        // A mapping between sound file names and the buffers for their storage
        SOUNDMAP_T soundMap;

        // An object for performing sound operations on a buffer.
        sf::Sound sound;
    };

    /*
     * A component that stores the names of tracks for sf::Music.
     * sf::Music is optimized for large audio tracks (~30 seconds+).
     *
     * TODO: Restrict types playable with these filenames to sf::Music objects.
     */
    struct MusicMaker : public AudioMaker {

        // Default constructor
        MusicMaker() {}

        // A mapping between music file names and their stream storage objects
        MUSICMAP_T musicMap;
    };

#pragma endregion

#pragma region Rendering

    struct Renderer : public ex::Component<Renderer> {

    };

    struct TextRenderer : public Renderer {

        TextRenderer() {
            font = sf::Font();
            text = sf::Text("Hello World", font);
        }
        
        sf::Font font;

        sf::Text text;
    };

    /*
     * A component that permits the rendering of a sprite for a given entity
     */
    struct SpriteRenderer : public Renderer {

        // Default constructor. SpriteRenderer::sprite left uninitialized
        SpriteRenderer(std::string file = "", 
            cmn::ERenderingLayer layer = cmn::ERenderingLayer::NO_LAYER, 
            int priority = 0) : textureFileName(file), renderLayer(layer), 
            renderPriority(priority) {
        
            sprite.setTextureRect(sf::IntRect(0, 0, (int)cmn::STD_UNITX, (int)cmn::STD_UNITY));
        }

        // The source textures for the sprite
        std::string textureFileName;

        // The current window into the texture being displayed to the screen
        sf::Sprite sprite;

        // The rendering layer for macro-sorting of render content
        cmn::ERenderingLayer renderLayer;

        // The drawing-order priority within the rendering layer. Top-most = high priority
        int renderPriority;

        bool operator<(const SpriteRenderer &other) {
            if (renderLayer < other.renderLayer) {
                return true;
            }
            else if (renderLayer == other.renderLayer) {
                if (renderPriority < other.renderPriority) {
                    return true;
                }
            /*  else if (renderPriority == other.renderPriority)
             *      Behavior undefined. No guarantees for which
             *      object will be drawn on top
             *  }
             */ 
            }
            return false;
        }
    };

    /*
     * A helper class to contain information regarding a given Animation.
     * A given texture (as a spritesheet) may contain several animation frames.
     */
    struct Animation {

        /*
         * Default Constructor
         * Initializes an animation with default values. For necessary functionality, include
         * the textureFileName and size at minimum.
         * Assume any given "spritesheet" AKA "animation texture" is purely a single animation
         * with only horizontal translation between frames.
         */
        Animation(std::string textureFileName = "", int size = 0, bool isLooping = false, float animationSpeed = 1.0f,
                int frameWidth = cmn::STD_UNITX, int frameHeight = cmn::STD_UNITY)
                : textureFileName(textureFileName), size(size), isLooping(isLooping), animationSpeed(animationSpeed),
                animationProgress(0.0f), frameWidth(frameWidth), frameHeight(frameHeight) {

            // Ensure that we have one viewing rectangle (sf::IntRect) into the texture for each sprite frame
            frames.resize(size);

            // Ensure that each successive window is placed over the next frame of the spritesheet animation in turn
            for (int i = 0; i < frames.size(); ++i) {
                frames[i].width = frameWidth;
                frames[i].height = frameHeight;
                frames[i].left += i*frameWidth;
            }
        }

        // The sections of the texture the animation draws from for each sprite
        std::vector<sf::IntRect> frames;
        
        // Whether the sprite animation should stop at the end or loop back to the start
        bool isLooping;

        // The width of each frame in the imported spritesheet texture
        int frameWidth;

        // The height of each frame in the imported spritesheet texture
        int frameHeight;

        // The number of frames in the spritesheet animation
        int size;

        // The speed of the animation (how many ticks per 1 iteration of the animation loop?)
        double animationSpeed;

        // The current progress towards reaching the animationSpeed threshold
        double animationProgress;

        // The name of the texture file referenced by the animation (the spritesheet, single line)
        std::string textureFileName;
    };

    /*
     * A component that tracks the Animations available to a given SpriteRenderer
     * and manages switching between them.
     */
    struct Animator : public ex::Component<Animator> {

        // Default Constructor
        Animator() : animName(""), frameId(0) {}

        // Custom Constructor, string + Animation
        Animator(std::string animName) : frameId(0) {
            // Error checking for animation name validity
            if (animName == "") {
                cerr << "Error: Attempt to register animation to empty string." << endl;
                throw 1;
            }

            this->animName = animName;
        }

        // The name of the animation in use
        std::string animName;
        
        // The index of the frame of the animation currently being displayed
        int frameId;
    };

#pragma endregion

#pragma region Timers

    struct Timer {

        // Default constructor
        Timer(std::shared_ptr<sf::Clock> clock = nullptr,
            ex::TimeDelta dTime = 0.0, bool isPlaying = true)
            : clock(clock ? clock : std::shared_ptr<sf::Clock>(new sf::Clock())),
            eTime(dTime), isPlaying(isPlaying) {}

        // Return private variable
        ex::TimeDelta getETime() {
            return eTime + clock->getElapsedTime().asSeconds();
        }

        // Rewind or forward timer by scanDis amount
        void scan(ex::TimeDelta scanDis) {
            eTime += scanDis;
        }

        // Pause timer only if timer is currently playing
        void pause() {
            if (isPlaying) {
                eTime += clock->restart().asSeconds();
                isPlaying = false;
            }
            else {
                cerr << "Timer already paused." << endl;
            }
        }

        // Play timer only if timer is currently paused
        void play() {
            if (!isPlaying) {
                clock->restart();
                isPlaying = true;
            }
            else {
                cerr << "Timer is already playing." << endl;
            }
        }

        //
        void reset() {
            eTime = 0.0;
            clock->restart();
        }



    private:
        // Clock for recording time
        std::shared_ptr <sf::Clock> clock;
        // Record of elapsed time
        ex::TimeDelta eTime;
        // Current state of timer
        bool isPlaying;
    };

    struct TimeTable : public ex::Component<TimeTable> {
        // Default constructor
        TimeTable() {}

        // Maps a name to current timers
        std::map<std::string, Timer> timerMap;
    };

#pragma endregion

#pragma region Behaviors

    struct BeginPlayBehavior : ex::Component<BeginPlayBehavior> {

        void(*beginPlay)();

    };

    struct UpdateBehavior : ex::Component<UpdateBehavior> {

        void(*update)(ex::TimeDelta dt);

    };

#pragma endregion


}

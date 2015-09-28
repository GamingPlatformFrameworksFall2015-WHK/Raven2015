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
        
        /// <summary>
        /// Initializes a new instance of the <see cref="Rigidbody"/> struct.
        /// </summary>
        /// <param name="velocity">The velocity.</param>
        /// <param name="acceleration">The acceleration.</param>
        /// <param name="radialVelocity">The radial velocity. (Optional)</param>
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
    
    /// <summary>
    /// <para>An abstract component used to identify collision areas.</para>
    /// <para>The originOffset's x and y values are relative to its Transform.</para>
    /// </summary>
    struct Collider : public ex::Component<Collider> {
        
        /// <summary>
        /// Initializes a new instance of the <see cref="Collider"/> struct.
        /// </summary>
        Collider() {}
                
        /// <summary>
        /// Abstract definition for virtual destructor to finalize an instance of 
        /// the abstract <see cref="Collider" /> class.
        /// </summary>
        virtual ~Collider() = 0;

        // The x-y coordinate of the collider's center relative to its transform.
        sf::Vector2f originOffset;
    };
        
    /// <summary>
    /// Inline empty implementation of <see cref="Collider"/>'s pure virtual deconstructor
    /// </summary>
    inline Collider::~Collider() {}
    
    /// <summary>
    /// A Collider with a box-shaped collision area
    /// </summary>
    struct BoxCollider : Collider {

         /// <summary>
         /// Initializes a new instance of the <see cref="BoxCollider"/> struct.
         /// </summary>
         /// <param name="width">The width. Defaults to Common::STD_UNITX</param>
         /// <param name="height">The height. Defaults to Common::STD_UNITY</param>
         /// <param name="x">The x coordinate.</param>
         /// <param name="y">The y coordinate.</param>
        BoxCollider(const float width = cmn::STD_UNITX,
            const float height = cmn::STD_UNITY,
            const float x = 0.0f, const float y = 0.0f)
            : width(width), height(height) {

            originOffset.x = x;
            originOffset.y = y;
        }
        
        /// <summary>
        /// Initializes a new scaled instance of the <see cref="BoxCollider"/> struct.
        /// </summary>
        /// <param name="scale">The scale.</param>
        /// <param name="x">The x.</param>
        /// <param name="y">The y.</param>
        BoxCollider(const float scale, const float x = 0.0f,
            const float y = 0.0f)
            : BoxCollider(cmn::STD_UNITX, cmn::STD_UNITY, x, y) {
            width *= scale;
            height *= scale;
        }
        
        /// <summary>
        /// Finalizes an instance of the <see cref="BoxCollider"/> class.
        /// </summary>
        virtual ~BoxCollider() override {}
        
        /// <summary>
        /// The range of the x-axis of the collider. Origin in the middle.
        /// </summary>
        float width;
        
        /// <summary>
        /// The range of the y-axis of the collider. Origin in the middle.
        /// </summary>
        float height;
    };
    
    /// <summary>
    /// A Collider with a circle-shaped collision area.
    /// </summary>
    struct CircleCollider : Collider {
        
        /// <summary>
        /// Initializes a new instance of the <see cref="CircleCollider"/> struct.
        /// </summary>
        /// <remarks>Defaults radius to 1/2 the standard unit in Common</remarks>
        /// <param name="radius">The radius</param>
        /// <param name="x">The x coordinate</param>
        /// <param name="y">The y coordinate</param>
        CircleCollider(const float radius = 0.5*(cmn::STD_UNITX),
            const float x = 0.0f, const float y = 0.0f)
            : radius(radius) {

            originOffset.x = x;
            originOffset.y = y;
        }
        
        /// <summary>
        /// Finalizes an instance of the <see cref="CircleCollider"/> class.
        /// </summary>
        virtual ~CircleCollider() override {}
        
        /// <summary>
        /// The radius of the circular collision area
        /// </summary>
        float radius;
    };

#pragma endregion

#pragma region Audio

    /// <summary>
    /// <para>A component that acts as a base class for audio-storage data</para>
    /// </summary>
    struct AudioMaker : public ex::Component<AudioMaker> {

    };

    /// <summary>
    /// <para>A component that stores the names of tracks for sf::Sound.</para>
    /// <para>sf::Sound is optimized for small audio tracks (a few seconds).</para>
    /// </summary>
    struct SoundMaker : public AudioMaker {

        /// <summary>
        /// Default constructor
        /// </summary>
        SoundMaker()  {}

        /// <summary>
        /// A mapping between sound file names and the buffers for their storage
        /// </summary>
        SOUNDMAP_T soundMap;

        /// <summary>
        /// An object for performing sound operations on a buffer.
        /// </summary>
        sf::Sound sound;
    };

    /// <summary>
    /// <para>A component that stores the names of tracks for sf::Music.</para>
    /// <para>sf::Music is optimized for large audio tracks (~30 seconds+).</para>
    /// </summary>
    struct MusicMaker : public AudioMaker {

        /// <summary>
        /// Default constructor
        /// </summary>
        MusicMaker() {}

        /// <summary>
        /// A mapping between music file names and their stream storage objects
        /// </summary>
        MUSICMAP_T musicMap;
    };

#pragma endregion

#pragma region Rendering

     /// <summary>
     /// <para>A wrapper class around drawable assets to allow for sorting.</para>
     /// <para>Sorting is based on layer first, priority second.</para>
     /// <para>A low priority means it will be drawn first, i.e. below other objects</para>
     /// </summary>
    struct Renderable {

        Renderable(const cmn::ERenderingLayer &renderLayer = cmn::ERenderingLayer::NO_LAYER, const int renderPriority = 0)
            : renderLayer(renderLayer), renderPriority(renderPriority), drawPtr(nullptr) {}

        /// <summary>
        /// A Drawable pointer used SOLELY for generic drawing (errors occurred otherwise)
        /// </summary>
        sf::Drawable* drawPtr;

        /// <summary>
        /// The rendering layer for macro-sorting of render content
        /// </summary>
        cmn::ERenderingLayer renderLayer;


        /// <summary>
        /// The drawing-order priority within the rendering layer. Top-most = high priority
        /// </summary>
        int renderPriority;

        /// <summary>
        /// <para>Assuming usage of priority queue with fixed max-heap functionality</para>
        /// <para>Need to have a 'less-than' operator that places higher priorities at minimum values</para>
        /// <para>Therefore, it simply references the 'greater-than' operator to achieve the desired inversion</para>
        /// </summary>
        /// <param name="other">The Renderable to compare against</param>
        /// <returns>Whether this Renderable is "greater-than" than the other</returns>
        bool operator<(const Renderable &other) const {
            return *this > other;
        }
        
        /// <summary>
        /// <para>Helps to sort Renderables based on layer and priority.</para>
        /// <para>Higher priorities are given minimal values b/c
        /// priority queues pop off items max-first</para>
        /// </summary>
        /// <param name="other">The Renderable to compare against</param>
        /// <returns>Whether this Renderable is "greater" than the other</returns>
        bool operator>(const Renderable &other) const {
            if (renderLayer > other.renderLayer) {
                return true;
            }
            else if (renderLayer == other.renderLayer) {
                if (renderPriority > other.renderPriority) {
                    return true;
                }
            /// else if (renderPriority == other.renderPriority)
            ///     Behavior undefined. No guarantees for which
            ///     object will be drawn on top
            /// }
            ///
            }
            return false;
        }
    };

    /// <summary>
    /// A sortable Text for rendering
    /// </summary>
    struct RenderableText : public Renderable {

        RenderableText(const std::string &textContent = "", const sf::Vector2f &position = sf::Vector2f(),
            const std::string &fontFile = "", const sf::Color &color = sf::Color::White,
            cmn::ERenderingLayer renderLayer = cmn::ERenderingLayer::NO_LAYER, int renderPriority = 0)
            : Renderable(renderLayer, renderPriority) {

            drawPtr = &text;

            if (!font.loadFromFile(fontFile)) {
                cerr << "Error: RenderableText failed to load font file <" + fontFile + ">" << endl;
                throw 1;
            }

            text.setString(textContent);
            text.setColor(color);
            text.setPosition(position);
            text.setFont(font);
        }

        /// <summary>
        /// A Font used to format text
        /// </summary>
        sf::Font font;

        /// <summary>
        /// A Text used to draw text to the window        
        /// </summary>
        sf::Text text;
    };

    /// <summary>
    /// A base class for sortable Shapes for rendering
    /// </summary>
    struct RenderableShape : public Renderable {

        RenderableShape(const cmn::ERenderingLayer &renderLayer = cmn::ERenderingLayer::NO_LAYER, const int renderPriority = 0)
            : Renderable(renderLayer, renderPriority) {}

    };

    /// <summary>
    /// A base class for sortable Circles for rendering
    /// </summary>
    struct RenderableCircle : public RenderableShape {

        RenderableCircle(const cmn::ERenderingLayer &renderLayer = cmn::ERenderingLayer::NO_LAYER, const int renderPriority = 0)
            : RenderableShape(renderLayer, renderPriority) {
        
            drawPtr = &circle;
        }

        sf::CircleShape circle;
    };

    /// <summary>
    /// A base class for sortable Rectangles for rendering
    /// </summary>
    struct RenderableRectangle : public RenderableShape {

        RenderableRectangle(const cmn::ERenderingLayer &renderLayer = cmn::ERenderingLayer::NO_LAYER, const int renderPriority = 0)
            : RenderableShape(renderLayer, renderPriority) {
        
            drawPtr = &rectangle;
        }

        sf::RectangleShape rectangle;
    };

    /// <summary>
    /// A base class for sortable Sprites for rendering & animation
    /// </summary>
    struct RenderableSprite : public Renderable {

        RenderableSprite(const std::string &textureFileName = "",
            const std::string &animName = "", const int frameId = 0,
            const cmn::ERenderingLayer &renderLayer = cmn::ERenderingLayer::NO_LAYER, const int renderPriority = 0)
            : Renderable(renderLayer, renderPriority), textureFileName(textureFileName), animName(animName), 
            frameId(frameId), sprite() {
        
            cout << "renderSprite contructor entering" << endl;
            drawPtr = &sprite;
        }

        /// <summary>
        /// The source texture file for the current sprite(sheet)        
        /// </summary>
        std::string textureFileName;

        /// <summary>
        /// The name of the animation in use. Empty string ("") indicates no animation necessary
        /// </summary>
        std::string animName;

        /// <summary>
        /// The index of the frame of the animation currently being displayed. Only significant if animation is necessary
        /// </summary>
        int frameId;

        /// <summary>
        /// The sprite to be sorted
        /// </summary>
        sf::Sprite sprite;
    };


    /// <summary>
    /// A component used to store renderable assets. Each asset is mapped by a name
    /// </summary>
    struct Renderer : public ex::Component<Renderer> {

        Renderer() {

            //Implement details of XML serialization and deserialization here

        }

        //virtual ~Renderer() override {}

        /// <summary>
        /// Maps a string name to a given Text to be rendered        
        /// </summary>
        std::map<std::string, RenderableText> texts;

        /// <summary>
        /// Maps a string name to a given Rectangle to be rendered        
        /// </summary>
        std::map<std::string, RenderableRectangle> rectangles;

        /// <summary>
        /// Maps a string name to a given Circle to be rendered        
        /// </summary>
        std::map<std::string, RenderableCircle> circles;

        /// <summary>
        /// Maps a string name to a given Sprite to be rendered        
        /// </summary>
        std::map<std::string, RenderableSprite> sprites;
    };

     /// <summary>
     /// <para>A helper class to contain information regarding a given Animation.</para>
     /// <para>A given texture (as a spritesheet) may contain several animation frames.</para>
     /// </summary>
    struct Animation {

         /// <summary>
         /// <para>Initializes a new instance of the <see cref="Animation"/> struct.</para>
         /// <para>Assume any given "spritesheet" AKA "animation texture" is purely a single animation
         /// with only horizontal translation between frames.</para>
         /// </summary>
         /// <param name="textureFileName">Name of the texture file.</param>
         /// <param name="size">The size.</param>
         /// <param name="isLooping">if set to <c>true</c> [is looping].</param>
         /// <param name="animationSpeed">The animation speed.</param>
         /// <param name="frameWidth">Width of the frame.</param>
         /// <param name="frameHeight">Height of the frame.</param>
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

        /// <summary>
        /// The sections of the texture the animation draws from for each sprite        
        /// </summary>
        std::vector<sf::IntRect> frames;
        
        /// <summary>
        /// Whether the sprite animation should stop at the end or loop back to the start        
        /// </summary>
        bool isLooping;

        /// <summary>
        /// The width of each frame in the imported spritesheet texture        
        /// </summary>
        int frameWidth;

        /// <summary>
        /// The height of each frame in the imported spritesheet texture        
        /// </summary>
        int frameHeight;

        /// <summary>
        /// The number of frames in the spritesheet animation        
        /// </summary>
        int size;

        /// <summary>
        /// The speed of the animation (how many ticks per 1 iteration of the animation loop?)        
        /// </summary>
        double animationSpeed;

        /// <summary>
        /// The current progress towards reaching the animationSpeed threshold
        /// </summary>
        double animationProgress;

        /// <summary>
        /// The name of the texture file referenced by the animation (the spritesheet, single line)        
        /// </summary>
        std::string textureFileName;
    };

#pragma endregion

#pragma region Timers

    /// <summary>
    /// A wrapper for an sf::Clock and ex::TimeDelta that allows a higher level of control over the clock
    /// </summary>
    struct Timer {
        
        /// <summary>
        /// Initializes a new instance of the <see cref="Timer"/> struct.
        /// </summary>
        /// <param name="clock">The clock.</param>
        /// <param name="deltaTime">The delta time.</param>
        /// <param name="isPlaying">if set to <c>true</c> [is playing].</param>
        Timer(sf::Clock clock = sf::Clock(), ex::TimeDelta deltaTime = 0.0, bool isPlaying = true)
            : elapsedTime(deltaTime), isPlaying(isPlaying) {}
        
        /// <summary>
        /// Gets the elapsed time.
        /// </summary>
        /// <returns>The time since the last restart operation</returns>
        ex::TimeDelta getElapsedTime() {
            return elapsedTime + clock.getElapsedTime().asSeconds();
        }

        /// <summary>
        /// Rewind or advance the timer by a given amount        
        /// </summary>
        /// <param name="scanDis">The distance to be scanned left or right in the timeline</param>
        void scan(ex::TimeDelta scanDistance) {
            elapsedTime += scanDistance;
        }

        /// <summary>
        /// Pause timer only if timer is currently playing        
        /// </summary>
        void pause() {
            if (isPlaying) {
                elapsedTime += clock.restart().asSeconds();
                isPlaying = false;
            }
            else {
                cerr << "Timer already paused." << endl;
            }
        }

        /// <summary>
        /// Play timer only if timer is currently paused        
        /// </summary>
        void play() {
            if (!isPlaying) {
                clock.restart();
                isPlaying = true;
            }
            else {
                cerr << "Timer is already playing." << endl;
            }
        }
                
        /// <summary>
        /// Restarts the timer, returning the time up to that point
        /// </summary>
        /// <returns>The accumulated time up till the point of restart</returns>
        ex::TimeDelta restart() {
            ex::TimeDelta time = getElapsedTime();
            elapsedTime = 0.0;
            clock.restart();
            return time;
        }



    private:        
        /// <summary>
        /// An sf::Clock to assist in recording time
        /// </summary>
        sf::Clock clock;
        
        /// <summary>
        /// Record of elapsed time
        /// </summary>
        ex::TimeDelta elapsedTime;
        
        /// <summary>
        /// Current state of timer
        /// </summary>
        bool isPlaying;
    };

    struct TimeTable : public ex::Component<TimeTable> {        
        /// <summary>
        /// Initializes a new instance of the <see cref="TimeTable"/> struct.
        /// </summary>
        TimeTable() {}
        
        /// <summary>
        /// Maps a name to current timers
        /// <remarks>Needs to be ported to a TimerSystem and keep just the names of owned timers</remarks>
        /// </summary>
        std::map<std::string, Timer> timerMap;
    };

#pragma endregion


#pragma region Behaviors

    /// <summary>
    /// A baseline Behavior component with functionality that many entities will likely reference.
    /// </summary>
    struct CoreBehavior : public ex::Component<CoreBehavior> {
        
        /// <summary>
        /// Initializes a new instance of the <see cref="CoreBehavior"/> struct.
        /// </summary>
        CoreBehavior() : awake(nullptr), beginPlay(nullptr), onCollisionEnter(nullptr), onCollisionExit(nullptr),
            preUpdate(nullptr), postUpdate(nullptr) {}
        
        /// <summary>
        /// Called when the entity instance is loaded
        /// </summary>
        void(*awake)();        

        /// <summary>
        /// Called when the game begins
        /// </summary>
        void(*beginPlay)();        

        /// <summary>
        /// Called if the entity has a Collider and has start colliding with another entity's Collider
        /// </summary>
        void(*onCollisionEnter)(ex::Entity &other);        

        /// <summary>
        /// Called if the entity has a Collider and has stopped colliding with another entity's Collider
        /// </summary>
        void(*onCollisionExit)(ex::Entity &other);        

        /// <summary>
        /// Called at the beginning of each frame before other System updates
        /// </summary>
        void(*preUpdate)(ex::TimeDelta dt);        

        /// <summary>
        /// Called at the end of each frame after other System updates
        /// </summary>
        void(*postUpdate)(ex::TimeDelta dt);

    };

#pragma endregion


}

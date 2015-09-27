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

    /*
     * A wrapper class around drawable assets to allow for sorting.
     * Sorting is based on layer first, priority second.
     * A low priority means it will be drawn first, i.e. below other objects
     */
    struct Renderable {

        Renderable(const cmn::ERenderingLayer &renderLayer = cmn::ERenderingLayer::NO_LAYER, const int renderPriority = 0)
            : renderLayer(renderLayer), renderPriority(renderPriority), drawPtr(nullptr) {}

        // A Drawable pointer used SOLELY for generic drawing (errors occurred otherwise)
        std::shared_ptr<sf::Drawable> drawPtr;

        // The rendering layer for macro-sorting of render content
        cmn::ERenderingLayer renderLayer;

        // The drawing-order priority within the rendering layer. Top-most = high priority
        int renderPriority;

        bool operator<(const Renderable &other) {
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
     * A sortable Text for rendering
     */
    struct RenderableText : public Renderable {

        RenderableText(const std::string &text = "", const sf::Vector2f &position = sf::Vector2f(),
            const std::string &fontFile = "", const sf::Color &color = sf::Color::White,
            cmn::ERenderingLayer renderLayer = cmn::ERenderingLayer::NO_LAYER, int renderPriority = 0)
            : Renderable(renderLayer, renderPriority), font(new sf::Font()), text(new sf::Text()) {

            drawPtr.reset(this->text.get());

            if (!font->loadFromFile(fontFile)) {
                cerr << "Error: RenderableText failed to load font file <" + fontFile + ">" << endl;
                throw 1;
            }

            if (this->text) {
                this->text->setString(text);
                this->text->setColor(color);
                this->text->setPosition(position);
                this->text->setFont(*font);
            }
        }

        // A Font used to format text
        std::shared_ptr<sf::Font> font;

        // A Text used to draw text to the window
        std::shared_ptr<sf::Text> text;
    };

    /*
     * A base class for containing sf::Shape objects in a sortable format
     */
    struct RenderableShape : public Renderable {

        RenderableShape(const cmn::ERenderingLayer &renderLayer = cmn::ERenderingLayer::NO_LAYER, const int renderPriority = 0)
            : Renderable(renderLayer, renderPriority) {}

    };

    /*
     * A sortable Circle for rendering
     */
    struct RenderableCircle : public RenderableShape {

        RenderableCircle(const cmn::ERenderingLayer &renderLayer = cmn::ERenderingLayer::NO_LAYER, const int renderPriority = 0)
            : RenderableShape(renderLayer, renderPriority), circle(new sf::CircleShape()) {
        
            drawPtr.reset(circle.get());
        }

        std::shared_ptr<sf::CircleShape> circle;
    };

    /*
     * A sortable Rectangle for rendering
     */
    struct RenderableRectangle : public RenderableShape {

        RenderableRectangle(const cmn::ERenderingLayer &renderLayer = cmn::ERenderingLayer::NO_LAYER, const int renderPriority = 0)
            : RenderableShape(renderLayer, renderPriority), rectangle(new sf::RectangleShape()) {
        
            drawPtr.reset(rectangle.get());
        }

        std::shared_ptr<sf::RectangleShape> rectangle;
    };

    /*
     * A sortable Sprite for rendering and animation
     */
    struct RenderableSprite : public Renderable {

        RenderableSprite(const std::string &textureFileName = "",
            const std::string &animName = "", const int frameId = 0,
            const cmn::ERenderingLayer &renderLayer = cmn::ERenderingLayer::NO_LAYER, const int renderPriority = 0)
            : Renderable(renderLayer, renderPriority), textureFileName(textureFileName), animName(animName), 
            frameId(frameId), sprite(new sf::Sprite()) {
        
            drawPtr.reset(sprite.get());
        }

        // The source texture file for the current sprite(sheet)
        std::string textureFileName;

        // The name of the animation in use. Empty string ("") indicates no animation necessary
        std::string animName;

        // The index of the frame of the animation currently being displayed. Only significant if animation is necessary
        int frameId;

        // The sprite to be sorted
        std::shared_ptr<sf::Sprite> sprite;
    };


    struct Renderer : public ex::Component<Renderer>, public cmn::Serializable {

        Renderer() {

            //Implement details of XML serialization and deserialization here

        }

        // Maps a string name to a given Text to be rendered
        std::map<std::string, std::shared_ptr<RenderableText>> texts;

        // Maps a string name to a given Rectangle to be rendered
        std::map<std::string, std::shared_ptr<RenderableRectangle>> rectangles;

        // Maps a string name to a given Circle to be rendered
        std::map<std::string, std::shared_ptr<RenderableCircle>> circles;

        // Maps a string name to a given Sprite to be rendered
        std::map<std::string, std::shared_ptr<RenderableSprite>> sprites;
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

#pragma endregion

#pragma region Behaviors

    /*
    * This is a baseline Behavior component with functionality that many entities will likely reference.
    */
    struct CoreBehavior : public ex::Component<CoreBehavior> {

        CoreBehavior() : awake(nullptr), beginPlay(nullptr), onCollisionEnter(nullptr), onCollisionExit(nullptr),
            preUpdate(nullptr), postUpdate(nullptr) {}

        // Called when the entity instance is loaded
        void(*awake)();
        // Called when the game begins
        void(*beginPlay)();
        // Called if the entity has a Collider and has start colliding with another entity's Collider
        void(*onCollisionEnter)(ex::Entity &other);
        // Called if the entity has a Collider and has stopped colliding with another entity's Collider
        void(*onCollisionExit)(ex::Entity &other);
        // Called at the beginning of each frame before other System updates
        void(*preUpdate)(ex::TimeDelta dt);
        // Called at the end of each frame after other System updates
        void(*postUpdate)(ex::TimeDelta dt);

    };

#pragma endregion


}

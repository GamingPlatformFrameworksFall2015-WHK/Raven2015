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

#include <SFML/Graphics.hpp>            // For sf::Sprite
#include <SFML/Audio/Sound.hpp>         // For sf::Sound
#include <SFML/Audio/Music.hpp>         // For sf::Music
#include <SFML/Audio/SoundBuffer.hpp>   // For sf::SoundBuffer
#include <map>                          // For std::map
#include <string>                       // For std::string
#include "entityx\Entity.h"             // For ex::Component
#include "EntityLibrary.h"              // For Entity
#include "FunctionLibrary.h"            // For FL, FTL
#include "DataAssetLibrary.h"           // For Renderable, Timer
#include "Common.h"                     // For etc.

namespace Raven {

#pragma region Data

    struct Data : public ex::Component<Data>, public cmn::Serializable {
        std::string name;
        std::string prefabName;
        bool modified;

        virtual std::string serialize(std::string tab) override;

        virtual void deserialize(XMLNode* node) override;

        virtual std::string getElementName() override { return "Data"; }
    };

#pragma endregion

#pragma region Physics
    
    // A component enabling a passive physical state. Required for placement.
    struct Transform : public ex::Component<Transform>, public cmn::Serializable {

        // Default constructor
        Transform(const float transformX = 0.0f, const float transformY = 0.0f,
            const float rotation = 0.0f) : rotation(rotation) {

            transform.x = transformX;
            transform.y = transformY;
        }

        // The x and y coordinates of the entity's Origin.
        sf::Vector2f transform;

        // The orientation of the entity, measured in degrees.
        // Assumes that 0 begins at the right, running counterclockwise.
        float rotation;

        virtual std::string serialize(std::string tab) override;

        virtual void deserialize(XMLNode* node) override;

        virtual std::string getElementName() override { return "Transform"; }
    };

    // A component enabling a dynamic physical state. Required for movement.
    struct Rigidbody : public ex::Component<Rigidbody>, public cmn::Serializable {

        // Default Constructor. All fields initialized to zero.
        Rigidbody(const float velocityX = 0.0f, const float velocityY = 0.0f,
            const float accelerationX = 0.0f, const float accelerationY = 0.0f,
            const float radialVelocity = 0.0f) : radialVelocity(radialVelocity) {

            velocity.x = velocityX;
            velocity.y = velocityY;
            acceleration.x = accelerationX;
            acceleration.y = accelerationY;
        }
        
        // Initializes a new instance of the <see cref="Rigidbody"/> struct.
        Rigidbody(const sf::Vector2f &velocity, const sf::Vector2f &acceleration,
            const float radialVelocity = 0.0f) : velocity(velocity),
            acceleration(acceleration), radialVelocity(radialVelocity) {}

        // The x and y components of the entity's current velocity.
        sf::Vector2f velocity;

        // The x and y components of the entity's current acceleration.
        sf::Vector2f acceleration;

        // The turning rate of the entity in degrees per second, counterclockwise.
        float radialVelocity;

        virtual std::string serialize(std::string tab) override;

        virtual void deserialize(XMLNode* node) override;

        virtual std::string getElementName() override { return "Rigidbody"; }
    };
    
    // An abstract component used to identify collision areas.
    // The originOffset's x and y values are relative to its Transform.
    struct Collider : public ex::Component<Collider> {
        // Initializes a new instance of the <see cref="Collider"/> struct.
        Collider() {}
                
        // Abstract definition for virtual destructor to finalize an instance of 
        // the abstract Collider class.
        virtual ~Collider() = 0;

        // The x-y coordinate of the collider's center relative to its transform.
        sf::Vector2f originOffset;
    };
        
    // Inline empty implementation of Collider's pure virtual deconstructor
    inline Collider::~Collider() {}
    
    // A Collider with a box-shaped collision area
    struct BoxCollider : public Collider, public cmn::Serializable {

         // Initializes a new instance of the <see cref="BoxCollider"/> struct.
        BoxCollider(const float width = cmn::STD_UNITX,
            const float height = cmn::STD_UNITY,
            const float x = 0.0f, const float y = 0.0f)
            : width(width), height(height) {

            originOffset.x = x;
            originOffset.y = y;
        }
        
        // Initializes a new scaled instance of the <see cref="BoxCollider"/> struct.
        BoxCollider(const float scale, const float x = 0.0f,
            const float y = 0.0f)
            : BoxCollider(cmn::STD_UNITX, cmn::STD_UNITY, x, y) {
            width *= scale;
            height *= scale;
        }
        
        // Finalizes an instance of the <see cref="BoxCollider"/> class.
        virtual ~BoxCollider() override {}
        
        // The range of the x-axis of the collider. Origin in the middle.
        float width;
        
        // The range of the y-axis of the collider. Origin in the middle.
        float height;
        
        // The set of layers to which the collider is assigned. 
        // The collider will "collide" with any BoxCollider that possesses the same entry.
        // The value is a boolean pair of options dictating when CollisionEvents are emitted.
        //     First="Required". Second="Automatic".
        // "Required" indicates that if the layer IS NOT matched up, no CollisionEvent will be emitted.
        // "Automatic" indicates that if the layer IS matched up, a CollisionEvent will immediately be emitted.
        // An "automatic" setting will override any lack of "required" layers
        std::map<std::string, std::pair<bool, bool>> layers;
        
        // The collision settings. Valid values can be found in Common::CollisionLayerSettings
        // SOLID : The layer that indicates the entities should be "pushed out of each other"
        // TRIGGER : The layer that indicates the entity will react to the collision
        std::set<std::string> collisionSettings;

        virtual std::string serialize(std::string tab) override;

        virtual void deserialize(XMLNode* node) override;

        virtual std::string getElementName() override { return "BoxCollider"; }
    };
    
#pragma endregion

#pragma region Audio

    // A component that acts as a base class for audio-storage data
    struct AudioMaker : public ex::Component<AudioMaker> {};

    // A component that stores the names of small tracks (< 30 seconds) for sf::Sound.
    struct SoundMaker : public AudioMaker, public cmn::Serializable {

        SoundMaker()  {}

        // A mapping between sound file names and the buffers for their storage
        SOUNDMAP_T soundMap;

        // An object for performing sound operations on a buffer.
        sf::Sound sound;

        virtual std::string serialize(std::string tab) override;

        virtual void deserialize(XMLNode* node) override;

        virtual std::string getElementName() override { return "SoundMaker"; }
    };

    // A component that stores the names of large tracks (~30 seconds+) for sf::Music.
    struct MusicMaker : public AudioMaker, public cmn::Serializable {

        MusicMaker() {}

        // A mapping between music file names and their stream storage objects
        MUSICMAP_T musicMap;

        virtual std::string serialize(std::string tab) override;

        virtual void deserialize(XMLNode* node) override;

        virtual std::string getElementName() override { return "MusicMaker"; }
    };

#pragma endregion

#pragma region Rendering


    // A component used to store renderable assets. Each asset is mapped by a name
    struct Renderer : public ex::Component<Renderer>, public cmn::Serializable {

        Renderer() {}

        // Maps a string name to a given Text to be rendered        
        std::map<std::string, std::shared_ptr<RenderableText>> texts;

        // Maps a string name to a given Rectangle to be rendered        
        std::map<std::string, std::shared_ptr<RenderableRectangle>> rectangles;

        // Maps a string name to a given Circle to be rendered        
        std::map<std::string, std::shared_ptr<RenderableCircle>> circles;

        // Maps a string name to a given Sprite to be rendered        
        std::map<std::string, std::shared_ptr<RenderableSprite>> sprites;

        virtual std::string serialize(std::string tab) override;

        virtual void deserialize(XMLNode* node) override;

        virtual std::string getElementName() override { return "Renderer"; }
    };

#pragma endregion

#pragma region Timers

    struct TimeTable : public ex::Component<TimeTable>, public cmn::Serializable {        
        // Initializes a new instance of the <see cref="TimeTable"/> struct.
        TimeTable() {}
        
        // Maps a name to current timers
        std::map<std::string, Timer> timerMap;

        virtual std::string serialize(std::string tab) override;

        virtual void deserialize(XMLNode* node) override;

        virtual std::string getElementName() override { return "TimeTable"; }
    };

#pragma endregion


#pragma region Behaviors

    // A baseline Behavior component with functionality that many entities will likely reference.
    struct CoreBehavior : public ex::Component<CoreBehavior>, public cmn::Serializable {
        
        // Initializes a new instance of the <see cref="CoreBehavior"/> struct.
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

        virtual std::string serialize(std::string tab) override;

        virtual void deserialize(XMLNode* node) override;

        virtual std::string getElementName() override { return "CoreBehavior"; }

    };

#pragma endregion

#pragma region ActionListener
    
    // A class designed to "listen" for specific actions and provide a response for the entity
    struct ActionListener : ex::Component<ActionListener>, public cmn::Serializable, public ex::Receiver<ActionListener> {
        
        // Initializes a new instance of the <see cref="ActionListener"/> struct.
        ActionListener(int pid = -1) : playerId(pid) {}
        
        // Maps string actions to a reactionary function. The "entity" is assumed to be the owner 
        std::map<std::string, Function> actionMap;
        
        // The player identifier. Assumed to be non-negative. -1 indicates "null" value
        int playerId;

        virtual std::string serialize(std::string tab) override;

        virtual void deserialize(XMLNode* node) override;

        virtual std::string getElementName() override { return "ActionListener"; }
    };

#pragma endregion

#pragma region ComponentLibrary

    class ComponentLibrary {
    public:
        const static std::string NULL_COMPONENT;
        const static std::string DATA;
        const static std::string TRANSFORM;
        const static std::string RIGIDBODY;
        const static std::string BOX_COLLIDER;
        const static std::string SOUND_MAKER;
        const static std::string MUSIC_MAKER;
        const static std::string RENDERER;
        const static std::string TIME_TABLE;
        const static std::string CORE_BEHAVIOR;
        const static std::string ACTION_LISTENER;

        static ex::Entity attachComponent(ex::Entity newOwner, std::string componentName) {
            if (componentName == DATA) {
                newOwner.assign<Data>();
            }
            else if (componentName == TRANSFORM) {
                newOwner.assign<Transform>();
            }
            else if (componentName == RIGIDBODY) {
                newOwner.assign<Rigidbody>();
            }
            else if (componentName == BOX_COLLIDER) {
                newOwner.assign<BoxCollider>();

            }
            else if (componentName == SOUND_MAKER) {
                newOwner.assign<SoundMaker>();

            }
            else if (componentName == MUSIC_MAKER) {
                newOwner.assign<MusicMaker>();

            }
            else if (componentName == RENDERER) {
                newOwner.assign<Renderer>();

            }
            else if (componentName == TIME_TABLE) {
                newOwner.assign<TimeTable>();
            }
            else if (componentName == CORE_BEHAVIOR) {
                newOwner.assign<CoreBehavior>();
            }
            else if (componentName == ACTION_LISTENER) {
                newOwner.assign<ActionListener>();
            }
            return newOwner;
        }
    };

#pragma endregion

}

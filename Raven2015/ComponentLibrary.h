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
#include "DataAssetLibrary.h"           // For Renderable, Timer
#include "Common.h"                     // For etc.


namespace Raven {

// Helper macros /*************************************************************************************************/
#define SERIALIZE_COMPONENT(type_name, e, str) auto a##type_name = e.component<type_name>(); str + (a##type_name ? a##type_name->serialize(tab) : "");
#define DESERIALIZE_COMPONENT(type_name, e, node) auto a##type_name = node->FirstChildElement(#type_name); a##type_name ? e.component<type_name>()->deserialize(a##type_name) : nullptr;
/******************************************************************************************************************/

// Update-Necessary Macros : altering the types of components that exist requires that the user update these macros
#define COMPONENT_TYPES(t) Data##_t, Transform##_t, Rigidbody##_t, BoxCollider##_t, SoundMaker##_t, MusicMaker##_t, Renderer##_t, TimeTable##_t
#define SERIALIZE_COMPONENTS(e, str) \
            SERIALIZE_COMPONENT(Data, e, str); \
            SERIALIZE_COMPONENT(Transform, e, str); \
            SERIALIZE_COMPONENT(Rigidbody, e, str); \
            SERIALIZE_COMPONENT(BoxCollider, e, str); \
            SERIALIZE_COMPONENT(SoundMaker, e, str); \
            SERIALIZE_COMPONENT(MusicMaker, e, str); \
            SERIALIZE_COMPONENT(Renderer, e, str); \
            SERIALIZE_COMPONENT(TimeTable, e, str);
#define DESERIALIZE_COMPONENTS(e, node) \
            DESERIALIZE_COMPONENT(Data, e, node); \
            DESERIALIZE_COMPONENT(Transform, e, node); \
            DESERIALIZE_COMPONENT(Rigidbody, e, node); \
            DESERIALIZE_COMPONENT(BoxCollider, e, node); \
            DESERIALIZE_COMPONENT(SoundMaker, e, node); \
            DESERIALIZE_COMPONENT(MusicMaker, e, node); \
            DESERIALIZE_COMPONENT(Renderer, e, node); \
            DESERIALIZE_COMPONENT(TimeTable, e, node);
/******************************************************************************************************************/
    enum ComponentType {
        COMPONENT_TYPES(_t)
    };

#define ADD_STATICS(type_name) \
        static std::string getElementName() { return #type_name; } \
        static ComponentType getType() { return ComponentType::type_name##_t; }

#pragma region Data

    struct Data : public ex::Component<Data>, public cmn::Serializable {
        std::string name;
        std::string prefabName;
        bool modified;

        virtual std::string serialize(std::string tab) override;
        virtual void deserialize(XMLNode* node) override;
        ADD_STATICS(Data);
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
        ADD_STATICS(Transform);
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
        ADD_STATICS(Rigidbody);
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
        ADD_STATICS(BoxCollider);
    };
    
#pragma endregion

#pragma region Audio

    // A component that acts as a base class for audio-storage data
    struct AudioMaker {};

    // A component that stores the names of small tracks (< 30 seconds) for sf::Sound.
    struct SoundMaker : public ex::Component<SoundMaker>, public AudioMaker, public cmn::Serializable {

        SoundMaker()  {}

        // A mapping between sound file names and the buffers for their storage
        SOUNDMAP_T soundMap;

        // An object for performing sound operations on a buffer.
        sf::Sound sound;

        virtual std::string serialize(std::string tab) override;
        virtual void deserialize(XMLNode* node) override;
        ADD_STATICS(SoundMaker);
    };

    // A component that stores the names of large tracks (~30 seconds+) for sf::Music.
    struct MusicMaker : public ex::Component<MusicMaker>, public AudioMaker, public cmn::Serializable {

        MusicMaker() {}

        // A mapping between music file names and their stream storage objects
        MUSICMAP_T musicMap;

        virtual std::string serialize(std::string tab) override;
        virtual void deserialize(XMLNode* node) override;
        ADD_STATICS(MusicMaker);
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
        ADD_STATICS(Renderer);
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
        ADD_STATICS(TimeTable);
    };

#pragma endregion

#pragma region Behaviors

    // Behaviors

#pragma endregion

#pragma region ComponentLibrary



    // This struct enables us to organize the serialization of components more easily
    // Now we can acquire the Data component of an entity, cycle through its components set
    // and call attachComponent as necessary
    struct ComponentLibrary {

        static std::string serializeEntity(ex::Entity e, std::string tab);

        static void deserializeEntity(ex::Entity e, XMLNode* node);
    };

#pragma endregion

}

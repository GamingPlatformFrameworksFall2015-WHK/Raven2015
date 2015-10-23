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
#include "Common.h"                     // For etc.
#include "DataAssetLibrary.h"           // For Renderable, Timer


namespace Raven {

    // Helper macros /*************************************************************************************************/
#define SERIALIZE_COMPONENT(type_name, e, str) auto a##type_name = e.component<type_name>(); str + (a##type_name ? a##type_name->serialize(tab) : "");
#define DESERIALIZE_COMPONENT(type_name, e, node) auto a##type_name = node->FirstChildElement(#type_name); a##type_name ? e.component<type_name>()->deserialize(a##type_name) : nullptr;
/******************************************************************************************************************/

/**** Update-Necessary Macros : altering the types of components that exist requires that the user update these macros *****/

// Used to instantiate the ComponentType enum
#define COMPONENT_TYPES(_t) Data##_t, Transform##_t, Rigidbody##_t, BoxCollider##_t, Pawn##_t, Tracker##_t, Pacer##_t, SoundMaker##_t, MusicMaker##_t, Renderer##_t, TimeTable##_t
// Used to pass into templated lists for acquiring all component types
#define COMPONENT_TYPE_LIST Data, Transform, Rigidbody, BoxCollider, Pawn, Tracker, Pacer, SoundMaker, MusicMaker, Renderer, TimeTable
// Used to exploit recursive parameter packs for iterating through all components on a given entity, regardless of type
// Must provide the actual instance of the component type so that the typename T exploited is the original type and not a pointer to the type or some such
#define COMPONENTS_OF_ENTITY(e) \
            *e.component<Data>().get(), \
            *e.component<Transform>().get(), \
            *e.component<Rigidbody>().get(), \
            *e.component<BoxCollider>().get(), \
            *e.component<Pawn>().get(), \
            *e.component<Tracker>().get(), \
            *e.component<Pacer>().get(), \
            *e.component<SoundMaker>().get(), \
            *e.component<MusicMaker>().get(), \
            *e.component<Renderer>().get(), \
            *e.component<TimeTable>().get() //<- an actual TimeTable, not a TimeTable* or ex::ComponentHandle<TimeTable>, etc.
// Used to serialize each component
#define SERIALIZE_COMPONENTS(e, str) \
            SERIALIZE_COMPONENT(Data, e, str); \
            SERIALIZE_COMPONENT(Transform, e, str); \
            SERIALIZE_COMPONENT(Rigidbody, e, str); \
            SERIALIZE_COMPONENT(BoxCollider, e, str); \
            SERIALIZE_COMPONENT(Pawn, e, str); \
            SERIALIZE_COMPONENT(Tracker, e, str); \
            SERIALIZE_COMPONENT(Pacer, e, str); \
            SERIALIZE_COMPONENT(SoundMaker, e, str); \
            SERIALIZE_COMPONENT(MusicMaker, e, str); \
            SERIALIZE_COMPONENT(Renderer, e, str); \
            SERIALIZE_COMPONENT(TimeTable, e, str);
// Used to deserialize each component
#define DESERIALIZE_COMPONENTS(e, node) \
            DESERIALIZE_COMPONENT(Data, e, node); \
            DESERIALIZE_COMPONENT(Transform, e, node); \
            DESERIALIZE_COMPONENT(Rigidbody, e, node); \
            DESERIALIZE_COMPONENT(BoxCollider, e, node); \
            DESERIALIZE_COMPONENT(Pawn, e, node); \
            DESERIALIZE_COMPONENT(Tracker, e, node); \
            DESERIALIZE_COMPONENT(Pacer, e, node); \
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

        Data(std::string entityName = "Default_Entity", std::string prefabName = "", bool modified = false) :
            name(entityName), prefabName(prefabName), modified(modified) {}

        // Copy Constructor
        Data(const Data& other) : name(other.name), prefabName(other.prefabName), modified(other.modified) {}

        // The displayed name for this entity
        std::string name;
        // The name of the prefab
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

        // Default constructor. All fields initialzied to zero
        Transform(const float transformX = 0.0f, const float transformY = 0.0f,
            const float rotation = 0.0f) : rotation(rotation) {

            transform.x = transformX;
            transform.y = transformY;
        }

        // Copy Constructor
        Transform(const Transform& other) : transform(other.transform), rotation(other.rotation) {}

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

        // Copy Constructor
        Rigidbody(const Rigidbody& other) : velocity(other.velocity), 
            acceleration(other.acceleration), radialVelocity(other.radialVelocity) {}

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
        
        // Copy Constructor
        BoxCollider(const BoxCollider& other) : width(other.width), height(other.height), 
            layers(other.layers), collisionSettings(other.collisionSettings) {}
        
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
        // FIXED : The layer that indicates the entity will react to the collision
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

        // Null Constructor
        SoundMaker()  {}

        // Copy Constructor
        SoundMaker(const SoundMaker& other) : soundMap(other.soundMap), sound(other.sound) {}

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

        // Null Constructor
        MusicMaker() {}

        // Copy Constructor
        MusicMaker(const MusicMaker& other) : musicMap(other.musicMap) {}

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

        // Null Constructor
        Renderer() {}

        // Copy Constructor
        Renderer(const Renderer& other) {
            for (auto name_item : other.texts) {
                texts.insert(std::make_pair(name_item.first, 
                    std::shared_ptr<RenderableText>(new RenderableText(*name_item.second.get()))));
        }
            for (auto name_item : other.rectangles) {
                rectangles.insert(std::make_pair(name_item.first, 
                    std::shared_ptr<RenderableRectangle>(new RenderableRectangle(*name_item.second.get()))));
            }
            for (auto name_item : other.circles) {
                circles.insert(std::make_pair(name_item.first, 
                    std::shared_ptr<RenderableCircle>(new RenderableCircle(*name_item.second.get()))));
                }
            for (auto name_item : other.sprites) {
                sprites.insert(std::make_pair(name_item.first, 
                    std::shared_ptr<RenderableSprite>(new RenderableSprite(*name_item.second.get()))));
            }
        }

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
        
        // Copy Constructor
        TimeTable(const TimeTable& other) : timerMap(other.timerMap) {}
        
        // Maps a name to current timers
        std::map<std::string, Timer> timerMap;

        virtual std::string serialize(std::string tab) override;
        virtual void deserialize(XMLNode* node) override;
        ADD_STATICS(TimeTable);
    };

#pragma endregion

#pragma region Behaviors

    // An abstract component used to classify player objects
    struct Pawn : public ex::Component<Pawn>, public cmn::Serializable {
        // Creates new instance of struct
        Pawn() : playerId(initId()) {
            if (playerId == -1) {
                cerr << "WARNING: Pawn component generated with automatic invalid player ID."
                    " This Pawn will not respond to input." << endl;
            }
            else {
                Pawn::ids.set(playerId);
            }
        }

        Pawn(int playerId) {
            if (!ids.test(playerId)) {
                this->playerId = playerId;
                ids.set(playerId);
            }
            else {
                cerr << "WARNING: Could not assign playerID " + std::to_string(playerId) +
                    " to the given entity." << endl;
                int i = Pawn::initId();
                if (i == -1) {
                    cerr << "WARNING: Could not assign a default ID to the given entity" << endl;
                }
                else {
                    playerId = i;
                    Pawn::ids.set(i);
                }
            }
        }

        // Copy Constructor
        Pawn(const Pawn& other)  {}

        ~Pawn() { ids.reset(playerId); }

        int playerId;

        static std::bitset<4> ids;

        static int initId() { return ids.test(0) ? 0 : ids.test(1) ? 1 : ids.test(2) ? 2 : ids.test(3) ? 3 : -1; }

        //Serialization and deserialization for edit/play mode
        virtual std::string serialize(std::string tab) override;
        virtual void deserialize(XMLNode* node) override;
        ADD_STATICS(Pawn);
    };

    // An abstract component used to classify AI objects
    // Tracker will follow closest pawn (player) object
    struct Tracker : public ex::Component<Tracker>, public cmn::Serializable {
        // Creates new instance of struct
        Tracker(ComponentType type = ComponentType::Pawn_t) : target(type) {}

        // Copy Constructor
        Tracker(const Tracker& other) : target(other.target) {}

        ComponentType target;

        //Serialization and deserialization for edit/play mode
        virtual std::string serialize(std::string tab) override;
        virtual void deserialize(XMLNode* node) override;
        ADD_STATICS(Tracker);
    };

    // An abstract component used to classify AI objects
    // Pacer will continuously pace with a horizontal(HOR_PATH),
    // vertical(VERT_PATH), or diagonal(DIAG_PATH), for a given
    // movement radius.
    struct Pacer : public ex::Component<Pacer>, public cmn::Serializable {
        // Creates new instance of struct
        Pacer(std::string direction, sf::Vector2f origin, float radius) 
            : direction(direction), origin(origin), radius(radius) {

            // Vertical path will only have a velocity in the y direction
            if (direction == VERT_PATH) {
                velocity.x = 0.f;
                velocity.y = 0.1f;
            }
            // Horizontal path will only have velocity in the x direction
            else if (direction == HOR_PATH) {
                velocity.x = 0.1f;
                velocity.y = 0.f;
            }
            // Diagonal path will have both x and y velocities
            else {
                velocity.x = 0.1f;
                velocity.y = 0.1f;
            }
        }

        // Copy Constructor
        Pacer(const Pacer& other) {
            direction = other.direction;
            velocity = other.velocity;
        }

        // Vertical(VERT_PATH), Horizontal(HOR_PATH), Diagonal(DIAG_PATH)
        std::string direction;

        // Velocity of Pacer that will be passed to entitie's rigidbody upon update()
        sf::Vector2f velocity;

        // Center point of pacer's path
        sf::Vector2f origin;

        // Distance out in either direction that pacer willtravel from its center point
        float radius;

        //Serialization and deserialization for edit/play mode
        virtual std::string serialize(std::string tab) override;
        virtual void deserialize(XMLNode* node) override;
        ADD_STATICS(Pacer);
    };
    

#pragma endregion

#pragma region ComponentLibrary

    // This struct enables us to organize the serialization of components more easily
    // Now we can acquire the Data component of an entity, cycle through its components set
    // and call attachComponent as necessary
    struct ComponentLibrary {

    };

#pragma endregion

}

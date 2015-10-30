#pragma once

#include "Common.h"
#include "entityx/System.h"
#include "ComponentLibrary.h"
#include "EntityLibrary.h"
#include "EventLibrary.h"
#include "WidgetLibrary.h"
#include <map>

namespace Raven {

    class XMLSystem : public ex::System<XMLSystem>, public ex::Receiver<XMLSystem> {
    public:
        XMLSystem();
        ~XMLSystem();

        // Does nothing
        void update(ex::EntityManager &es, ex::EventManager &events,
            ex::TimeDelta dt) override {}

        void configure(ex::EventManager& event_manager) {
            event_manager.subscribe<XMLSaveEvent>(*this);
            event_manager.subscribe<XMLLoadEvent>(*this);
            event_manager.subscribe<XMLUpdateEntityNameEvent>(*this);
        }

        // Upon reception of an XMLLoadEvent, the system will de-serialize the XMLDocument and reinstate the previous game state
        void receive(const XMLLoadEvent& e);
        // Upon reception of an XMLSaveEvent, the system will serialize the XMLDocument and preserve the current game state
        void receive(const XMLSaveEvent& e);

        // For display purposes in the GUISystem
        std::string getAssetNameFromFilePath(std::string assetFilePath, bool includeExtension);

        // Game (De)Serialization
        void serializeRavenGame();
        void deserializeRavenGame();

        /////////////// Saving & Loading  //////////////
        bool saveAssets();
        // currently inefficient. Will re-write the entire prefabs xml document
        // Do not have a method for inserting / removing / re-arranging a single prefab
        bool savePrefabs(); 
        bool saveLevel(std::string levelFilePath);
        bool loadAssets();
        // (designed for possible dynamic level-streaming)
        bool loadLevel(std::string levelFilePath, sf::Vector2f levelOffset, bool clearEntitiesBeforehand);

        // Entity / Component (De)Serialization
        std::string serializeEntity(ex::Entity e, std::string tab);
        void deserializeEntity(ex::Entity e, XMLNode* node);
        // Confirms whether the named prefab can be found in prefabs.xml
        bool prefabExists(std::string prefabName);
        // Instantiates the named prefab. Returns nullptr if the prefab is not found in prefabs.xml
        // Same as prefabExists, but proceeds to deserialize the found prefab.
        std::shared_ptr<ex::Entity> instantiate(std::string prefabName);

        // Responds to a request to update a given entity's name in either the levelMap/levelDoc or prefabsDoc
        void receive(const XMLUpdateEntityNameEvent& event);

        // The serialization document tracking assets
        XMLDocument assetsDoc;
        // The serialization document tracking prefabs
        XMLDocument prefabsDoc;
        // The serialization document tracking entities currently in existence
        // Leaving the definition open for possible level-streaming later on
        XMLDocument levelDoc;

        // Maintains the set of texture file paths
        std::set<std::string> textureFilePathSet;
        // Maintains the set of music file paths
        std::set<std::string> musicFilePathSet;
        // Maintains the set of sound file paths
        std::set<std::string> soundFilePathSet;
        // Maintains the set of font file paths
        std::set<std::string> fontFilePathSet;
        // Maps the user-defined asset name to the Animation
        std::map<std::string, std::shared_ptr<Animation>> animationMap;
        // Maps the user-defined asset name to the RenderableText
        std::map<std::string, std::shared_ptr<RenderableText>> renderableTextMap;
        // Maps the user-defined asset name to the RenderableRectangle
        std::map<std::string, std::shared_ptr<RenderableRectangle>> renderableRectangleMap;
        // Maps the user-defined asset name to the RenderableCircle
        std::map<std::string, std::shared_ptr<RenderableCircle>> renderableCircleMap;
        // Maps the user-defined asset name to the RenderableSprite
        std::map<std::string, std::shared_ptr<RenderableSprite>> renderableSpriteMap;
        // Maps the user-defined asset name to a given prefab
        //std::map<std::string, std::shared_ptr<ex::Entity>> prefabMap;
        // Maintains the set of level file paths
        std::set<std::string> levelFilePathSet;
        // Maps the user-defined asset name to a given entity instance
        std::map<std::string, std::shared_ptr<ex::Entity>> levelMap;

    private:
        ///////////////// Asset Serialization ///////////////
        std::string serializeAssets();
        std::string serializeTextureFilePathSet(std::string tab);
        std::string serializeMusicFilePathSet(std::string tab);
        std::string serializeSoundFilePathSet(std::string tab);
        std::string serializeFontFilePathSet(std::string tab);
        std::string serializeLevelFilePathSet(std::string tab);
        std::string serializeAnimationMap(std::string tab);
        std::string serializeRenderableTextMap(std::string tab);
        std::string serializeRenderableRectangleMap(std::string tab);
        std::string serializeRenderableCircleMap(std::string tab);
        std::string serializeRenderableSpriteMap(std::string tab);

        //////////////// Asset Deserialization //////////////
        void deserializeAssets(XMLNode* node);
        void deserializeTextureFilePathSet(XMLNode* node);
        void deserializeMusicFilePathSet(XMLNode* node);
        void deserializeSoundFilePathSet(XMLNode* node);
        void deserializeFontFilePathSet(XMLNode* node);
        void deserializeLevelFilePathSet(XMLNode* node);
        void deserializeAnimationMap(XMLNode* node);
        void deserializeRenderableTextMap(XMLNode* node);
        void deserializeRenderableRectangleMap(XMLNode* node);
        void deserializeRenderableCircleMap(XMLNode* node);
        void deserializeRenderableSpriteMap(XMLNode* node);

        // LevelMap (De)Serialization
        std::string serializeLevelMap();
        void deserializeLevel(XMLNode* node, sf::Vector2f levelOffset, bool clearEntitiesBeforehand);

        /////////////// (De)Serialization Utility Methods //////////////////
        std::string XMLSystem::serializeFilePathSet(std::set<std::string> filePathSet, std::string wrapperElement, std::string tab);
        void XMLSystem::deserializeFilePathSet(std::set<std::string> filePathSet, std::string wrapperElement, XMLNode* node);
        std::string getXMLHeader(std::string topLevelElement, std::string fileName);
        XMLElement* findXMLEntity(XMLNode* top, std::string entityName);
        template <typename C>
        std::string serializeEntityComponents(ex::Entity e, std::string tab, C* c);
        template <typename C, typename... Components>
        std::string serializeEntityComponents(ex::Entity e, std::string tab, C* c, Components*... components);
        template <typename C>
        void deserializeEntityComponents(ex::Entity e, XMLNode* node, bool firstCall, C* c);
        template <typename C, typename... Components>
        void deserializeEntityComponents(ex::Entity e, XMLNode* node, bool firstCall, C* c, Components*... components);

        //////////////// Constants //////////////////////////
        const std::string newline = "\r\n";
        const std::string xmlVersion = "1.0";
        const std::string xmlEncoding = "utf-8";
        const std::string xmlExt = ".xml";
        const std::string dtdExt = ".dtd";
        const std::string assetsFileName = "assets" + xmlExt;
        const std::string assetsDesignFileName = "assets" + dtdExt;
        const std::string assetsFirstChildElement = "ASSETS";
        const std::string prefabsFileName = "prefabs" + xmlExt;
        const std::string prefabsDesignFileName = "prefabs" + dtdExt;
        const std::string prefabsFirstChildElement = "PREFABS";
        const std::string levelDesignFileName = "level" + dtdExt;
        const std::string levelFirstChildElement = "LEVEL";
    };

}

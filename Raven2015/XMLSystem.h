#pragma once

#include "Common.h"
#include "entityx/System.h"
#include "ComponentLibrary.h"
#include "EventLibrary.h"
#include <map>

namespace Raven {

    class XMLSystem : public ex::System<XMLSystem>, public ex::Receiver<XMLSystem> {
    public:
        XMLSystem();
        ~XMLSystem();

        // Does nothing
        void update(ex::EntityManager &es, ex::EventManager &events,
            ex::TimeDelta dt) override {}

        void receive(const XMLLoadEvent& e);
        void receive(const XMLSaveEvent& e);

        std::string getAssetNameFromFilePath(std::string assetFilePath, bool includeExtension);

        XMLDocument doc;

        static const std::string xmlFileName;

        // Maintains the set of asset file paths
        std::set<std::string> textureFilePathSet;
        // Maps the user-defined asset name to the file path
        std::set<std::string> musicFilePathSet;
        // Maps the user-defined asset name to the file path
        std::set<std::string> soundFilePathSet;
        // Maps the user-defined asset name to the file path
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
        // Essentially maps a name to an entity. All entities in the list are prefabs
        std::map<std::string, std::shared_ptr<ex::Entity>> prefabMap;
        // Maps the user-defined level name to a given sequence of named Entities 
        // Essentially maps a level name to a set of named entities that may or may not be prefabs
        std::map<std::string, std::map<std::string, std::shared_ptr<ex::Entity>>> levelMap;

    private:
        std::string serializeTextureFilePathSet(std::string tab);
        std::string serializeMusicFilePathSet(std::string tab);
        std::string serializeSoundFilePathSet(std::string tab);
        std::string serializeFontFilePathSet(std::string tab);
        std::string serializeAnimationMap(std::string tab);
        std::string serializeRenderableTextMap(std::string tab);
        std::string serializeRenderableRectangleMap(std::string tab);
        std::string serializeRenderableCircleMap(std::string tab);
        std::string serializeRenderableSpriteMap(std::string tab);
        std::string serializePrefabMap(std::string tab);
        std::string serializeLevelMap(std::string tab);

        void deserializeTextureFilePathSet(XMLNode* node);
        void deserializeMusicFilePathSet(XMLNode* node);
        void deserializeSoundFilePathSet(XMLNode* node);
        void deserializeFontFilePathSet(XMLNode* node);
        void deserializeAnimationMap(XMLNode* node);
        void deserializeRenderableTextMap(XMLNode* node);
        void deserializeRenderableRectangleMap(XMLNode* node);
        void deserializeRenderableCircleMap(XMLNode* node);
        void deserializeRenderableSpriteMap(XMLNode* node);
        void deserializePrefabMap(XMLNode* node);
        void deserializeLevelMap(XMLNode* node);

        std::string serializeRavenGame();
        void deserializeRavenGame();

        std::string XMLSystem::serializeFilePathSet(std::set<std::string> filePathSet, std::string wrapperElement, std::string tab);
        void XMLSystem::deserializeFilePathSet(std::set<std::string> filePathSet, XMLNode* node);
    };

}

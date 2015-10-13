#pragma once

#include "Common.h"
#include "entityx/System.h"
#include "ComponentLibrary.h"
#include <map>

namespace Raven {

    class XMLSystem :
        public ex::System<XMLSystem>
    {
    public:
        XMLSystem();
        ~XMLSystem();

        // Maps the user-defined asset name to the file path
        std::map<std::string, std::string> textureFileMap;
        // Maps the user-defined asset name to the file path
        std::map<std::string, std::string> musicFileMap;
        // Maps the user-defined asset name to the file path
        std::map<std::string, std::string> soundFileMap;
        // Maps the user-defined asset name to the file path
        std::map<std::string, std::string> fontFileMap;
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

        std::string serializeTextureFileMap(std::string tab);
        std::string serializeMusicFileMap(std::string tab);
        std::string serializeSoundFileMap(std::string tab);
        std::string serializeFontFileMap(std::string tab);
        std::string serializeAnimationMap(std::string tab);
        std::string serializeRenderableTextMap(std::string tab);
        std::string serializeRenderableRectangleMap(std::string tab);
        std::string serializeRenderableCircleMap(std::string tab);
        std::string serializeRenderableSpriteMap(std::string tab);
        std::string serializePrefabMap(std::string tab);
        std::string serializeLevelMap(std::string tab);

        void deserializeTextureFileMap(XMLNode* node);
        void deserializeMusicFileMap(XMLNode* node);
        void deserializeSoundFileMap(XMLNode* node);
        void deserializeFontFileMap(XMLNode* node);
        void deserializeAnimationMap(XMLNode* node);
        void deserializeRenderableTextMap(XMLNode* node);
        void deserializeRenderableRectangleMap(XMLNode* node);
        void deserializeRenderableCircleMap(XMLNode* node);
        void deserializeRenderableSpriteMap(XMLNode* node);
        void deserializePrefabMap(XMLNode* node);
        void deserializeLevelMap(XMLNode* node);

        std::string serializeEntitiesHelper(std::map<std::string, std::shared_ptr<ex::Entity>> map, std::string tab,
            bool checkForPrefabs);
        void deserializeEntitiesHelper(std::map<std::string, std::shared_ptr<ex::Entity>> map, XMLNode* node,
            bool checkForPrefabs);

        std::string getTextureName(std::string textureFileName);
        std::string getMusicName(std::string musicFileName);
        std::string getSoundName(std::string soundFileName);
        std::string getFontName(std::string fontFileName);

    };

}

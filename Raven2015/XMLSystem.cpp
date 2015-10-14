#include "XMLSystem.h"

namespace Raven {

    XMLSystem::XMLSystem() {}

#pragma region Serialization

    std::string XMLSystem::serializeTextureFilePathSet(std::string tab) {
        return serializeFilePathSet(textureFilePathSet, "Textures", tab);
    }
    std::string XMLSystem::serializeMusicFilePathSet(std::string tab) {
        return serializeFilePathSet(musicFilePathSet, "Music", tab);
    }
    std::string XMLSystem::serializeSoundFilePathSet(std::string tab) {
        return serializeFilePathSet(soundFilePathSet, "Sounds", tab);
    }
    std::string XMLSystem::serializeFontFilePathSet(std::string tab) {
        return serializeFilePathSet(fontFilePathSet, "Fonts", tab);
    }
    std::string XMLSystem::serializeAnimationMap(std::string tab) {
        std::string animationContent = "";
        for (auto animation : animationMap) {
            animationContent +=
                tab + "  <Item Name=\"" + animation.first + "\">\r\n" +
                tab + "    <TextureFilePath>" + animation.second->textureFileName + "</TextureFilePath>\r\n" +
                tab + "    <FrameCount>" + std::to_string(animation.second->size) + "</FrameCount>\r\n" +
                tab + "    <FrameWidth>" + std::to_string(animation.second->frameWidth) + "</FrameWidth>\r\n" +
                tab + "    <FrameHeight>" + std::to_string(animation.second->frameHeight) + "</FrameHeight>\r\n" +
                tab + "    <AnimationSpeed>" + std::to_string(animation.second->animationSpeed) + "</AnimationSpeed>\r\n" +
                tab + "    <IsLooping>" + std::to_string(animation.second->isLooping) + "</IsLooping>\r\n" +
                tab + "  </Item>\r\n";
        }

        return
            tab + "<Animations>\r\n" +
            animationContent +
            tab + "</Animations>\r\n";
    }
    std::string XMLSystem::serializeRenderableTextMap(std::string tab) {
        std::string renderableTextContent = "";
        for (auto renderableText : renderableTextMap) {
            renderableTextContent +=
                tab + "  <Item Name=\"" + renderableText.first + "\">\r\n" +
                tab + "    <RenderLayer>" + std::to_string(renderableText.second->renderLayer) + "</RenderLayer>\r\n" +
                tab + "    <RenderPriority>" + std::to_string(renderableText.second->renderPriority) + "</RenderPriority>\r\n" +
                tab + "    <TextContent>" + renderableText.second->text.getString() + "</TextContent>\r\n" +
                tab + "    <FontFilePath>" + renderableText.second->fontFilePath + "</FontFilePath>\r\n" +
                tab + "  </Item>\r\n";
        }

        return
            tab + "<RenderableTexts>\r\n" +
            renderableTextContent +
            tab + "</RenderableTexts>\r\n";

    }
    std::string XMLSystem::serializeRenderableRectangleMap(std::string tab) {
        std::string renderableRectangleContent = "";
        for (auto renderableRectangle : renderableRectangleMap) {
            renderableRectangleContent +=
                tab + "  <Item Name=\"" + renderableRectangle.first + "\">\r\n" +
                tab + "    <RenderLayer>" + std::to_string(renderableRectangle.second->renderLayer) + "</RenderLayer>\r\n" +
                tab + "    <RenderPriority>" + std::to_string(renderableRectangle.second->renderPriority) + "</RenderPriority>\r\n" +
                tab + "  </Item>\r\n";
        }

        return
            tab + "<RenderableRectangles>\r\n" +
            renderableRectangleContent +
            tab + "</RenderableRectangles>\r\n";
    }
    std::string XMLSystem::serializeRenderableCircleMap(std::string tab) {
        std::string renderableCircleContent = "";
        for (auto renderableCircle : renderableCircleMap) {
            renderableCircleContent +=
                tab + "  <Item Name=\"" + renderableCircle.first + "\">\r\n" +
                tab + "    <RenderLayer>" + std::to_string(renderableCircle.second->renderLayer) + "</RenderLayer>\r\n" +
                tab + "    <RenderPriority>" + std::to_string(renderableCircle.second->renderPriority) + "</RenderPriority>\r\n" +
                tab + "  </Item>\r\n";
        }

        return
            tab + "<RenderableCircles>\r\n" +
            renderableCircleContent +
            tab + "</RenderableCircles>\r\n";
    }
    std::string XMLSystem::serializeRenderableSpriteMap(std::string tab) {
        std::string renderableSpriteContent = "";
        for (auto renderableSprite : renderableSpriteMap) {
            renderableSpriteContent +=
                tab + "  <Item Name=\"" + renderableSprite.first + "\">\r\n" +
                tab + "    <RenderLayer>" + std::to_string(renderableSprite.second->renderLayer) + "</RenderLayer>\r\n" +
                tab + "    <RenderPriority>" + std::to_string(renderableSprite.second->renderPriority) + "</RenderPriority>\r\n" +
                tab + "    <TextureFilePath>" + renderableSprite.second->textureFileName + "</TextureFilePath>\r\n" +
                tab + "    <AnimationName>" + renderableSprite.second->animName + "</AnimationName>\r\n" +
                tab + "  </Item>\r\n";
        }

        return
            tab + "<RenderableSprites>\r\n" +
            renderableSpriteContent +
            tab + "</RenderableSprites>\r\n";
    }
    std::string XMLSystem::serializePrefabMap(std::string tab) {
        return 
            "<Prefabs>\r\n" +
            serializeEntitiesHelper(prefabMap, tab, false) + //no need to 'check for prefabs' since they all are
            "</Prefabs>\r\n";
    }
    std::string XMLSystem::serializeLevelMap(std::string tab) {
        std::string levelContent = "";
        for (auto level : levelMap) {
            levelContent +=
                tab + "<Level Name\"" + level.first + "\">\r\n" +
                tab + "  <Entities>\r\n" +
                serializeEntitiesHelper(level.second, tab + "  ", true) + //do need to 'check for prefabs' to skip recreation of entity
                tab + "  </Entities>\r\n" +
                tab + "</Level>\r\n";
        }
        return levelContent;
    }

#pragma endregion

#pragma region Deserialization

    void XMLSystem::deserializeTextureFilePathSet(XMLNode* node) {
        deserializeFilePathSet(textureFilePathSet, node);
    }
    void XMLSystem::deserializeMusicFilePathSet(XMLNode* node) {
        deserializeFilePathSet(musicFilePathSet, node);
    }
    void XMLSystem::deserializeSoundFilePathSet(XMLNode* node) {
        deserializeFilePathSet(soundFilePathSet, node);
    }
    void XMLSystem::deserializeFontFilePathSet(XMLNode* node) {
        deserializeFilePathSet(fontFilePathSet, node);
    }
    void XMLSystem::deserializeAnimationMap(XMLNode* node) {

    }
    void XMLSystem::deserializeRenderableTextMap(XMLNode* node) {

    }
    void XMLSystem::deserializeRenderableRectangleMap(XMLNode* node) {

    }
    void XMLSystem::deserializeRenderableCircleMap(XMLNode* node) {

    }
    void XMLSystem::deserializeRenderableSpriteMap(XMLNode* node) {

    }
    void XMLSystem::deserializePrefabMap(XMLNode* node) {

    }
    void XMLSystem::deserializeLevelMap(XMLNode* node) {

    }

#pragma endregion

#pragma region FileNameMappings

    std::string XMLSystem::getAssetNameFromFilePath(std::string assetFilePath, bool includeExtension) {
        int nameStart = assetFilePath.find_last_of('/')+1;
        if (!includeExtension) {
            return assetFilePath.substr(nameStart);
        }
        else {
            return assetFilePath.substr(nameStart, assetFilePath.find_last_of('.') - nameStart);
        }
    }

#pragma endregion

#pragma region SerializationHelpers

    std::string XMLSystem::serializeEntitiesHelper(std::map<std::string, std::shared_ptr<ex::Entity>> map, std::string tab,
            bool checkForPrefabs) {

        std::string entityContent = "";
        for (auto name_entity : map) {
            auto data = name_entity.second->component<Data>();

            // If we assume that the prefabMap is filled, check whether our entity's prefab's name can be found in the list of prefabs.
            // If we DO find an entry (meaning the entity is linked to a given prefab), then check whether it has been modified from the
            // original.
            if (checkForPrefabs && prefabMap.find(data->prefabName) != prefabMap.end() && !data->modified) {
                // If we found an entry and it hasn't been modified, then just input the name of the 
                // entity (i.e. prefab) and leave it at that
                entityContent += tab + "  <PrefabName>" + name_entity.first + "</PrefabName>\r\n";
            }
            else {
                // Else, if we did not find an entry, or if we did, but it didn't precisely match the original prefab,
                // then we need to outline the exact details of the Entity's structure.
                // Note: Future implementations could speed this process by adding "modified" booleans to each individual component instead

                entityContent +=
                    tab + "  <Entity Name=\"" + name_entity.first + "\">\r\n";
                
                // save current tab length
                std::string tempTab = tab;
                // Increment the tab length so that all successive serializations are tabbed by 1 more
                tab += "  ";

                if (name_entity.second->has_component<Data>()) {
                    entityContent += name_entity.second->component<Data>()->serialize(tab);
                }
                if (name_entity.second->has_component<Transform>()) {
                    entityContent += name_entity.second->component<Transform>()->serialize(tab);
                }
                if (name_entity.second->has_component<Rigidbody>()) {
                    entityContent += name_entity.second->component<Rigidbody>()->serialize(tab);
                }
                if (name_entity.second->has_component<BoxCollider>()) {
                    entityContent += name_entity.second->component<BoxCollider>()->serialize(tab);
                }
                if (name_entity.second->has_component<SoundMaker>()) {
                    entityContent += name_entity.second->component<SoundMaker>()->serialize(tab);
                }
                if (name_entity.second->has_component<MusicMaker>()) {
                    entityContent += name_entity.second->component<MusicMaker>()->serialize(tab);
                }
                if (name_entity.second->has_component<Renderer>()) {
                    entityContent += name_entity.second->component<Renderer>()->serialize(tab);
                }
                if (name_entity.second->has_component<TimeTable>()) {
                    entityContent += name_entity.second->component<TimeTable>()->serialize(tab);
                }
                if (name_entity.second->has_component<CoreBehavior>()) {
                    entityContent += name_entity.second->component<CoreBehavior>()->serialize(tab);
                }
                if (name_entity.second->has_component<ActionListener>()) {
                    entityContent += name_entity.second->component<ActionListener>()->serialize(tab);
                }

                // reinstate tab length
                tab = tempTab;

                entityContent +=
                    tab + "  </Entity>\r\n";
            }
        }

        return entityContent;
    }

    void XMLSystem::deserializeEntitiesHelper(std::map<std::string, std::shared_ptr<ex::Entity>> map, XMLNode* node,
            bool checkForPrefabs) {


    }

    std::string XMLSystem::serializeFilePathSet(std::set<std::string> filePathSet, std::string wrapperElement, std::string tab) {
        std::string content = "";
        for (auto path : filePathSet) {
            content += tab + "  <FilePath>" + path + "</FilePath>\r\n";
        }

        return
            tab + "<" + wrapperElement + ">\r\n" +
            content +
            tab + "</" + wrapperElement + ">\r\n";
    }

    void XMLSystem::deserializeFilePathSet(std::set<std::string> filePathSet, XMLNode* node) {
        filePathSet.clear();
        XMLElement* path = node->FirstChildElement("FilePath");
        while (path) {
            filePathSet.insert(path->GetText());
            path = path->NextSiblingElement("FilePath");
        }
    }

#pragma endregion

}

#include "XMLSystem.h"

namespace Raven {

    XMLSystem::XMLSystem()
    {
    }


    XMLSystem::~XMLSystem()
    {
    }

#pragma region Serialization

    std::string XMLSystem::serializeTextureFileMap(std::string tab) {
        std::string textureContent = "";
        for (auto texture : textureFileMap) {
            textureContent +=
                tab + "  <Item Name=\"" + texture.first + "\">\r\n" +
                tab + "    <FilePath>" + texture.second + "</FilePath>\r\n" +
                tab + "  </Item>\r\n";
        }

        return
            tab + "<Textures>\r\n" +
            textureContent +
            tab + "</Textures>\r\n";
    }
    std::string XMLSystem::serializeMusicFileMap(std::string tab) {
        std::string musicContent = "";
        for (auto music : musicFileMap) {
            musicContent +=
                tab + "  <Item Name=\"" + music.first + "\">\r\n" +
                tab + "    <FilePath>" + music.second + "</FilePath>\r\n" +
                tab + "  </Item>\r\n";
        }

        return
            tab + "<Music>\r\n" +
            musicContent +
            tab + "</Music>\r\n";
    }
    std::string XMLSystem::serializeSoundFileMap(std::string tab) {
        std::string soundContent = "";
        for (auto sound : soundFileMap) {
            soundContent +=
                tab + "  <Item Name=\"" + sound.first + "\">\r\n" +
                tab + "    <FilePath>" + sound.second + "</FilePath>\r\n" +
                tab + "  </Item>\r\n";
        }

        return
            tab + "<Sounds>\r\n" +
            soundContent +
            tab + "</Sounds>\r\n";
    }
    std::string XMLSystem::serializeFontFileMap(std::string tab) {
        std::string fontContent = "";
        for (auto font : fontFileMap) {
            fontContent +=
                tab + "  <Item Name=\"" + font.first + "\">\r\n" +
                tab + "    <FilePath>" + font.second + "</FilePath>\r\n" +
                tab + "  </Item>\r\n";
        }

        return
            tab + "<Fonts>\r\n" +
            fontContent +
            tab + "</Fonts>\r\n";
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
        return 
            "<Entities>\r\n" +
            serializeEntitiesHelper(prefabMap, tab, true) + //do need to 'check for prefabs' to skip recreation of entity
            "</Entities>\r\n";
    }

#pragma endregion

#pragma region Deserialization

    void XMLSystem::deserializeTextureFileMap(XMLNode* node) {

    }
    void XMLSystem::deserializeMusicFileMap(XMLNode* node) {

    }
    void XMLSystem::deserializeSoundFileMap(XMLNode* node) {

    }
    void XMLSystem::deserializeFontFileMap(XMLNode* node) {

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

    std::string XMLSystem::getTextureName(std::string textureFileName) {
        for (auto name_filepath : textureFileMap) {
            if (name_filepath.second == textureFileName) {
                return name_filepath.first;
            }
        }
        return "";
    }

    std::string XMLSystem::getMusicName(std::string musicFileName) {
        for (auto name_filepath : musicFileMap) {
            if (name_filepath.second == musicFileName) {
                return name_filepath.first;
            }
        }
        return "";
    }

    std::string XMLSystem::getSoundName(std::string soundFileName) {
        for (auto name_filepath : soundFileMap) {
            if (name_filepath.second == soundFileName) {
                return name_filepath.first;
            }
        }
        return "";
    }

    std::string XMLSystem::getFontName(std::string fontFileName) {
        for (auto name_filepath : fontFileMap) {
            if (name_filepath.second == fontFileName) {
                return name_filepath.first;
            }
        }
        return "";
    }

#pragma endregion

#pragma region ComponentTranslation


    std::string XMLSystem::serializeEntitiesHelper(std::map<std::string, std::shared_ptr<ex::Entity>> map, std::string tab,
            bool checkForPrefabs) {

        std::string entityContent = "";
        for (auto entity : map) {
            entityContent +=
                tab + "  <Entity Name=\"" + entity.first + "\">\r\n";

            if (entity.second->has_component<Data>()) {
                entityContent += entity.second->component<Data>()->serialize(tab);
            }
            if (entity.second->has_component<Transform>()) {
                entityContent += entity.second->component<Transform>()->serialize(tab);
            }
            if (entity.second->has_component<Rigidbody>()) {
                entityContent += entity.second->component<Rigidbody>()->serialize(tab);
            }
            if (entity.second->has_component<BoxCollider>()) {
                entityContent += entity.second->component<BoxCollider>()->serialize(tab);
            }
            if (entity.second->has_component<SoundMaker>()) {
                entityContent += entity.second->component<SoundMaker>()->serialize(tab);
            }
            if (entity.second->has_component<MusicMaker>()) {
                entityContent += entity.second->component<MusicMaker>()->serialize(tab);
            }
            if (entity.second->has_component<Renderer>()) {
                entityContent += entity.second->component<Renderer>()->serialize(tab);
            }
            if (entity.second->has_component<TimeTable>()) {
                entityContent += entity.second->component<TimeTable>()->serialize(tab);
            }
            if (entity.second->has_component<CoreBehavior>()) {
                entityContent += entity.second->component<CoreBehavior>()->serialize(tab);
            }
            if (entity.second->has_component<ActionListener>()) {
                entityContent += entity.second->component<ActionListener>()->serialize(tab);
            }

            entityContent +=
                tab + "  </Entity>\r\n";

        }

        return entityContent;
    }

    void XMLSystem::deserializeEntitiesHelper(std::map<std::string, std::shared_ptr<ex::Entity>> map, XMLNode* node,
            bool checkForPrefabs) {


    }
#pragma endregion

}

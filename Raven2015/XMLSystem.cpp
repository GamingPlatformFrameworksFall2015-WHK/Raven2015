#include "XMLSystem.h"
#include "EntityLibrary.h"
#include "WidgetLibrary.h"

namespace Raven {

    XMLSystem::XMLSystem() {
        // Create the base entity for the user to use
        //deserializeRavenGame();

        /*
        std::shared_ptr<ex::Entity> eptr(new ex::Entity(EntityLibrary::Create::Entity("Base Entity")));
        prefabMap.insert(std::make_pair("Base Entity", eptr));
        ex::ComponentHandle<Data> data = prefabMap["Base Entity"]->component<Data>();
        data->name = "Base Entity";
        data->prefabName = "Base Entity";
        data->modified = false;
        */
    }

    XMLSystem::~XMLSystem() {}

    const std::string XMLSystem::xmlFileName = "raven.xml";

#pragma region Raven Game Serialization

    std::string XMLSystem::serializeRavenGame() {
        return "<?xml version=\"1.0\"?>\r\n"
            "<!DOCTYPE RAVEN SYSTEM \"raven.dtd\">\r\n"
            "<RAVEN>\r\n"
            "  <Assets>\r\n" +
            serializeTextureFilePathSet("    ") +
            serializeMusicFilePathSet("    ") +
            serializeSoundFilePathSet("    ") +
            serializeFontFilePathSet("    ") +
            serializeAnimationMap("    ") +
            "    <Renderables>\r\n" +
            serializeRenderableTextMap("      ") +
            serializeRenderableRectangleMap("      ") +
            serializeRenderableCircleMap("      ") +
            serializeRenderableSpriteMap("      ") +
            "    </Renderables>\r\n"
            "  </Assets>\r\n" +
            serializePrefabMap("  ") +
            serializeLevelMap("  ") +
            "</RAVEN>\r\n";
    }

    void XMLSystem::deserializeRavenGame() {
        XMLElement* raven = doc.FirstChildElement("RAVEN");
        XMLElement* assets = raven->FirstChildElement("Assets");
        deserializeTextureFilePathSet(assets->FirstChildElement("Textures"));
        deserializeTextureFilePathSet(assets->FirstChildElement("Music"));
        deserializeTextureFilePathSet(assets->FirstChildElement("Sounds"));
        deserializeTextureFilePathSet(assets->FirstChildElement("Fonts"));
        deserializeAnimationMap(assets->FirstChildElement("Animations"));
        XMLElement* renderables = assets->FirstChildElement("Renderables");
        deserializeRenderableTextMap(renderables->FirstChildElement("RenderableTexts"));
        deserializeRenderableRectangleMap(renderables->FirstChildElement("RenderableRectangles"));
        deserializeRenderableCircleMap(renderables->FirstChildElement("RenderableCircles"));
        deserializeRenderableSpriteMap(renderables->FirstChildElement("RenderableSprites"));
        deserializePrefabMap(raven->FirstChildElement("Prefabs"));
        deserializeLevelMap(raven->FirstChildElement("Levels"));
    }

#pragma endregion

#pragma region Entity Serialization

    std::string XMLSystem::serializeEntity(ex::Entity e, std::string tab, bool forPrefab) {
        std::string str =
            tab + (forPrefab ? "<P" : "<L") + "Entity>\r\n";
        std::string tempTab = tab;  // save tab
        tab += "  ";                // increment tab
        SERIALIZE_COMPONENTS(e, str, forPrefab)// process components
        tab = tempTab;              // reinstate tab
        return str +=
            tab + (forPrefab ? "</P" : "</L") + "Entity>\r\n";
    }

    void XMLSystem::deserializeEntity(ex::Entity e, XMLNode* node, bool forPrefab) {
        DESERIALIZE_COMPONENTS(e, node, forPrefab)
    }

#pragma endregion

#pragma region Events

#pragma region Serialization Events

    void XMLSystem::receive(const XMLLoadEvent& e) {
        if (doc.LoadFile(xmlFileName.c_str()) != XML_NO_ERROR) {
            cerr << "WARNING: Game XML Document Failed To Load!" << endl;
        }
        else {
            cout << "Game XML Document successfully loaded." << endl;
        }
        deserializeRavenGame();
    }

    void XMLSystem::receive(const XMLSaveEvent& e) {
        doc.Parse(serializeRavenGame().c_str());
        if (doc.SaveFile(xmlFileName.c_str()) != XML_NO_ERROR) {
            cerr << "WARNING: Game Failed To Save!" << endl;
        }
        else {
            cout << "Game successfully saved." << endl;
        }
    }

#pragma endregion

#pragma region Data Management Events

    void XMLSystem::receive(const XMLUpdateEntityNameEvent& event) {
        ex::Entity e = event.entity;
        std::string newName = event.newName;
        std::map<std::string, std::shared_ptr<ex::Entity>>* map;
        // Acquire the map
        if (event.isPrefab) {
            map = &prefabMap;
        }
        else {
            map = &levelMap[cmn::game->currentLevelName];
        }
        // Double check whether we can even find a record of the entity's name
        if (map->find(e.component<Data>()->name) != map->end()) {
            // Acquire the entity pointer
            std::shared_ptr<ex::Entity> ptr = (*map)[e.component<Data>()->name];
            // Destroy the record of the previous name
            map->erase(e.component<Data>()->name);
            // Create a new record using the new name
            map->insert(std::make_pair(newName, ptr));
            // Assign the new name to the entity itself
            ptr->component<Data>()->name = newName;
        }
        // Else, simply add the new record
        else {
            e.component<Data>()->name = newName;
            map->insert(std::make_pair(newName, std::shared_ptr<ex::Entity>(new ex::Entity(e))));
        }
    }

#pragma endregion

#pragma endregion

#pragma region Population Initialization

    template <typename PanelType>
    void XMLSystem::receiveEntityMap(const GUIWidgetListEvent<PanelType, ENTITY_LIST_LIST_ITEM_TEMPLATE>& e,
            std::map<std::string, std::shared_ptr<ex::Entity>>& map) {

        // A switch statement wouldn't work for some reason. e.POPULATE not recognized as a constant...
        // Address later if desired since switch statements are more efficient.
        if (e.op == e.POPULATE) { 
            e.box->RemoveAll();
            for (auto name_entity : map) {
                WidgetLibrary::WidgetList<PanelType, ENTITY_LIST_LIST_ITEM_TEMPLATE>::appendWidget(
                    e.box, name_entity.first, e.formatter);
            }
        }
        else if (e.op == e.ADD) {
            WidgetLibrary::WidgetList<PanelType, ENTITY_LIST_LIST_ITEM_TEMPLATE>::appendWidget(
                e.box, e.itemName, e.formatter);
        }
        else if (e.op == e.REMOVE) {
            WidgetLibrary::WidgetList<PanelType, ENTITY_LIST_LIST_ITEM_TEMPLATE>::removeWidget(
                e.box, e.itemName);
        }
    }

    void XMLSystem::receive(const GUIWidgetListEvent<WidgetLibrary::SceneHierarchyPanel, ENTITY_LIST_LIST_ITEM_TEMPLATE>& e) {
        receiveEntityMap<WidgetLibrary::SceneHierarchyPanel>(e, levelMap[cmn::game->currentLevelName]);
    }

    void XMLSystem::receive(const GUIWidgetListEvent<WidgetLibrary::PrefabListPanel, ENTITY_LIST_LIST_ITEM_TEMPLATE>& e) {
        receiveEntityMap<WidgetLibrary::PrefabListPanel>(e, prefabMap);
    }

#pragma endregion

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
                tab + "  <Animation Name=\"" + animation.first + "\">\r\n" +
                tab + "    <AnimationTextureFilePath>" + animation.second->textureFileName + "</AnimationTextureFilePath>\r\n" +
                tab + "    <FrameCount>" + std::to_string(animation.second->size) + "</FrameCount>\r\n" +
                tab + "    <FrameWidth>" + std::to_string(animation.second->frameWidth) + "</FrameWidth>\r\n" +
                tab + "    <FrameHeight>" + std::to_string(animation.second->frameHeight) + "</FrameHeight>\r\n" +
                tab + "    <AnimationSpeed>" + std::to_string(animation.second->animationSpeed) + "</AnimationSpeed>\r\n" +
                tab + "    <IsLooping>" + std::to_string(animation.second->isLooping) + "</IsLooping>\r\n" +
                tab + "  </Animation>\r\n";
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
                tab + "  <RenderableText Name=\"" + renderableText.first + "\">\r\n" +
                tab + "    <RenderableTextRenderLayer>" + std::to_string(renderableText.second->renderLayer) + "</RenderableTextRenderLayer>\r\n" +
                tab + "    <RenderableTextRenderPriority>" + std::to_string(renderableText.second->renderPriority) + "</RenderableTextRenderPriority>\r\n" +
                tab + "    <RenderableTextTextContent>" + renderableText.second->text.getString() + "</RenderableTextTextContent>\r\n" +
                tab + "    <RenderableTextFontFilePath>" + renderableText.second->fontFilePath + "</RenderableTextFontFilePath>\r\n" +
                tab + "  </RenderableText>\r\n";
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
                tab + "  <RenderableRectangle Name=\"" + renderableRectangle.first + "\">\r\n" +
                tab + "    <RenderableRectangleRenderLayer>" + std::to_string(renderableRectangle.second->renderLayer) + "</RenderableRectangleRenderLayer>\r\n" +
                tab + "    <RenderableRectangleRenderPriority>" + std::to_string(renderableRectangle.second->renderPriority) + "</RenderableRectangleRenderPriority>\r\n" +
                tab + "  </RenderableRectangle>\r\n";
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
                tab + "  <RenderableCircle Name=\"" + renderableCircle.first + "\">\r\n" +
                tab + "    <RenderableCircleRenderLayer>" + std::to_string(renderableCircle.second->renderLayer) + "</RenderableCircleRenderLayer>\r\n" +
                tab + "    <RenderableCircleRenderPriority>" + std::to_string(renderableCircle.second->renderPriority) + "</RenderableCircleRenderPriority>\r\n" +
                tab + "  </RenderableCircle>\r\n";
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
                tab + "  <RenderableSprite Name=\"" + renderableSprite.first + "\">\r\n" +
                tab + "    <RenderableSpriteRenderLayer>" + std::to_string(renderableSprite.second->renderLayer) + "</RenderableSpriteRenderLayer>\r\n" +
                tab + "    <RenderableSpriteRenderPriority>" + std::to_string(renderableSprite.second->renderPriority) + "</RenderableSpriteRenderPriority>\r\n" +
                tab + "    <RenderableSpriteTextureFilePath>" + renderableSprite.second->textureFileName + "</RenderableSpriteTextureFilePath>\r\n" +
                tab + "    <RenderableSpriteAnimationName>" + renderableSprite.second->animName + "</RenderableSpriteAnimationName>\r\n" +
                tab + "  </RenderableSprite>\r\n";
        }

        return
            tab + "<RenderableSprites>\r\n" +
            renderableSpriteContent +
            tab + "</RenderableSprites>\r\n";
    }

    std::string XMLSystem::serializePrefabMap(std::string tab) {
        std::string prefabMapContent = "";
        for (auto item : prefabMap) {
            prefabMapContent +=
                serializeEntity(*item.second, tab + "  ", true);
        }
        return
            tab + "<Prefabs>\r\n" +
            prefabMapContent +
            tab + "</Prefabs>\r\n";
    }

    std::string XMLSystem::serializeLevelMap(std::string tab) {
        std::string levelMapContent = tab + "<Levels>\r\n";
        // For each level
        for (auto levelName_entityMap : levelMap) {

            // Add the header for the level's XML tag
            levelMapContent += tab + "  <Level Name=\"" + levelName_entityMap.first + "\">\r\n";

            // For each entity in the level
            for (auto name_entity : levelName_entityMap.second) {
                // Grab the entity's data
                auto data = name_entity.second->component<Data>();

                // If the entity is linked to and is a copy of a prefab, then there's no need to store its information
                if (data->prefabName != "" && !data->modified) {
                    levelMapContent += tab + "    <PrefabNameCheck>" + data->prefabName + "</PrefabNameCheck>\r\n";
                }
                // Else, if we need to store its information, serialize each of its components
                else {
                    levelMapContent += serializeEntity(*name_entity.second, tab + "    ", false);
                }
            }

            // Add the tail of the level's XML tag
            levelMapContent += tab + "  </Level>\r\n";
        }
        return levelMapContent += tab += "</Levels>\r\n";
    }

#pragma endregion

#pragma region Deserialization

    void XMLSystem::deserializeTextureFilePathSet(XMLNode* node) {
        deserializeFilePathSet(textureFilePathSet, "Textures", node);
    }

    void XMLSystem::deserializeMusicFilePathSet(XMLNode* node) {
        deserializeFilePathSet(musicFilePathSet, "Music", node);
    }

    void XMLSystem::deserializeSoundFilePathSet(XMLNode* node) {
        deserializeFilePathSet(soundFilePathSet, "Sounds", node);
    }

    void XMLSystem::deserializeFontFilePathSet(XMLNode* node) {
        deserializeFilePathSet(fontFilePathSet, "Fonts", node);
    }

    void XMLSystem::deserializeAnimationMap(XMLNode* node) {
        animationMap.clear();
        XMLElement* item = node->FirstChildElement("Animation");
        while (item) {

            // Get the name of the Item
            std::string name = item->FirstAttribute()->Value();

            // Instantiate the given asset
            animationMap.insert(std::make_pair(name, std::shared_ptr<Animation>(new Animation())));
            std::shared_ptr<Animation> ptr = animationMap[name];
            
            XMLElement* e = item->FirstChildElement("AnimationTextureFilePath");
            ptr->textureFileName = e->Value();
            e = item->FirstChildElement("FrameCount");
            e->QueryIntText(&ptr->size);
            e = item->FirstChildElement("FrameWidth");
            e->QueryIntText(&ptr->frameWidth);
            e = item->FirstChildElement("FrameHeight");
            e->QueryIntText(&ptr->frameHeight);
            e = item->FirstChildElement("AnimationSpeed");
            e->QueryDoubleText(&ptr->animationSpeed);
            e = item->FirstChildElement("IsLooping");
            e->QueryBoolText(&ptr->isLooping);

            item = item->NextSiblingElement("Animation");
        }
    }

    void XMLSystem::deserializeRenderableTextMap(XMLNode* node) {
        renderableTextMap.clear();
        XMLElement* item = node->FirstChildElement("RenderableText");
        while (item) {

            // Get the name of the Item
            std::string name = item->FirstAttribute()->Value();

            // Instantiate the given asset
            renderableTextMap.insert(std::make_pair(name, std::shared_ptr<RenderableText>(new RenderableText())));
            std::shared_ptr<RenderableText> ptr = renderableTextMap[name];
            
            XMLElement* e = item->FirstChildElement("RenderableTextRenderLayer");
            int i;
            e->QueryIntText(&i);
            ptr->renderLayer = (cmn::ERenderingLayer) i;
            e = item->FirstChildElement("RenderableTextRenderPriority");
            e->QueryIntText(&ptr->renderPriority);
            e = item->FirstChildElement("RenderableTextTextContent");
            ptr->text.setString(e->GetText());
            e = item->FirstChildElement("RenderableTextFontFilePath");
            ptr->fontFilePath = e->GetText();

            item = item->NextSiblingElement("RenderableText");
        }

    }

    void XMLSystem::deserializeRenderableRectangleMap(XMLNode* node) {
        renderableRectangleMap.clear();
        XMLElement* item = node->FirstChildElement("RenderableRectangle");
        while (item) {

            // Get the name of the Item
            std::string name = item->FirstAttribute()->Value();

            // Instantiate the given asset
            renderableRectangleMap.insert(std::make_pair(name, std::shared_ptr<RenderableRectangle>(new RenderableRectangle())));
            std::shared_ptr<RenderableRectangle> ptr = renderableRectangleMap[name];
            
            XMLElement* e = item->FirstChildElement("RenderableRectangleRenderLayer");
            int i;
            e->QueryIntText(&i);
            ptr->renderLayer = (cmn::ERenderingLayer) i;
            e = item->FirstChildElement("RenderableRectangleRenderPriority");
            e->QueryIntText(&ptr->renderPriority);

            item = item->NextSiblingElement("RenderableRectangle");
        }
    }

    void XMLSystem::deserializeRenderableCircleMap(XMLNode* node) {
        renderableCircleMap.clear();
        XMLElement* item = node->FirstChildElement("RenderableCircle");
        while (item) {

            // Get the name of the Item
            std::string name = item->FirstAttribute()->Value();

            // Instantiate the given asset
            renderableCircleMap.insert(std::make_pair(name, std::shared_ptr<RenderableCircle>(new RenderableCircle())));
            std::shared_ptr<RenderableCircle> ptr = renderableCircleMap[name];
            
            XMLElement* e = item->FirstChildElement("RenderableCircleRenderLayer");
            int i;
            e->QueryIntText(&i);
            ptr->renderLayer = (cmn::ERenderingLayer) i;
            e = item->FirstChildElement("RenderableCircleRenderPriority");
            e->QueryIntText(&ptr->renderPriority);

            item = item->NextSiblingElement("RenderableCircle");
        }
    }

    void XMLSystem::deserializeRenderableSpriteMap(XMLNode* node) {
        renderableSpriteMap.clear();
        XMLElement* item = node->FirstChildElement("RenderableSprite");
        while (item) {

            // Get the name of the Item
            std::string name = item->FirstAttribute()->Value();

            // Instantiate the given asset
            renderableSpriteMap.insert(std::make_pair(name, std::shared_ptr<RenderableSprite>(new RenderableSprite())));
            std::shared_ptr<RenderableSprite> ptr = renderableSpriteMap[name];
            
            XMLElement* e = item->FirstChildElement("RenderableSpriteRenderLayer");
            int i;
            e->QueryIntText(&i);
            ptr->renderLayer = (cmn::ERenderingLayer) i;
            e = item->FirstChildElement("RenderableSpriteRenderPriority");
            e->QueryIntText(&ptr->renderPriority);
            e = item->FirstChildElement("RenderableSpriteTextureFilePath");
            ptr->textureFileName = e->GetText();
            e = item->FirstChildElement("RenderableSpriteAnimationName");
            ptr->animName = e->GetText();

            item = item->NextSiblingElement("RenderableSprite");
        }
    }

    void XMLSystem::deserializePrefabMap(XMLNode* node) {
        prefabMap.clear();
        XMLElement* elem = node->FirstChildElement("PEntity");
        while (elem) {
            std::shared_ptr<ex::Entity> entity(new ex::Entity(cmn::entities->create()));
            deserializeEntity(*entity, elem, true);
            if (entity->has_component<Data>()) cout << "Prefab Entity has a Data!" << endl;
            if (entity->has_component<Transform>()) cout << "Prefab Entity has a Transform!" << endl;
            if (entity->has_component<Rigidbody>()) cout << "Prefab Entity has a Rigidbody!" << endl;
            prefabMap.insert(std::make_pair(entity->component<Data>()->name, entity));
            elem = elem->NextSiblingElement("PEntity");
        }
    }
    
    void XMLSystem::deserializeLevelMap(XMLNode* node) {
        levelMap.clear();
        XMLElement* lnode = node->FirstChildElement("Level");
        while (lnode) {
            std::string levelName = lnode->Attribute("Name");
            XMLElement* item = lnode->FirstChildElement("LevelEntity");
            levelMap.insert(std::make_pair(levelName, std::map<std::string, std::shared_ptr<ex::Entity>>()));
            while (item) {
                std::string prefabName = item->FirstChildElement("PrefabNameCheck")->GetText();
                std::shared_ptr<ex::Entity> entity(&cmn::entities->create());
                if (prefabName == "") {
                    XMLSystem::deserializeEntity(*entity, item->FirstChildElement("LEntity"), false);
                }
                else {
                    EntityLibrary::copyEntity(*entity, *prefabMap[prefabName]);
                }
                levelMap[levelName].insert(std::make_pair(entity->component<Data>()->name, entity));
                item = item->NextSiblingElement("LevelEntity");
            }
            lnode = lnode->NextSiblingElement("Level");
        }
    }

#pragma endregion

#pragma region FileNameMappings

    std::string XMLSystem::getAssetNameFromFilePath(std::string assetFilePath, bool includeExtension) {
        auto nameStart = assetFilePath.find_last_of('/')+1;
        if (!includeExtension) {
            return assetFilePath.substr(nameStart);
        }
        else {
            return assetFilePath.substr(nameStart, assetFilePath.find_last_of('.') - nameStart);
        }
    }


    std::string XMLSystem::serializeFilePathSet(std::set<std::string> filePathSet, std::string wrapperElement, std::string tab) {
        std::string content = "";
        std::string prefix = (wrapperElement == "Music" ? wrapperElement : wrapperElement.substr(wrapperElement.size() - 1));
        for (auto path : filePathSet) {
            content += tab + "  <" + prefix + "FilePath>" + path + "</" + prefix + "FilePath>\r\n";
        }

        return
            tab + "<" + wrapperElement + ">\r\n" +
            content +
            tab + "</" + wrapperElement + ">\r\n";
    }

    void XMLSystem::deserializeFilePathSet(std::set<std::string> filePathSet, std::string wrapperElement, XMLNode* node) {
        filePathSet.clear();
        std::string prefix = (wrapperElement == "Music" ? wrapperElement : wrapperElement.substr(wrapperElement.size() - 1));
        XMLElement* path = node->FirstChildElement((wrapperElement + "FilePath").c_str());
        while (path) {
            filePathSet.insert(path->GetText());
            path = node->NextSiblingElement((wrapperElement + "FilePath").c_str());
        }
    }

#pragma endregion

}

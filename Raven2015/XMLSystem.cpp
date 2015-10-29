#include "XMLSystem.h"
#include "EntityLibrary.h"
#include "WidgetLibrary.h"

namespace Raven {

    XMLSystem::XMLSystem() {}

    XMLSystem::~XMLSystem() {}

#pragma region Raven Game Serialization

    void XMLSystem::serializeRavenGame() {
        cout << "Saving game project..." << endl;
        bool success = true;

        // Assets
        cout << "Step 1: Assets" << endl;
        success &= saveAssets();

        // Prefabs
        cout << "Step 2: Prefabs" << endl;
        success &= savePrefabs();

        // Level
        cout << "Step 3: Level" << endl;
        success &= saveLevel(cmn::game->currentLevelPath);

        // Report
        if (success) {
            cout << "Game successfully saved!" << endl;
        }
        else {
            cout << "A problem occurred during game save." << endl;
        }
    }

    void XMLSystem::deserializeRavenGame() {
        cout << "Loading game project..." << endl;
        bool success = true;

        // Assets
        if (assetsDoc.LoadFile(assetsFileName.c_str()) != XML_NO_ERROR) {
            cerr << "WARNING: " + assetsFileName + " failed to load!" << endl;
            success = false;
        }
        else {
            cout << assetsFileName + " successfully loaded. Deserializing..." << endl;
            deserializeAssets(assetsDoc.FirstChildElement(assetsFirstChildElement.c_str()));
        }

        // Prefabs
        if (prefabsDoc.LoadFile(prefabsFileName.c_str()) != XML_NO_ERROR) {
            cerr << "WARNING: " + prefabsFileName + " failed to load!" << endl;
            success = false;
        }
        else {
            cout << prefabsFileName + " successfully loaded. Deserializing..." << endl;
        }

        // Level
        std::string level = cmn::game->currentLevelPath;
        if (levelDoc.LoadFile(level.c_str()) != XML_NO_ERROR) {
            cerr << "WARNING: " + getAssetNameFromFilePath(level, true) + " failed to load!" << endl;
            success = false;
        }
        else {
            cout << getAssetNameFromFilePath(level, true) + " successfully loaded. Deserializing..." << endl;
            deserializeLevel(levelDoc.FirstChildElement(levelFirstChildElement.c_str()));
        }

        // Report
        if (success) {
            cout << "Game project was successfully loaded" << endl;
        }
        else {
            cout << "There was a problem with loading the game project" << endl;
        }
    }

#pragma endregion

#pragma region Entity Serialization

    std::string XMLSystem::serializeEntity(ex::Entity e, std::string tab) {
        std::string str =
            tab + "<Entity>" + newline;
        std::string tempTab = tab;  // save tab
        tab += "  ";                // increment tab
        str += serializeEntityComponents<COMPONENT_TYPE_LIST>(e, tab, COMPONENT_TYPES(::getNullPtrToType()));// process components
        tab = tempTab;              // reinstate tab
        return str +=
            tab + "</Entity>" + newline;
    }

    void XMLSystem::deserializeEntity(ex::Entity e, XMLNode* node) {
        deserializeEntityComponents<COMPONENT_TYPE_LIST>(e, node, true, COMPONENT_TYPES(::getNullPtrToType()));
    }

    template <typename C>
    std::string XMLSystem::serializeEntityComponents(ex::Entity e, std::string tab, C* c = nullptr) {
        if (e.has_component<C>()) {
            return e.component<C>()->serialize(tab);
        }
        return "";
    }

    template <typename C, typename... Components>
    std::string XMLSystem::serializeEntityComponents(ex::Entity e, std::string tab, C* c, Components*... components) {
        std::string s = "";
        if (e.has_component<C>()) {
            s = e.component<C>()->serialize(tab);
        }
        return s + serializeEntityComponents<Components...>(e, tab, components...);
    }

    template <typename C>
    void XMLSystem::deserializeEntityComponents(ex::Entity e, XMLNode* node, bool firstCall, C* c) {
        if (firstCall) {
            EntityLibrary::clearEntity<C>(e, nullptr);
        }
        XMLElement* elem = node->FirstChildElement(C::getElementName().c_str());
        if (elem) {
            e.assign<C>()->deserialize(elem);
        }
    }

    template <typename C, typename... Components>
    void XMLSystem::deserializeEntityComponents(ex::Entity e, XMLNode* node, bool firstCall, C* c, Components*... components) {
        if (firstCall) {
            EntityLibrary::clearEntity<Components...>(e, Components::getNullPtrToType()...));
        }
        XMLElement* elem = node->FirstChildElement(C::getElementName().c_str());
        if (elem) {
            e.assign<C>()->deserialize(elem);
            cout << "Found Component. Deserializing... : " + C::getElementName() << endl;
        }
        else {
            cout << "Could not find component: " + C::getElementName() << endl;
        }
        deserializeEntityComponents<Components...>(e, node, false, components...);
    }

#pragma endregion

#pragma region Events

#pragma region Serialization Events

    void XMLSystem::receive(const XMLLoadEvent& e) {
        deserializeRavenGame();
    }

    void XMLSystem::receive(const XMLSaveEvent& e) {
        serializeRavenGame();
    }

#pragma endregion

#pragma region Data Management Events

    void XMLSystem::receive(const XMLUpdateEntityNameEvent& event) {
        ex::Entity e = event.entity;
        std::string newName = event.newName;

        XMLNode* top = nullptr;
        if (event.isPrefab) {
            top = prefabsDoc.FirstChild();
        }

        // Double check whether we can even find a record of the entity's name
        if (levelMap.find(e.component<Data>()->name) != levelMap.end()) {
            // Acquire the entity pointer
            std::shared_ptr<ex::Entity> ptr = levelMap[e.component<Data>()->name];
            // Destroy the record of the previous name
            levelMap.erase(e.component<Data>()->name);
            // Create a new record using the new name
            levelMap.insert(std::make_pair(newName, ptr));
            // Assign the new name to the entity itself
            ptr->component<Data>()->name = newName;
        }
        // Else, simply add the new record
        else {
            e.component<Data>()->name = newName;
            levelMap.insert(std::make_pair(newName, std::shared_ptr<ex::Entity>(new ex::Entity(e))));
        }
    }

#pragma endregion

#pragma endregion


#pragma region Prefab Manipulation

    bool XMLSystem::prefabExists(std::string prefabName) {
        XMLNode* top = prefabsDoc.FirstChild();
        XMLElement* entityNode = top->FirstChildElement("Entity");
        bool found = false;
        while (entityNode && !found) {
            XMLElement* data = entityNode->FirstChildElement("Data");
            if (data->FirstChildElement("Name")->GetText() == prefabName && 
                    data->FirstChildElement("PrefabName")->GetText() == "this") {

                found = true;
            }
            entityNode = entityNode->NextSiblingElement("Entity");
        }
        return found;
    }

    std::shared_ptr<ex::Entity> XMLSystem::instantiate(std::string prefabName) {
        XMLNode* top = prefabsDoc.FirstChild();
        XMLElement* entityNode = top->FirstChildElement("Entity");
        std::shared_ptr<ex::Entity> entity(nullptr);
        while (entityNode && !entity) {
            XMLElement* data = entityNode->FirstChildElement("Data");
            if (data->FirstChildElement("Name")->GetText() == prefabName && 
                    data->FirstChildElement("PrefabName")->GetText() == "this") {

                entity.reset(new ex::Entity(cmn::game->entities.create()));
                deserializeEntity(*entity, entityNode);
            }
            entityNode = entityNode->NextSiblingElement("Entity");
        }
        return entity;
    }

#pragma endregion

#pragma region Serialization

    std::string XMLSystem::serializeAssets() {
        std::string tab = "  ";
        return
            getXMLHeader(assetsFirstChildElement, assetsFileName) +
            "<" + assetsFirstChildElement + ">" + newline +
            serializeTextureFilePathSet(tab) +
            serializeMusicFilePathSet(tab) +
            serializeSoundFilePathSet(tab) +
            serializeFontFilePathSet(tab) +
            serializeLevelFilePathSet(tab) +
            serializeAnimationMap(tab) +
            serializeRenderableTextMap(tab) +
            serializeRenderableRectangleMap(tab) +
            serializeRenderableCircleMap(tab) +
            serializeRenderableSpriteMap(tab) +
            "</" + assetsFirstChildElement + ">" + newline;
    }

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

    std::string XMLSystem::serializeLevelFilePathSet(std::string tab) {
        return serializeFilePathSet(levelFilePathSet, "Levels", tab);
    }

    std::string XMLSystem::serializeAnimationMap(std::string tab) {
        std::string animationContent = "";
        for (auto animation : animationMap) {
            animationContent +=
                tab + "  <Animation Name=\"" + animation.first + "\">" + newline +
                tab + "    <AnimationTextureFilePath>" + animation.second->textureFileName + "</AnimationTextureFilePath>" + newline +
                tab + "    <FrameCount>" + std::to_string(animation.second->size) + "</FrameCount>" + newline +
                tab + "    <FrameWidth>" + std::to_string(animation.second->frameWidth) + "</FrameWidth>" + newline +
                tab + "    <FrameHeight>" + std::to_string(animation.second->frameHeight) + "</FrameHeight>" + newline +
                tab + "    <AnimationSpeed>" + std::to_string(animation.second->animationSpeed) + "</AnimationSpeed>" + newline +
                tab + "    <IsLooping>" + std::to_string(animation.second->isLooping) + "</IsLooping>" + newline +
                tab + "  </Animation>" + newline;
        }

        return
            tab + "<Animations>" + newline +
            animationContent +
            tab + "</Animations>" + newline;
    }

    std::string XMLSystem::serializeRenderableTextMap(std::string tab) {
        std::string renderableTextContent = "";
        for (auto renderableText : renderableTextMap) {
            renderableTextContent +=
                tab + "  <RenderableText Name=\"" + renderableText.first + "\">" + newline +
                tab + "    <RenderableTextRenderLayer>" + std::to_string(renderableText.second->renderLayer) + "</RenderableTextRenderLayer>" + newline +
                tab + "    <RenderableTextRenderPriority>" + std::to_string(renderableText.second->renderPriority) + "</RenderableTextRenderPriority>" + newline +
                tab + "    <RenderableTextTextContent>" + renderableText.second->text.getString() + "</RenderableTextTextContent>" + newline +
                tab + "    <RenderableTextFontFilePath>" + renderableText.second->fontFilePath + "</RenderableTextFontFilePath>" + newline +
                tab + "  </RenderableText>" + newline;
        }

        return
            tab + "<RenderableTexts>" + newline +
            renderableTextContent +
            tab + "</RenderableTexts>" + newline;

    }

    std::string XMLSystem::serializeRenderableRectangleMap(std::string tab) {
        std::string renderableRectangleContent = "";
        for (auto renderableRectangle : renderableRectangleMap) {
            renderableRectangleContent +=
                tab + "  <RenderableRectangle Name=\"" + renderableRectangle.first + "\">" + newline +
                tab + "    <RenderableRectangleRenderLayer>" + std::to_string(renderableRectangle.second->renderLayer) + "</RenderableRectangleRenderLayer>" + newline +
                tab + "    <RenderableRectangleRenderPriority>" + std::to_string(renderableRectangle.second->renderPriority) + "</RenderableRectangleRenderPriority>" + newline +
                tab + "  </RenderableRectangle>" + newline;
        }

        return
            tab + "<RenderableRectangles>" + newline +
            renderableRectangleContent +
            tab + "</RenderableRectangles>" + newline;
    }

    std::string XMLSystem::serializeRenderableCircleMap(std::string tab) {
        std::string renderableCircleContent = "";
        for (auto renderableCircle : renderableCircleMap) {
            renderableCircleContent +=
                tab + "  <RenderableCircle Name=\"" + renderableCircle.first + "\">" + newline +
                tab + "    <RenderableCircleRenderLayer>" + std::to_string(renderableCircle.second->renderLayer) + "</RenderableCircleRenderLayer>" + newline +
                tab + "    <RenderableCircleRenderPriority>" + std::to_string(renderableCircle.second->renderPriority) + "</RenderableCircleRenderPriority>" + newline +
                tab + "  </RenderableCircle>" + newline;
        }

        return
            tab + "<RenderableCircles>" + newline +
            renderableCircleContent +
            tab + "</RenderableCircles>" + newline;
    }

    std::string XMLSystem::serializeRenderableSpriteMap(std::string tab) {
        std::string renderableSpriteContent = "";
        for (auto renderableSprite : renderableSpriteMap) {
            renderableSpriteContent +=
                tab + "  <RenderableSprite Name=\"" + renderableSprite.first + "\">" + newline +
                tab + "    <RenderableSpriteRenderLayer>" + std::to_string(renderableSprite.second->renderLayer) + "</RenderableSpriteRenderLayer>" + newline +
                tab + "    <RenderableSpriteRenderPriority>" + std::to_string(renderableSprite.second->renderPriority) + "</RenderableSpriteRenderPriority>" + newline +
                tab + "    <RenderableSpriteTextureFilePath>" + renderableSprite.second->textureFileName + "</RenderableSpriteTextureFilePath>" + newline +
                tab + "    <RenderableSpriteAnimationName>" + renderableSprite.second->animName + "</RenderableSpriteAnimationName>" + newline +
                tab + "  </RenderableSprite>" + newline;
        }

        return
            tab + "<RenderableSprites>" + newline +
            renderableSpriteContent +
            tab + "</RenderableSprites>" + newline;
    }

    std::string XMLSystem::serializeLevelMap() {
        std::string levelMapContent =  
            getXMLHeader(levelFirstChildElement, cmn::game->currentLevelPath) +
            "<" + levelFirstChildElement + ">" + newline;

        // For each entity in the level
        for (auto name_entity : levelMap) {

            // Grab the entity's data
            auto data = name_entity.second->component<Data>();

            // Check whether it has a prefab and is identical to it
            if (data->prefabName != "" && !data->modified) {

                // If so, only serialize the core components
                levelMapContent += serializeEntityComponents<Data, Transform, Rigidbody>(
                    *name_entity.second, "  ", nullptr, nullptr, nullptr);
            }
            else {
                // Otherwise, serialize the entire entity
                levelMapContent += serializeEntity(*name_entity.second, "  ");
            }
        }

        // Add the tail of the level's XML tag
        levelMapContent += "</" + levelFirstChildElement + ">" + newline;
    }

#pragma endregion

#pragma region Deserialization

    void XMLSystem::deserializeAssets(XMLNode* node) {
        deserializeTextureFilePathSet(node->FirstChildElement("Textures"));
        deserializeMusicFilePathSet(node->FirstChildElement("Music"));
        deserializeSoundFilePathSet(node->FirstChildElement("Sounds"));
        deserializeFontFilePathSet(node->FirstChildElement("Fonts"));
        deserializeLevelFilePathSet(node->FirstChildElement("Levels"));
        deserializeAnimationMap(node->FirstChildElement("Animations"));
        XMLElement* renderables = node->FirstChildElement("Renderables");
        deserializeRenderableTextMap(renderables->FirstChildElement("RenderableTexts"));
        deserializeRenderableRectangleMap(renderables->FirstChildElement("RenderableRectangles"));
        deserializeRenderableCircleMap(renderables->FirstChildElement("RenderableCircles"));
        deserializeRenderableSpriteMap(renderables->FirstChildElement("RenderableSprites"));
    }

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

    void XMLSystem::deserializeLevelFilePathSet(XMLNode* node) {
        deserializeFilePathSet(levelFilePathSet, "Levels", node);
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

    void XMLSystem::deserializeLevel(XMLNode* node) {
        XMLElement* item = node->FirstChildElement("Entity");
        while (item) {
            std::shared_ptr<ex::Entity> entity;
            XMLElement* data = item->FirstChildElement("Data");
            std::string prefabName = data->FirstChildElement("PrefabName")->GetText();
            bool modified;
            data->FirstChildElement("Modified")->QueryBoolText(&modified);
            // If it is a prefab
            if (prefabName != "" && !modified) {
                entity = instantiate(prefabName);
            }
            if (!entity) {
                entity.reset(new ex::Entity(cmn::game->entities.create()));
            }
            XMLSystem::deserializeEntity(*entity, item); //This will overwrite Data / Transform / Rigidbody information
            levelMap.insert(std::make_pair(entity->component<Data>()->name, entity));
            item = item->NextSiblingElement("Entity");
        }
    }

#pragma endregion

#pragma region Saving / Loading

    bool XMLSystem::saveAssets() {
        cout << "Attempting to save assets..." << endl;
        assetsDoc.Parse(serializeAssets().c_str());
        if (assetsDoc.SaveFile(assetsFileName.c_str()) != XML_NO_ERROR) {
            cerr << "WARNING: Assets Failed To Save!" << endl;
            return false;
        }
        else {
            cout << "Assets successfully saved." << endl;
            return true;
        }
    }

    bool XMLSystem::savePrefabs() {
        if (prefabsDoc.SaveFile(prefabsFileName.c_str()) != XML_NO_ERROR) {
            cerr << "WARNING: Prefabs Failed To Save!" << endl;
            return false;
        }
        else {
            cout << "Prefabs successfully saved." << endl;
            return true;
        }
    }

    bool XMLSystem::saveLevel(std::string levelPathName) {
        levelDoc.Parse(serializeLevelMap().c_str());
        if (levelDoc.SaveFile(cmn::game->currentLevelPath.c_str()) != XML_NO_ERROR) {
            cerr << "WARNING: Level Failed To Save!" << endl;
            return false;
        }
        else {
            cout << "Level successfully saved." << endl;
            return true;
        }
    }

    bool XMLSystem::loadLevel(std::string levelPathName, sf::Vector2f levelOffset = sf::Vector2f()) {
        cout << "Loading level..." << endl;
        if (levelDoc.LoadFile(levelPathName.c_str()) != XML_NO_ERROR) {
            cerr << "WARNING: Level entities failed to load!" << endl;
            return false;
        }
        else {
            cout << "Level entities successfully loaded." << endl;
            return true;
        }
        deserializeLevel(levelDoc.FirstChild());
    }

    void XMLSystem::clearEntities() {
        levelMap.clear();
    }

#pragma endregion

#pragma region (De)Serialization Utility Methods

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
            content += tab + "  <" + prefix + "FilePath>" + path + "</" + prefix + "FilePath>" + newline;
        }

        return
            tab + "<" + wrapperElement + ">" + newline +
            content +
            tab + "</" + wrapperElement + ">" + newline;
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

    std::string XMLSystem::getXMLHeader(std::string topLevelElement, std::string fileName) {
        return
            "<?xml version=\"" + xmlVersion + "\" encoding=\"" + xmlEncoding + "\"?>" + newline +
            "<!DOCTYPE " + topLevelElement + " SYSTEM \"" + fileName + "\">" + newline;
    }

    XMLElement* XMLSystem::findXMLEntity(XMLNode* top, std::string entityName) {
        XMLElement* entity = top->FirstChildElement("Entity");
        while (entity) {
            if (entity->FirstChildElement("Data")->FirstChildElement("Name")->GetText() == entityName) {
                return entity;
            }
            entity = entity->NextSiblingElement("Entity");
        }
        return nullptr;
    }

#pragma endregion

}

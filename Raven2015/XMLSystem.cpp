#include "XMLSystem.h"
#include "EntityLibrary.h"
#include "WidgetLibrary.h"

namespace Raven {

    XMLSystem::XMLSystem() : entityCounter(0) {}

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
            cout << "Game project successfully saved!" << endl;
        }
        else {
            cout << "A problem occurred during game project save." << endl;
        }
    }

    void XMLSystem::deserializeRavenGame() {
        cout << "Loading game project..." << endl;
        bool success = true;

        // Assets
        cout << "Step 1: Assets" << endl;
        success &= loadAssets();

        // Prefabs
        cout << "Step 2: Prefabs" << endl;
        success &= loadPrefabs();

        // Level
        cout << "Step 3: Level" << endl;
        success &= loadLevel(cmn::game->currentLevelPath, sf::Vector2f(), true);

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
        return
            tab + "<Entity>" + newline +
            serializeEntityComponents<COMPONENT_TYPE_LIST>(e, tab + "  ", COMPONENT_TYPES(::getNullPtrToType())) +
            tab + "</Entity>" + newline;
    }

    void XMLSystem::deserializeEntity(ex::Entity e, XMLNode* node) {
        EntityLibrary::clearEntity<COMPONENT_TYPE_LIST>(e, COMPONENT_TYPES(::getNullPtrToType()));
        deserializeEntityComponents<COMPONENT_TYPE_LIST>(e, node, COMPONENT_TYPES(::getNullPtrToType()));
        entitySet.insert(e);
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
    void XMLSystem::deserializeEntityComponents(ex::Entity e, XMLNode* node, C* c) {
        XMLElement* elem = node->FirstChildElement(c->getElementName().c_str());
        if (elem) {
            e.assign<C>()->deserialize(elem);
            cout << "Found Component. Deserializing... : " + c->getElementName() << endl;
        }
        else {
            cout << "Could not find component: " + c->getElementName() << endl;
        }
    }

    template <typename C, typename... Components>
    void XMLSystem::deserializeEntityComponents(ex::Entity e, XMLNode* node, C* c, Components*... components) {
        XMLElement* elem = node->FirstChildElement((c->getElementName()).c_str());
        if (elem) {
            e.assign<C>()->deserialize(elem);
            cout << "Found Component. Deserializing... : " + c->getElementName() << endl;
        }
        else {
            cout << "Could not find component: " + c->getElementName() << endl;
        }
        deserializeEntityComponents<Components...>(e, node, components...);
    }

#pragma endregion

#pragma region Events

#pragma region (De)Serialization Events

    void XMLSystem::receive(const XMLLoadEvent& e) {
        deserializeRavenGame();
    }

    void XMLSystem::receive(const XMLSaveEvent& e) {
        serializeRavenGame();
    }

#pragma endregion

    /*
#pragma region Data Management Events

    // Updates the name of entity instances and within XML documents. Still must
    // update GUI display with updated names before they will be shown to the user.
    void XMLSystem::receive(const XMLUpdateEntityNameEvent& event) {
        ex::Entity e = event.entity;
        std::string newName = event.newName;
        XMLDocument* doc = nullptr;

        XMLNode* top = nullptr;
        if (event.isPrefab) {
            top = prefabsDoc.FirstChildElement(prefabsFirstChildElement.c_str());
            doc = &prefabsDoc;
        }
        else {
            top = levelDoc.FirstChildElement(levelFirstChildElement.c_str());
            doc = &levelDoc;
        }
        XMLElement* entityNode = top->FirstChildElement("Entity");
        while (entityNode) {

            // Acquire the Entity's Data Node
            XMLElement* dataNode = entityNode->FirstChildElement("Data");
            // Acquire the Entity's Data
            auto data = e.component<Data>();
            // Acquire the Entity's Data's Name Node
            XMLElement* nameNode = dataNode->FirstChildElement("Name");
            // If we have found the entity we were looking for...
            if (nameNode->GetText() == data->name) {
                // Assign the new name
                nameNode->SetText(newName.c_str());
                entityNode = nullptr; // stop the searching loop
            }
            else {
                entityNode = entityNode->NextSiblingElement("Entity");
            }
        }

        // If the item is not a prefab, change the name of the current instance as well.
        if (!event.isPrefab) {
            for (auto entity : entitySet) {
                if (e.component<Data>()->id == id_entity.first) {
                    id_entity.second->component<Data>()->name = newName;
                    cout << "Instance name has been changed!" << endl;
                    break;
                }
            }
        }
    }

#pragma endregion
    */

#pragma endregion

#pragma region Prefab Manipulation

    bool XMLSystem::prefabExists(std::string prefabName) {
        XMLNode* top = prefabsDoc.FirstChildElement(prefabsFirstChildElement.c_str());
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

    ex::Entity XMLSystem::instantiate(std::string prefabName) {
        XMLNode* top = prefabsDoc.FirstChildElement(prefabsFirstChildElement.c_str());
        XMLElement* entityNode = top->FirstChildElement("Entity");
        while (entityNode) {
            XMLElement* data = entityNode->FirstChildElement("Data");
            if (data->FirstChildElement("Name")->GetText() == prefabName) {
                ex::Entity entity = EntityLibrary::Create::Entity(prefabName, false);
                deserializeEntity(entity, entityNode);
                return entity;
            }
            entityNode = entityNode->NextSiblingElement("Entity");
        }
        return ex::Entity();
    }

#pragma endregion

#pragma region Serialization

    std::string XMLSystem::serializeAssets() {
        std::string tab = "  ";
        return
            getXMLHeader(assetsFirstChildElement, assetsDesignFileName) +
            "<" + assetsFirstChildElement + ">" + newline +
            serializeTextureFilePathSet(tab) +
            serializeMusicFilePathSet(tab) +
            serializeSoundFilePathSet(tab) +
            serializeFontFilePathSet(tab) +
            serializeLevelFilePathSet(tab) +
            serializeAnimationMap(tab) +
            serializeRenderables(tab) +
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

    std::string XMLSystem::serializeRenderables(std::string tab) {
        return
            tab + "<Renderables>" + newline +
            serializeRenderableTextMap(tab + "  ") +
            serializeRenderableRectangleMap(tab + "  ") +
            serializeRenderableCircleMap(tab + "  ") +
            serializeRenderableSpriteMap(tab + "  ") +
            tab + "</Renderables>" + newline;
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

    std::string XMLSystem::serializeEntitySet() {
        std::string entitySetContent =  
            getXMLHeader(levelFirstChildElement, levelDesignFileName) +
            "<" + levelFirstChildElement + ">" + newline;

        // For each entity in the level (in reverse, else writes in opposite order each time)
        for (auto itr = entitySet.rbegin(); itr != entitySet.rend(); itr++) {

            auto entity = *itr;

            // Grab the entity's data
            auto data = entity.component<Data>();

            // Check whether it has a prefab and is identical to it
            if (data->prefabName != "NULL" && !data->modified) {

                entitySetContent += "  <Entity>\r\n";
                // If so, only serialize the core components to overwrite values from
                // the prefab that will be instantiated later on from XML
                entitySetContent += serializeEntityComponents<Data, Transform, Rigidbody>(
                    entity, "    ", nullptr, nullptr, nullptr);
                entitySetContent += "  </Entity>\r\n";
            }
            else {
                // Otherwise, serialize the entire entity to build from scratch later on
                entitySetContent += serializeEntity(entity, "  ");
            }

        }

        // Add the tail of the level's XML tag
        entitySetContent += "</" + levelFirstChildElement + ">" + newline;
        return entitySetContent;
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
        deserializeRenderables(node->FirstChildElement("Renderables"));
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

    void XMLSystem::deserializeRenderables(XMLNode* node) {
        deserializeRenderableTextMap(node->FirstChildElement("RenderableTexts"));
        deserializeRenderableRectangleMap(node->FirstChildElement("RenderableRectangles"));
        deserializeRenderableCircleMap(node->FirstChildElement("RenderableCircles"));
        deserializeRenderableSpriteMap(node->FirstChildElement("RenderableSprites"));
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

    void XMLSystem::deserializeEntitySet(XMLNode* node, sf::Vector2f levelOffset = sf::Vector2f(), bool clearEntitiesBeforehand = false) {
        if (clearEntitiesBeforehand) {
            entitySet.clear();
        }
        XMLElement* item = node->FirstChildElement("Entity");
        while (item) {
            ex::Entity entity;
            XMLElement* data = item->FirstChildElement("Data");
            std::string prefabName = data->FirstChildElement("PrefabName")->GetText();
            bool modified;
            data->FirstChildElement("Modified")->QueryBoolText(&modified);
            // If it is a prefab
            if (prefabName != "NULL" && !modified) {
                if (entity = instantiate(prefabName)) { // logged in entitySet simultaneously
                    EntityLibrary::clearCoreComponents(entity);
                    deserializeEntityComponents<Data, Transform, Rigidbody>(entity, item, nullptr, nullptr, nullptr);
                }
            }
            if (!entity.valid()) {
                entity = EntityLibrary::Create::Entity();
                deserializeEntity(entity, item); // logged in entitySet simultaneously
            }
            //entity->component<Data>()->name += " " + entityCounter++;
            entity.component<Transform>()->transform += levelOffset; // Place the entities at locations relative to the level origin
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
        cout << "Attempting to save prefabs..." << endl;
        // (Nothing to serialize since it is serialized in real-time)
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
        levelDoc.Parse(serializeEntitySet().c_str());
        if (levelDoc.SaveFile(cmn::game->currentLevelPath.c_str()) != XML_NO_ERROR) {
            cerr << "WARNING: Level Failed To Save!" << endl;
            return false;
        }
        else {
            cout << "Level successfully saved." << endl;
            return true;
        }
    }

    bool XMLSystem::loadAssets() {
        if (assetsDoc.LoadFile(assetsFileName.c_str()) != XML_NO_ERROR) {
            cerr << "WARNING: " + getNameFromFilePath(assetsFileName, true) + " failed to load!" << endl;
            return false;
        }
        else {
            cout << getNameFromFilePath(assetsFileName, true) + " successfully loaded. Deserializing..." << endl;
            deserializeAssets(assetsDoc.FirstChildElement(assetsFirstChildElement.c_str()));
            return true;
        }
    }

    bool XMLSystem::loadPrefabs() {
        if (prefabsDoc.LoadFile(prefabsFileName.c_str()) != XML_NO_ERROR) {
            cerr << "WARNING: " + getNameFromFilePath(prefabsFileName, true) + " failed to load!" << endl;
            return false;
        }
        else {
            cout << getNameFromFilePath(prefabsFileName, true) + " successfully loaded. Deserializing..." << endl;
            return true;
        }
    }

    bool XMLSystem::loadLevel(std::string levelFilePath, sf::Vector2f levelOffset = sf::Vector2f(), bool clearEntitiesBeforehand = false) {
        cout << "Loading level..." << endl;
        levelFilePathSet.insert(levelFilePath); // As a "set", it will already not add it if already present
        if (levelDoc.LoadFile(levelFilePath.c_str()) != XML_NO_ERROR) {
            cerr << "WARNING: Level " + getNameFromFilePath(levelFilePath, true) + " failed to load!" << endl;
            return false;
        }
        else {
            deserializeEntitySet(levelDoc.FirstChildElement(levelFirstChildElement.c_str()), levelOffset, clearEntitiesBeforehand);
            cout << "Level " + getNameFromFilePath(levelFilePath, true) + " successfully loaded. Deserializing..." << endl;
            return true;
        }
    }

#pragma endregion

#pragma region (De)Serialization Utility Methods

    std::string XMLSystem::getNameFromFilePath(std::string assetFilePath, bool includeExtension) {
        auto nameStart = assetFilePath.find_last_of('/')+1;
        if (!includeExtension) {
            return assetFilePath.substr(nameStart);
        }
        else {
            return assetFilePath.substr(nameStart, assetFilePath.find_last_of('.') - nameStart);
        }
    }

    std::string XMLSystem::serializeFilePathSet(std::set<std::string>& filePathSet, std::string wrapperElement, std::string tab) {
        std::string content = "";
        std::string prefix = (wrapperElement == "Music" ? wrapperElement : wrapperElement.substr(0, wrapperElement.size() - 1));
        for (auto path : filePathSet) {
            content += tab + "  <" + prefix + "FilePath>" + path + "</" + prefix + "FilePath>" + newline;
        }

        return
            tab + "<" + wrapperElement + ">" + newline +
            content +
            tab + "</" + wrapperElement + ">" + newline;
    }

    void XMLSystem::deserializeFilePathSet(std::set<std::string>& filePathSet, std::string wrapperElement, XMLNode* node) {
        filePathSet.clear();
        std::string prefix = (wrapperElement == "Music" ? wrapperElement : wrapperElement.substr(0, wrapperElement.size() - 1));
        XMLElement* path = node->FirstChildElement((prefix + "FilePath").c_str());
        while (path) {
            filePathSet.insert(path->GetText());
            path = node->NextSiblingElement((prefix + "FilePath").c_str());
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

    ex::Entity XMLSystem::findEntityByName(std::string entityName) {
        for (auto entity : entitySet) {
            if (entityName == entity.component<Data>()->name) {
                return entity;
            }
        }
        return ex::Entity();
    }

    void XMLSystem::clearNonPersistentEntities() {
        for (auto entity : entitySet) {
            if (!entity.component<Data>()->persistent) {
                entitySet.erase(entity);
            }
        }
    }

#pragma endregion

}

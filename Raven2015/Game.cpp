
#include "Common.h"
#include "Game.h"
#include "entityx/entityx.h"
#include "SFML/Graphics.hpp"
#include "MovementSystem.h"
#include "AudioSystem.h"
#include "CollisionSystem.h"
#include "InputSystem.h"
#include "GUISystem.h"
#include "RenderingSystem.h"
#include "XMLSystem.h"
#include "entityx/deps/Dependencies.h"
#include "EntityLibrary.h"

namespace Raven {

    Game::Game(sf::RenderTarget& target) : Game() {

    }

    Game::Game() : EntityX(), editMode(true), defaultLevelPath("Resources/XML/DefaultLevel.xml"), currentLevelPath(defaultLevelPath) {
        systems.add<XMLSystem>();
        systems.add<MovementSystem>();  // No dependencies
        systems.add<AudioSystem>();     // No dependencies
        systems.add<CollisionSystem>(); // No dependencies
        systems.add<InputSystem>();     // No dependencies
        systems.add<GUISystem>(systems.system<InputSystem>());                  // Required that this comes after InputSystem
        systems.add<RenderingSystem>(systems.system<GUISystem>());              // Required that this comes after GUISystem
        systems.add<ex::deps::Dependency<Rigidbody, Transform>>();
        systems.add<ex::deps::Dependency<BoxCollider, Rigidbody, Transform>>();
        systems.configure();

        cmn::game = this;
    }

    void Game::initialize() {
        load();
        auto xml = systems.system<XMLSystem>();
        auto gui = systems.system<GUISystem>();
        gui->populatePrefabList(xml->prefabsDoc);
        gui->populateSceneHierarchy(xml->levelMap);
    }

    void Game::loadLevel(std::string levelFilePath = "", sf::Vector2f levelOffset = sf::Vector2f(), bool clearEntitiesBeforehand = false) {
        if (levelFilePath == "") {
            levelFilePath = currentLevelPath;
        }
        systems.system<XMLSystem>()->loadLevel(levelFilePath, levelOffset, clearEntitiesBeforehand);
        systems.system<GUISystem>()->populateSceneHierarchy(systems.system<XMLSystem>()->levelMap);
    }

    void Game::saveLevel() {
        systems.system<XMLSystem>()->saveLevel(currentLevelPath);
    }

    void Game::addLevel(std::string levelFilePath) {
        systems.system<XMLSystem>()->levelFilePathSet.insert(levelFilePath);
        // Doesn't add it to the file system (must first save the level)
    }

    void Game::removeLevel(std::string levelFilePath) {
        systems.system<XMLSystem>()->levelFilePathSet.erase(levelFilePath);
        // Doesn't remove it from the file system
    }

    void Game::updateGameMode(ex::TimeDelta dt) {
        systems.update<InputSystem>(dt);     // process new instructions for entities
        systems.update<MovementSystem>(dt);  // move entities
        systems.update<CollisionSystem>(dt); // check whether entities are now colliding
        systems.update<RenderingSystem>(dt); // draw all entities to the Canvas
        systems.update<GUISystem>(dt);       // update and draw GUI widgets
    }

    void Game::updateEditMode(ex::TimeDelta dt) {
        systems.update<RenderingSystem>(dt); // draw all entities to the Canvas
        systems.update<GUISystem>(dt);       // update and draw GUI widgets
    }

    ex::Entity Game::makeEntity(std::string name = "") {
        std::shared_ptr<ex::Entity> e(&EntityLibrary::Create::Entity(name));
        if (name != "") {
            systems.system<XMLSystem>()->levelMap.insert(
                std::make_pair(name, e));
        }
        return *e;
    }

    std::shared_ptr<ex::Entity> Game::instantiatePrefab(std::string name, std::string prefabName) {
        auto entity = systems.system<XMLSystem>()->instantiate(prefabName);
        if (entity == nullptr) {
            cerr << "Warning: Attempted to instantiate entity \"" + name + "\" from non-existent prefab \"" + prefabName + "\"." << endl;
            cerr << "         Entity will not be instantiated." << endl;
        }
        return entity;
    }

    // Serializes game content into the Raven XML document
    void Game::save() {
        events.emit<XMLSaveEvent>();
    }

    // Deserializes game content from the Raven XML document
    void Game::load() {
        events.emit<XMLLoadEvent>();
    }

    void Game::clearEntities() {
        std::map<std::string, std::shared_ptr<ex::Entity>> tempMap;
        auto firstMap = systems.system<XMLSystem>()->levelMap;
        for (auto name_entity : firstMap) {
            if (name_entity.second->component<Data>()->persistent) {
                tempMap.insert(name_entity);
            }
        }
    }
}

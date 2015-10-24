
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

    Game::Game() : editMode(true) {
        systems.add<MovementSystem>();  // No dependencies
        systems.add<AudioSystem>();     // No dependencies
        systems.add<CollisionSystem>(); // No dependencies
        systems.add<InputSystem>();     // No dependencies
        systems.add<GUISystem>(systems.system<InputSystem>());                  // Required that this comes after InputSystem
        systems.add<RenderingSystem>(systems.system<GUISystem>());              // Required that this comes after GUISystem
        systems.add<XMLSystem>();
        systems.add<ex::deps::Dependency<Rigidbody, Transform>>();
        systems.add<ex::deps::Dependency<BoxCollider, Rigidbody, Transform>>();
        systems.configure();

        cmn::entities = &entities;
        cmn::events = &events;
        cmn::game = this;
    }

    void Game::updateGameMode(ex::TimeDelta dt) {
        systems.update<InputSystem>(dt);     // process new instructions for entities
        systems.update<MovementSystem>(dt);  // move entities
        systems.update<CollisionSystem>(dt); // check whether entities are now colliding
        systems.update<RenderingSystem>(dt); // draw all entities to the Canvas
        systems.update<GUISystem>(dt);       // update and draw GUI widgets
    }

    void Game::updateEditMode(ex::TimeDelta dt) {
        //systems.update<RenderingSystem>(dt); // draw all entities to the Canvas
        systems.update<GUISystem>(dt);       // update and draw GUI widgets
    }

    ex::Entity Game::makeEntity() {
        std::shared_ptr<ex::Entity> e(&entities.create());
        std::string s = (e->assign<Data>()->name = "Default Entity");
        e->assign<Transform>();
        e->assign<Rigidbody>();
        systems.system<XMLSystem>()->levelMap[currentLevelName].insert(std::make_pair(s, e));
        return *e;
    }

    ex::Entity Game::makeEntity(std::string name) {
        if (name == "") {
            cerr << "Error: Attempted to assign empty string to entity name";
            throw 1;
        }
        ex::Entity e = makeEntity();
        e.component<Data>()->name = name;
        return e;
    }

    std::shared_ptr<ex::Entity> Game::instantiatePrefab(std::string name, std::string prefabName) {
        std::shared_ptr<ex::Entity> e(&entities.create());
        auto map = systems.system<XMLSystem>()->prefabMap;
        if (map.find(prefabName) == map.end()) {
            cerr << "Warning: Attempted to instantiate entity \"" + name + "\" from non-existent prefab \"" + prefabName + "\"" << endl;
            cerr << "         Entity will not be instantiated." << endl;
            return nullptr;
        }
        else {
            EntityLibrary::copyEntity(*e, *map[prefabName]);
            e->component<Data>()->name = name;
            systems.system<XMLSystem>()->levelMap[currentLevelName].insert(std::make_pair(name, e));
            return e;
        }
    }

}

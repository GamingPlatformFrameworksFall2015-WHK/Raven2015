
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
#include "entityx/deps/Dependencies.h"

namespace Raven {

    Game::Game(sf::RenderTarget& target) : Game() {

    }

    Game::Game() {
        systems.add<MovementSystem>();  // No dependencies
        systems.add<AudioSystem>();     // No dependencies
        systems.add<CollisionSystem>(); // No dependencies
        systems.add<InputSystem>();     // No dependencies
        systems.add<GUISystem>(systems.system<InputSystem>());                  // Required that this comes after InputSystem
        systems.add<RenderingSystem>(systems.system<GUISystem>());              // Required that this comes after GUISystem
        systems.add<ex::deps::Dependency<Rigidbody, Transform>>();
        systems.add<ex::deps::Dependency<BoxCollider, Rigidbody, Transform>>();
        systems.configure();
    }

    void Game::update(ex::TimeDelta dt) {
        systems.update<InputSystem>(dt);     // process new instructions for entities
        systems.update<MovementSystem>(dt);  // move entities
        systems.update<CollisionSystem>(dt); // check whether entities are now colliding
        systems.update<RenderingSystem>(dt); // draw all entities to the Canvas
        systems.update<GUISystem>(dt);       // update and draw GUI widgets
    }

}

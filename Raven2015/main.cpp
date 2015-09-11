/* 
 * Classname:   Gaming Platform Frameworks
 * Project:     Raven
 * Version:     1.0
 *
 * Copyright:   The contents of this document are the property of its creators.
 *              Reproduction or usage of it without permission is prohibited.
 *
 * Owners:      Will Nations
 *              Hailee Ammons
 *              Kevin Wang
 */

#pragma once

#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML\System.hpp"
#include "entityx/entityx.h"
#include "ComponentLibrary.h"
#include "MovementSystem.h"
#include "AudioSystem.h"
#include "CollisionSystem.h"
#include "entityx/deps/Dependencies.h"
#include "Entities\Entity.h"

using namespace Raven;

class Game : public ex::EntityX {
public:
    explicit Game(sf::RenderTarget &target) {
        systems.add<MovementSystem>();
        systems.add<AudioSystem>();
        systems.add<CollisionSystem>();
        systems.configure();
    }

    void update(ex::TimeDelta dt) {
        systems.update<MovementSystem>(dt);
        systems.update<AudioSystem>(dt);
        //systems.update<CollisionSystem>(dt);
    }
};

int main() {

    std::srand((unsigned int) std::time(nullptr));

    // Create game window
    sf::RenderWindow window(sf::VideoMode(600,400), "Raven Test");
    Game game(window);

    // Add dependencies
    cout << "Adding component dependencies..." << endl;
    game.systems.add<ex::deps::Dependency<Rigidbody, Transform>>();
    game.systems.add<ex::deps::Dependency<BoxCollider, Rigidbody, Transform>>();
    game.systems.add<ex::deps::Dependency<CircleCollider, Rigidbody, Transform>>();
    game.systems.configure();

    Entity e1 = game.entities.create();
    e1.assign<BoxCollider>();
    Entity e2 = game.entities.create();
    e2.assign<BoxCollider>();
    e2.component<BoxCollider>().get()->origin.x = 350;
    e2.assign<SoundMaker>("sample_audio_file.wav");

    cout << "Starting game loop..." << endl;
    sf::Clock mainClock;
    while (window.isOpen()) {

        sf::Event event;

        while (window.pollEvent(event)) {

            switch(event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:

                break;
            }
        }

        /*
         * Per iteration, clear the window, record delta time, update systems,
         * and redisplay.
         */
        window.clear();
        sf::Time deltaTime = mainClock.restart();
        game.update(deltaTime.asSeconds());
        window.display();
    }

    return 0;
}

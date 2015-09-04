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
#include <SFML/Graphics.hpp>
#include "entityx/entityx.h"
#include "ComponentLibrary.h"
#include "MovementSystem.h"
#include "entityx/deps/Dependencies.h"

using std::cout;
using std::cerr;
using std::endl;

namespace ex = entityx;

class Game : public ex::EntityX {
public:
    explicit Game(sf::RenderTarget &target) {
        systems.add<MovementSystem>();
        systems.configure();
    }

    void update(ex::TimeDelta dt) {
        systems.update<MovementSystem>(dt);
    }
};

int main() {

    std::srand((unsigned int) std::time(nullptr));

    sf::RenderWindow window(sf::VideoMode(600,400), "Raven Test");
    
    cout << "Adding component dependencies..." << endl;
    Game game(window);
    game.systems.add<ex::deps::Dependency<Rigidbody, Transform>>();

    cout << "Starting game loop..." << endl;
    while (window.isOpen()) {

        sf::Event event;

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.display();
    }

    return 0;
}

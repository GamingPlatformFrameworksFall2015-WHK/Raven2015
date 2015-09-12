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
#include <map>
#include <vector>
#include <string>
#include <exception>
#include "SFML/Graphics.hpp"
#include "SFML\System.hpp"
#include "entityx/entityx.h"
#include "ComponentLibrary.h"
#include "MovementSystem.h"
#include "AudioSystem.h"
#include "CollisionSystem.h"
#include "entityx/deps/Dependencies.h"

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
        systems.update<CollisionSystem>(dt);
    }
};

class InputHandler {
public:
    std::map<sf::Keyboard::Key, std::string> key_map{
        { sf::Keyboard::A,		"move_left" },
        { sf::Keyboard::D,		"move_right" },
        { sf::Keyboard::W,		"move_up" },
        { sf::Keyboard::S,		"move_down" },
        { sf::Keyboard::Left,	"move_left" },
        { sf::Keyboard::Right,	"move_right" },
        { sf::Keyboard::Up,		"move_up" },
        { sf::Keyboard::Down,	"move_down" },
        { sf::Keyboard::Escape, "exit" }
    };

    InputHandler() {
        //read config file
        //remap key_map so in config: 'move_left = A' => insert_input(A, move_left);
    }

    void insert_input(sf::Keyboard::Key key, std::string action) {
        try {
            if (key_map.at(key) != action) {
                key_map[key] = action;
            }
        }
        catch (std::exception e) {
            //If k does not match the key of any element in the container, the function throws an out_of_range exception.
            key_map.insert(std::pair<sf::Keyboard::Key, std::string>(key, action));
        }
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

    // Dependencies Verification + Create 2 Entities with colliders
    ex::Entity entity1 = game.entities.create();
    entity1.assign<BoxCollider>();
    if (entity1.has_component<Transform>()) {
        cout << "TRUE" << endl;
    }
    ex::Entity entity2 = game.entities.create();
    entity2.assign<BoxCollider>();

    // Graphics for now
    sf::CircleShape shape;
    shape.setRadius(32.f);
    shape.setPosition(entity1.component<Transform>().get()->transform.x, 
        entity1.component<Transform>().get()->transform.y);
    shape.setFillColor(sf::Color::Cyan);

    sf::CircleShape shape2;
    shape2.setRadius(32.f);
    shape2.setPosition(entity2.component<Transform>().get()->transform.x, 
        entity2.component<Transform>().get()->transform.y);
    shape2.setFillColor(sf::Color::Red);

    cout << "Starting game loop..." << endl;
    sf::Clock mainClock;
    while (window.isOpen()) {

        sf::Event event;

        while (window.pollEvent(event)) {

            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed: {
                InputHandler i_d;
                int speed = 10;
                if (i_d.key_map[event.key.code] == "move_right") {
                    entity1.component<Transform>().get()->transform.x += speed;
                }
                else if (i_d.key_map[event.key.code] == "move_down") {
                    entity1.component<Transform>().get()->transform.y += speed;
                }
                else if (i_d.key_map[event.key.code] == "move_left") {
                    entity1.component<Transform>().get()->transform.x -= speed;
                }
                else if (i_d.key_map[event.key.code] == "move_up") {
                    entity1.component<Transform>().get()->transform.y -= speed;
                }
                else if (i_d.key_map[event.key.code] == "exit") {
                    window.close();
                }
                break;
            }
            case sf::Event::MouseButtonPressed:
                break;
            case sf::Event::JoystickButtonPressed:
                break;
            case sf::Event::JoystickMoved:
                break;
            }
        }
        shape.setPosition(entity1.component<Transform>().get()->transform.x, 
            entity1.component<Transform>().get()->transform.y);

        /*
         * Per iteration, clear the window, record delta time, update systems,
         * and redisplay.
         */
        window.clear();
        window.draw(shape); //Graphic for now
        window.draw(shape2); //Graphic for now
        sf::Time deltaTime = mainClock.restart();
        game.update(deltaTime.asSeconds());
        window.display();
    }

    return 0;
}
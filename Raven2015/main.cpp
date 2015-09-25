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
#include "InputSystem.h"
#include "entityx/deps/Dependencies.h"

using namespace Raven;

class Game : public ex::EntityX {
public:
    explicit Game(sf::RenderTarget &target) {
        systems.add<MovementSystem>();
        systems.add<AudioSystem>();
        systems.add<CollisionSystem>();
		systems.add<InputSystem>();
        systems.configure();
    }

    void update(ex::TimeDelta dt) {
        systems.update<MovementSystem>(dt);
        systems.update<CollisionSystem>(dt);
		systems.update<InputSystem>(dt);
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
	entity1.assign<SoundMaker>();

    ex::Entity entity2 = game.entities.create();
    entity2.assign<BoxCollider>();
	entity2.assign<SoundMaker>();

    ex::Entity entity3 = game.entities.create();
    entity3.assign<BoxCollider>(cmn::STD_UNITX, cmn::STD_UNITY, 80.f, 80.f);

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

    sf::CircleShape shape3;
    shape3.setRadius(32.f);
    shape3.setPosition(80, 80);
    shape3.setFillColor(sf::Color::Green);

	//InputSystem creating
	InputSystem input;

    cout << "Starting game loop..." << endl;
    sf::Clock mainClock;
    while (window.isOpen()) {

        sf::Event event;

		while (window.pollEvent(event)) {
			game.systems.system<InputSystem>()->setEventType(event);
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed: {
				int speed = 10;
				if (input.getAction(event.key.code) == "move_right") {
					entity1.component<Transform>().get()->transform.x += speed;
				}
				else if (input.getAction(event.key.code) == "move_down") {
					entity1.component<Transform>().get()->transform.y += speed;
				}
				else if (input.getAction(event.key.code) == "move_left") {
					entity1.component<Transform>().get()->transform.x -= speed;
				}
				else if (input.getAction(event.key.code) == "move_up") {
					entity1.component<Transform>().get()->transform.y -= speed;
				}
				else if (input.getAction(event.key.code) == "exit") {
					window.close();
				}
				break;
			}
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
        window.draw(shape3); //Graphic for now
        //sf::Time deltaTime = mainClock.restart();
        //game.update(deltaTime.asSeconds());
        sf::Time deltaTime = mainClock.getElapsedTime();
        if (deltaTime.asSeconds() > 1) 
            game.update(mainClock.restart().asSeconds());
        window.display();
    }

    return 0;
}
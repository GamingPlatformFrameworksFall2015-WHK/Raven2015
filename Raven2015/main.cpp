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
<<<<<<< HEAD
=======
#include "RenderingSystem.h"
>>>>>>> master
#include "entityx/deps/Dependencies.h"

using namespace Raven;

class Game : public ex::EntityX {
public:
    explicit Game(sf::RenderTarget &target) {
        systems.add<MovementSystem>();
        systems.add<AudioSystem>();
        systems.add<CollisionSystem>();
<<<<<<< HEAD
		systems.add<InputSystem>();
=======
        systems.add<InputSystem>();
        systems.add<RenderingSystem>();
>>>>>>> master
        systems.configure();
    }

    void update(ex::TimeDelta dt) {
        systems.update<MovementSystem>(dt);
        systems.update<CollisionSystem>(dt);
<<<<<<< HEAD
		systems.update<InputSystem>(dt);
=======
        systems.update<InputSystem>(dt);
        systems.update<RenderingSystem>(dt);
>>>>>>> master
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
    game.systems.add <ex::deps::Dependency<Animator, SpriteRenderer>>();
    game.systems.configure();

    ex::Entity entity1 = game.entities.create();
    entity1.assign<BoxCollider>();
<<<<<<< HEAD
	entity1.assign<SoundMaker>();
=======
    entity1.assign<SpriteRenderer>("Resources/Textures/BlueDot_vibrating.png", cmn::ERenderingLayer::Foreground, 0);
    entity1.assign<Animator>("BlueDotIdle");
    game.systems.system<RenderingSystem>()->initialize(game.entities, window);
    game.systems.system<RenderingSystem>()->registerAnimation("BlueDotIdle", std::shared_ptr<Animation>(
        new Animation("Resources/Textures/BlueDot_vibrating.png", 2, true, 30.0)));
    game.systems.system<RenderingSystem>()->registerAnimation("BlueDotDamaged", std::shared_ptr<Animation>(
        new Animation("Resources/Textures/BlueDot_damaged.png", 4, true)));

    ex::Entity efps = game.entities.create();
    efps.assign<Transform>();
    efps.assign<TextRenderer>();
    ex::ComponentHandle<TextRenderer> renderer = efps.component<TextRenderer>();
    renderer->text.setPosition(400.0f, 50.0f);
    renderer->text.setColor(sf::Color::White);
    renderer->font.loadFromFile("Resources/Fonts/black_jack.ttf");
    renderer->text.setFont(renderer->font);
    renderer->text.setString(sf::String(std::string(std::to_string(40))));

    ex::Entity entity2 = game.entities.create();
    entity2.assign<BoxCollider>();
    entity2.assign<SoundMaker>();
    game.events.emit<AudioEvent>("Resources/Audio/Sounds/choose.ogg", entity2.component<SoundMaker>().get(),
        cmn::EAudioType::SOUND, cmn::EAudioOperation::LOAD, cmn::EAudioLoop::FALSE);
    game.events.emit<AudioEvent>("Resources/Audio/Sounds/choose.ogg", entity2.component<SoundMaker>().get(),
        cmn::EAudioType::SOUND, cmn::EAudioOperation::PLAY, cmn::EAudioLoop::UNCHANGED);

    std::shared_ptr<InputSystem> input = game.systems.system<InputSystem>();

    /*
    // Dependencies Verification + Create 2 Entities with colliders
    ex::Entity entity1 = game.entities.create();
    entity1.assign<Transform>();
>>>>>>> master

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
    */

	//InputSystem creating
	InputSystem input;

    cout << "Starting game loop..." << endl;
    sf::Clock mainClock;
    Timer fpsTimer;
    int fps = 0;
    while (window.isOpen()) {

        sf::Event event;

<<<<<<< HEAD
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
=======
        // Calculate FPS based on iterations game loop has completed in 1 second
        if (fpsTimer.getETime() >= 1.0) {
            // Update FPS display with fps value
            fpsTimer.reset();
            fps = 0;
        }
        else {
            fps++;
        }

        renderer->text.setString(sf::String(std::string(std::to_string(fps))));

        while (window.pollEvent(event)) {
            input->setEventType(event);
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed: {
                int speed = 10;
                if (input->getAction(event.key.code) == "move_right") {
                    entity1.component<Transform>().get()->transform.x += speed;
                }
                else if (input->getAction(event.key.code) == "move_down") {
                    entity1.component<Transform>().get()->transform.y += speed;
                }
                else if (input->getAction(event.key.code) == "move_left") {
                    entity1.component<Transform>().get()->transform.x -= speed;
                }
                else if (input->getAction(event.key.code) == "move_up") {
                    entity1.component<Transform>().get()->transform.y -= speed;
                }
                else if (input->getAction(event.key.code) == "exit") {
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
        /*shape.setPosition(entity1.component<Transform>().get()->transform.x, 
            entity1.component<Transform>().get()->transform.y);*/
>>>>>>> master

        /*
         * Per iteration, clear the window, record delta time, update systems,
         * and redisplay.
         */
        window.clear();
        //window.draw(shape); //Graphic for now
        //window.draw(shape2); //Graphic for now
        //window.draw(shape3); //Graphic for now
        //window.draw(sprite);
        sf::Time deltaTime = mainClock.restart();
        game.update(deltaTime.asSeconds());
        window.display();
    }

    
    return 0;
}
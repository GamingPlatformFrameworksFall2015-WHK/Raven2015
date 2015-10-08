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
#include "RenderingSystem.h"
#include "entityx/deps/Dependencies.h"

using namespace Raven;

class Game : public ex::EntityX {
public:
    explicit Game(sf::RenderTarget &target) {
        systems.add<MovementSystem>();
        systems.add<AudioSystem>();
        systems.add<CollisionSystem>();
        systems.add<InputSystem>();
        systems.add<RenderingSystem>();
        systems.add<ex::deps::Dependency<Rigidbody, Transform>>();
        systems.add<ex::deps::Dependency<BoxCollider, Rigidbody, Transform>>();
        systems.add<ex::deps::Dependency<CircleCollider, Rigidbody, Transform>>();
        systems.configure();
    }

    void update(ex::TimeDelta dt) {
        systems.update<MovementSystem>(dt); 
        systems.update<CollisionSystem>(dt); 
        systems.update<InputSystem>(dt);
        systems.update<RenderingSystem>(dt);
    }
};

int main() {

    std::srand((unsigned int)std::time(nullptr));

    // Create game window
    sf::RenderWindow window(sf::VideoMode(600, 400), "Raven Test");
    Game game(window);

    // This should all eventually get converted into XML, that way no "registration" is required
    ex::Entity entity1 = game.entities.create();
    entity1.assign<BoxCollider>();
    ex::ComponentHandle<Renderer> renderer = entity1.assign<Renderer>();
    renderer->sprites["BlueDot"].reset(new RenderableSprite(
        "Resources/Textures/BlueDot_vibrating.png", "BlueDotIdle", 0, cmn::ERenderingLayer::Foreground, 0));
    game.systems.system<RenderingSystem>()->initialize(game.entities, window);
    game.systems.system<RenderingSystem>()->registerAnimation("BlueDotIdle",
        new Animation("Resources/Textures/BlueDot_vibrating.png", 2, true, 30.0));
    game.systems.system<RenderingSystem>()->registerAnimation("BlueDotDamaged",
        new Animation("Resources/Textures/BlueDot_damaged.png", 4, true));

    ex::Entity efps = game.entities.create();
    efps.assign<Transform>();
    ex::ComponentHandle<Renderer> efps_renderer = efps.assign<Renderer>();
    std::string fpsStr = "FPS";
    efps_renderer->texts[fpsStr].reset(new RenderableText("", sf::Vector2f(400.0f, 50.0f),
        "Resources/Fonts/black_jack.ttf", sf::Color::White, cmn::ERenderingLayer::HUD));

    ex::Entity entity2 = game.entities.create();
    entity2.assign<BoxCollider>();
    entity2.assign<SoundMaker>();
    game.events.emit<AudioEvent>("Resources/Audio/Sounds/choose.ogg", entity2.component<SoundMaker>().get(),
        cmn::EAudioType::SOUND, cmn::EAudioOperation::AUDIO_LOAD, cmn::EAudioLoop::LOOP_FALSE);
    game.events.emit<AudioEvent>("Resources/Audio/Sounds/choose.ogg", entity2.component<SoundMaker>().get(),
        cmn::EAudioType::SOUND, cmn::EAudioOperation::AUDIO_PLAY, cmn::EAudioLoop::LOOP_UNCHANGED);

    std::shared_ptr<InputSystem> input = game.systems.system<InputSystem>();

    cout << "Starting game loop..." << endl;
    sf::Clock mainClock;
    Timer gameTimer;
	Timer fpsTimer;
	const double deltaTime = 0.01;
	int fps = 0;
	double currentTime = gameTimer.getElapsedTime();
	double accumulator = 0.0;
    while (window.isOpen()) {

        sf::Event event;

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

        /*
        * We have a fixed delta time split of 0.01 seconds which means that
		* we will only update our physics systems 100 times per second.
		* This seperates our rendering from our physics which will stabilize and 
		* smooth our frame rate.  The accumulator tracks any leftover time
		* that needs to be considered for the next update function so that
		* we do not get jittery displays.
        */
        
		//If FPS timer has surpassed 1 second, update FPS display and then reset FPS timer and counter
		if (fpsTimer.getElapsedTime() >= 1.0) {
			efps_renderer->texts["FPS"]->text.setString(sf::String(std::to_string(fps)));
			fpsTimer.restart();
			fps = 0;
		}

		double newTime = gameTimer.getElapsedTime();
		double frameTime = newTime - currentTime;
		currentTime = newTime;
		accumulator += frameTime;

		//If we have reached delta time value, update game systems, increment FPS counter,
		//subtract delta time from accumulator so we don't lose any leftover time,
		//and clear window to prepare for next display.
		while (accumulator >= deltaTime) {
			fps++;
			window.clear();
			game.update(frameTime);
			accumulator -= deltaTime;
		}
		
		window.display();
    }


    return 0;
}
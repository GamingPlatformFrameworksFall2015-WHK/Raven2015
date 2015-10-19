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
#include "SFML/System.hpp"
#include "entityx/entityx.h"
#include "ComponentLibrary.h"
#include "MovementSystem.h"
#include "AudioSystem.h"
#include "CollisionSystem.h"
#include "InputSystem.h"
#include "RenderingSystem.h"
#include "GUISystem.h"
#include "entityx/deps/Dependencies.h"
#include "Game.h"

using namespace Raven;

int main() {

    std::srand((unsigned int)std::time(nullptr));

    // Create EntityX-required game window (but won't be used)
    sf::RenderWindow requiredWindow;

    // Create EntityX manager object
    Game game(requiredWindow);
    std::shared_ptr<sf::RenderWindow> window(game.systems.system<GUISystem>()->mainWindow);

    // This should all eventually get converted into XML, that way no "registration" is required

    game.systems.system<RenderingSystem>()->initialize(game.entities);
    game.systems.system<RenderingSystem>()->registerAnimation("BlueDotIdle",
        new Animation("Resources/Textures/BlueDot_vibrating.png", 2, true, 100.0));
    game.systems.system<RenderingSystem>()->registerAnimation("BlueDotDamaged",
        new Animation("Resources/Textures/BlueDot_damaged.png", 4, true));


    //Create pawn entity that plaer will control
    ex::Entity pawnEntity = game.entities.create();
    pawnEntity.assign<Pawn>();
    pawnEntity.assign<BoxCollider>()->collisionSettings.insert(COLLISION_LAYER_SETTINGS_SOLID);
    ex::ComponentHandle<rvn::Renderer> pawnRend = pawnEntity.assign<rvn::Renderer>();
    pawnRend->sprites["BlueDot"].reset(new RenderableSprite(
        "Resources/Textures/BlueDot_vibrating.png", "BlueDotIdle", 0, cmn::ERenderingLayer::Foreground, 0));

    //Create tracker entity that will follow closest pawn
    ex::Entity trackerEntity = game.entities.create();
    trackerEntity.assign<Tracker>();
    trackerEntity.assign<Transform>()->transform.x = 400.0f;
    trackerEntity.component<Transform>()->transform.y = 5.0f;
    trackerEntity.assign<BoxCollider>()->collisionSettings.insert(COLLISION_LAYER_SETTINGS_SOLID);
    ex::ComponentHandle<rvn::Renderer> trackerRend = trackerEntity.assign<rvn::Renderer>();
    trackerRend->sprites["BlueDot"].reset(new RenderableSprite(
        "Resources/Textures/BlueDot_vibrating.png", "BlueDotIdle", 0, cmn::ERenderingLayer::Foreground, 0));

    //Create vertical pacer
    ex::Entity vertPacerEntity = game.entities.create();
    vertPacerEntity.assign<Pacer>(VERT_PATH, sf::Vector2f(200.0f, 200.0f), 20.0f);
    vertPacerEntity.assign<Transform>()->transform.x = 200.0f;
    vertPacerEntity.component<Transform>()->transform.y = 200.0f;
    vertPacerEntity.assign<Rigidbody>();
    ex::ComponentHandle<rvn::Renderer> vertPacerRend = vertPacerEntity.assign<rvn::Renderer>();
    vertPacerRend->sprites["BlueDot"].reset(new RenderableSprite(
        "Resources/Textures/BlueDot_vibrating.png", "BlueDotIdle", 0, cmn::ERenderingLayer::Foreground, 0));

    //Create horizontal pacer
    ex::Entity horPacerEntity = game.entities.create();
    horPacerEntity.assign<Pacer>(HOR_PATH, sf::Vector2f(350.0f, 500.0f), 50.0f);
    horPacerEntity.assign<Transform>()->transform.x = 350.0f;
    horPacerEntity.component<Transform>()->transform.y = 300.0f;
    horPacerEntity.assign<Rigidbody>();
    ex::ComponentHandle<rvn::Renderer> horPacerRend = horPacerEntity.assign<rvn::Renderer>();
    horPacerRend->sprites["BlueDot"].reset(new RenderableSprite(
        "Resources/Textures/BlueDot_vibrating.png", "BlueDotIdle", 0, cmn::ERenderingLayer::Foreground, 0));

    ex::Entity efps = game.entities.create();
    efps.assign<Transform>(400.0f, 50.0f, 90.0f);
    ex::ComponentHandle<rvn::Renderer> efps_renderer = efps.assign<rvn::Renderer>();
    std::string fpsStr = "FPS";
    efps_renderer->texts[fpsStr].reset(new RenderableText("100", sf::Vector2f(400.0f, 50.0f),
        "Resources/Fonts/black_jack.ttf", sf::Color::White, cmn::ERenderingLayer::HUD));

    XMLDocument doc;                //The document to process the string
    XMLPrinter printer;             //The printer that allows us to print to stdout
    static std::string xml;         //The xml string. Must be static to be parsed by TinyXML-2
    const std::string EOLN = "\r\n";//The newline character(s) to be used during parsing
    std::string tab = "";           //The current level of tabbing executed
    const std::string tinc = "  ";  //tab increment length

    ex::Entity entity2 = game.entities.create();
    entity2.assign<BoxCollider>();
    entity2.assign<SoundMaker>();
    game.events.emit<AudioEvent>("Resources/Audio/Sounds/choose.ogg", entity2.component<SoundMaker>().get(),
        cmn::EAudioType::SOUND, cmn::EAudioOperation::AUDIO_LOAD, cmn::EAudioLoop::LOOP_FALSE);
    game.events.emit<AudioEvent>("Resources/Audio/Sounds/choose.ogg", entity2.component<SoundMaker>().get(),
        cmn::EAudioType::SOUND, cmn::EAudioOperation::AUDIO_PLAY, cmn::EAudioLoop::LOOP_UNCHANGED);


	//std::shared_ptr<ex::EntityManager> entities(game.systems.system<GUISystem>()->entities);
    std::shared_ptr<InputSystem> input = game.systems.system<InputSystem>();

    cout << "Starting game loop..." << endl;
    sf::Clock mainClock;
    Timer fpsTimer;
    Timer gameTimer;
    double currentTime = 0.0;
    double accumulator = 0.0;
    int fps = 0;
    while (game.systems.system<GUISystem>()->isMainWindowOpen()) {

        // Calculate FPS based on iterations game loop has updated in 1 second
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
        while (accumulator >= FPS_100_TICK_TIME) {
            game.systems.system<GUISystem>()->pollEvents();
            game.systems.system<GUISystem>()->clear();
            game.update(frameTime);
            fps++;
            accumulator -= FPS_100_TICK_TIME;
        }

        game.systems.system<GUISystem>()->display();
    }

    return 0;
}

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

#include <iostream>             // For std::cout, std::cerr, std::endl
#include "SFML/System.hpp"      // For sf::RenderWindow
#include "Common.h"             // For FPS constants
#include "DataAssetLibrary.h"   // For rvn::Timer
#include "Game.h"               // For rvn::Game

using namespace Raven;

int main() {

    std::srand((unsigned int)std::time(nullptr));

    // Create EntityX-required game window (but won't be used)
    sf::RenderWindow requiredWindow;

    // Create EntityX manager object
    Game game(requiredWindow);
    std::shared_ptr<sf::RenderWindow> window = game.getWindow();

    /*
    // This should all eventually get converted into XML, that way no "registration" is required
    game.systems.system<RenderingSystem>()->initialize(game.entities);
    game.systems.system<RenderingSystem>()->registerAnimation("BlueDotIdle",
        new Animation("Resources/Textures/BlueDot_vibrating.png", 4, true, 100.0));
    game.systems.system<RenderingSystem>()->registerAnimation("Bomb",
        new Animation("Resources/Textures/Circle_Flashing_YellowOrangeRed.png", 4, true));

    //Create pawn entity that player will control
    ex::Entity pawnEntity = EntityLibrary::Create::Entity("Player");
    pawnEntity.assign<Pawn>();
    pawnEntity.assign<BoxCollider>()->collisionSettings.insert(COLLISION_LAYER_SETTINGS_SOLID);
    ex::ComponentHandle<rvn::Renderer> pawnRend = pawnEntity.assign<rvn::Renderer>();
    RenderableSprite* pawnSprite = new RenderableSprite("Resources/Textures/BlueDot_vibrating.png", "BlueDotIdle", 0, 0.f,0.f, cmn::ERenderingLayer::Foreground, 0);
    pawnRend->sprites.insert(std::make_pair("BlueDot", std::shared_ptr<RenderableSprite>(pawnSprite)));
        //"Resources/Textures/BlueDot_vibrating.png", "BlueDotIdle", 0, 0.f,0.f, cmn::ERenderingLayer::Foreground, 0))));
    cout << "Initial offsetX pawnSprite: " << pawnSprite->offsetX << endl;
    cout << "Initial offsetX for pawnEntity's sprite: " << pawnEntity.component<rvn::Renderer>()->sprites["BlueDot"]->offsetX << endl;

    //Create tracker entity that will follow closest pawn
    ex::Entity trackerEntity = EntityLibrary::Create::Entity("First Tracker");
    trackerEntity.assign<Tracker>();
    trackerEntity.component<Transform>()->transform.x = 400.0f;
    trackerEntity.component<Transform>()->transform.y = 5.0f;
    trackerEntity.assign<BoxCollider>()->collisionSettings.insert(COLLISION_LAYER_SETTINGS_SOLID);
    ex::ComponentHandle<rvn::Renderer> trackerRend = trackerEntity.assign<rvn::Renderer>();
    trackerRend->sprites["BlueDot"].reset(new RenderableSprite(
        "Resources/Textures/BlueDot_vibrating.png", "BlueDotIdle", 0, 0.f,0.f, cmn::ERenderingLayer::Foreground, 0));

    //Create vertical pacer
    ex::Entity vertPacerEntity = EntityLibrary::Create::Entity("Vertical Pacer");
    vertPacerEntity.assign<Pacer>(Pacer::Direction::VERTICAL, sf::Vector2f(200.0f, 200.0f), 20.0f);
    vertPacerEntity.component<Transform>()->transform.x = 200.0f;
    vertPacerEntity.component<Transform>()->transform.y = 200.0f;
    vertPacerEntity.component<Rigidbody>();
    ex::ComponentHandle<rvn::Renderer> vertPacerRend = vertPacerEntity.assign<rvn::Renderer>();
    vertPacerRend->sprites["BlueDot"].reset(new RenderableSprite(
        "Resources/Textures/BlueDot_vibrating.png", "BlueDotIdle", 0, 0.f,0.f, cmn::ERenderingLayer::Foreground, 0));

    //Create horizontal pacer
    ex::Entity horPacerEntity = EntityLibrary::Create::Entity("Horizontal Pacer");
    horPacerEntity.assign<Pacer>(Pacer::Direction::HORIZONTAL, sf::Vector2f(350.0f, 500.0f), 50.0f);
    horPacerEntity.component<Transform>()->transform.x = 350.0f;
    horPacerEntity.component<Transform>()->transform.y = 300.0f;
    horPacerEntity.component<Rigidbody>();
    ex::ComponentHandle<rvn::Renderer> horPacerRend = horPacerEntity.assign<rvn::Renderer>();
    horPacerRend->sprites["BlueDot"].reset(new RenderableSprite(
        "Resources/Textures/BlueDot_vibrating.png", "BlueDotIdle", 0, 0.f,0.f, cmn::ERenderingLayer::Foreground, 0));

    ex::Entity efps = EntityLibrary::Create::Entity("Frame Rate");
    efps.component<Transform>()->transform = sf::Vector2f(400.0f, 50.0f);
    efps.component<Transform>()->rotation = 90.0f;
    ex::ComponentHandle<rvn::Renderer> efps_renderer = efps.assign<rvn::Renderer>();
    std::string fpsStr = "FPS";
    efps_renderer->texts[fpsStr].reset(new RenderableText("100", sf::Vector2f(400.0f, 50.0f),
        "Resources/Fonts/black_jack.ttf", sf::Color::White, 0.f, 0.f, cmn::ERenderingLayer::HUD));

    XMLDocument doc;                //The document to process the string
    XMLPrinter printer;             //The printer that allows us to print to stdout
    static std::string xml;         //The xml string. Must be static to be parsed by TinyXML-2
    const std::string EOLN = "\r\n";//The newline character(s) to be used during parsing
    std::string tab = "";           //The current level of tabbing executed
    const std::string tinc = "  ";  //tab increment length

    ex::Entity entity2 = EntityLibrary::Create::Entity("Sound Tester");
    entity2.assign<BoxCollider>();
    entity2.assign<SoundMaker>();
    game.events.emit<AudioEvent>("Resources/Audio/Sounds/choose.ogg", entity2.component<SoundMaker>().get(),
        cmn::EAudioType::SOUND, cmn::EAudioOperation::AUDIO_LOAD, cmn::EAudioLoop::LOOP_FALSE);
    game.events.emit<AudioEvent>("Resources/Audio/Sounds/choose.ogg", entity2.component<SoundMaker>().get(),
        cmn::EAudioType::SOUND, cmn::EAudioOperation::AUDIO_PLAY, cmn::EAudioLoop::LOOP_UNCHANGED);

    game.entities.each<rvn::Renderer>([&](ex::Entity entity, rvn::Renderer& renderer) {
        cout << entity.id().id() << ": " << entity.component<Data>()->name << endl;
    });

    //std::shared_ptr<ex::EntityManager> entities(game.systems.system<GUISystem>()->entities);
    std::shared_ptr<InputSystem> input = game.systems.system<InputSystem>();
    */

    game.initialize();
    game.save();
    cout << "Saved game" << endl;

    cout << "Starting game loop..." << endl;
    sf::Clock mainClock;
    Timer fpsTimer;
    Timer gameTimer;
    double currentTime = 0.0;
    double accumulator = 0.0;
    int fps = 0;
    while (game.isMainWindowOpen()) {

        // Calculate FPS based on iterations game loop has updated in 1 second
        if (fpsTimer.getElapsedTime() >= 1.0) {
            //efps_renderer->texts["FPS"]->text.setString(sf::String(std::to_string(fps)));
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
            game.pollEvents();
            game.clearWindow();
            game.editMode ? game.updateEditMode(frameTime) : game.updateGameMode(frameTime);
            //game.updateGameMode(frameTime);
            fps++;
            accumulator -= FPS_100_TICK_TIME;
			/*if (window.get()->pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					window.get()->close();
				}
			}*/
        }

        game.displayWindow();
    }

    return 0;
}

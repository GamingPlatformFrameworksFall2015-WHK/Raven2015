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

using namespace Raven;

class Game : public ex::EntityX {
public:
    explicit Game(sf::RenderTarget &target) : Game() {}

    explicit Game() {
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

    void update(ex::TimeDelta dt) {
        systems.update<InputSystem>(dt);     // process new instructions for entities
        systems.update<MovementSystem>(dt);  // move entities
        systems.update<CollisionSystem>(dt); // check whether entities are now colliding
        systems.update<RenderingSystem>(dt); // draw all entities to the Canvas
        systems.update<GUISystem>(dt);       // update and draw GUI widgets
    }
};

int main() {

    std::srand((unsigned int)std::time(nullptr));

    // Create EntityX-required game window (but won't be used)
    sf::RenderWindow requiredWindow;

    // Create EntityX manager object
    Game game(requiredWindow);
    std::shared_ptr<sf::RenderWindow> window(game.systems.system<GUISystem>()->mainWindow);

    // This should all eventually get converted into XML, that way no "registration" is required
    ex::Entity entity1 = game.entities.create();
    entity1.assign<BoxCollider>();
    ex::ComponentHandle<rvn::Renderer> renderer = entity1.assign<rvn::Renderer>();
    renderer->sprites["BlueDot"].reset(new RenderableSprite(
        "Resources/Textures/BlueDot_vibrating.png", "BlueDotIdle", 0, cmn::ERenderingLayer::Foreground, 0));
    game.systems.system<RenderingSystem>()->initialize(game.entities);
    game.systems.system<RenderingSystem>()->registerAnimation("BlueDotIdle",
        new Animation("Resources/Textures/BlueDot_vibrating.png", 2, true, 30.0));
    game.systems.system<RenderingSystem>()->registerAnimation("BlueDotDamaged",
        new Animation("Resources/Textures/BlueDot_damaged.png", 4, true));

    ex::Entity efps = game.entities.create();
    efps.assign<Transform>(400.0f, 50.0f, 90.0f);
    ex::ComponentHandle<rvn::Renderer> efps_renderer = efps.assign<rvn::Renderer>();
    std::string fpsStr = "FPS";
    efps_renderer->texts[fpsStr].reset(new RenderableText("40", sf::Vector2f(400.0f, 50.0f),
        "Resources/Fonts/black_jack.ttf", sf::Color::White, cmn::ERenderingLayer::HUD));

    XMLDocument doc;                //The document to process the string
    XMLPrinter printer;             //The printer that allows us to print to stdout
    static std::string xml;         //The xml string. Must be static to be parsed by TinyXML-2
    const std::string EOLN = "\r\n";//The newline character(s) to be used during parsing
    std::string tab = "";           //The current level of tabbing executed
    const std::string tinc = "  ";  //tab increment length

    // The following code successfully stores the contents of a Transform component into an XMLDocument object
    xml = "<?xml version=\"1.0\"?>" + EOLN +
        "<!DOCTYPE RAVEN SYSTEM \"raven.dtd\">" + EOLN +
        "<RAVEN>" + EOLN;
    xml += efps.component<Transform>()->serialize(tab += tinc);
    xml += "</RAVEN>";
    doc.Parse(xml.c_str());
    doc.Print(&printer);
    cout << printer.CStr() << endl;
    /******************************************************************************************/

    // The following code successfully loads a Transform component from the XMLDocument content
    XMLNode* node = doc.FirstChildElement("RAVEN")->FirstChild();
    ex::Entity e = game.entities.create();

    e.assign<Transform>();
    cout << e.component<Transform>()->getElementName() << endl;
    cout << e.component<Transform>()->transform.x << endl;
    cout << e.component<Transform>()->transform.y << endl;
    cout << e.component<Transform>()->rotation << endl;

    e.component<Transform>()->deserialize(node);
    cout << e.component<Transform>()->getElementName() << endl;
    cout << e.component<Transform>()->transform.x << endl;
    cout << e.component<Transform>()->transform.y << endl;
    cout << e.component<Transform>()->rotation << endl;
    /*******************************************************************************************/

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
    Timer fpsTimer;
    int fps = 0;
    while (game.systems.system<GUISystem>()->isMainWindowOpen()) {

        // Calculate FPS based on iterations game loop has completed in 1 second
        if (fpsTimer.getElapsedTime() >= 1.0) {
            // Update FPS display with fps value
            fpsTimer.restart();
            fps = 0;
        }
        else {
            fps++;
        }

        game.systems.system<GUISystem>()->pollEvents();

        //
        // Per iteration, clear the window, record delta time, update systems,
        // and redisplay.
        //

        game.systems.system<GUISystem>()->clear();
        sf::Time deltaTime = mainClock.restart();
        game.update(deltaTime.asSeconds());
        game.systems.system<GUISystem>()->display();
    }


    return 0;
}
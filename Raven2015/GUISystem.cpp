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
#include "GUISystem.h"

namespace Raven {

    // Preset GameWindowType definitions
    const std::string GameWindowTypes::VIEWPORT_WINDOW = "Viewport";
    const std::string GameWindowTypes::EDITOR_WINDOW = "Editor";

    // Preset GUIWindowType definitions
    const std::string GUIWindowTypes::SCENE_HIERARCHY = "Scene Hierarchy";

    // Perform initializations
    GUISystem::GUISystem(std::shared_ptr<InputSystem> inputSystem) : input(inputSystem) {

        // Create all preset types of windows
        generateRenderWindow(GameWindowTypes::VIEWPORT_WINDOW,
            std::shared_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(600, 400), GameWindowTypes::VIEWPORT_WINDOW)));
        generateRenderWindow(GameWindowTypes::EDITOR_WINDOW,
            std::shared_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(600, 400), GameWindowTypes::EDITOR_WINDOW)));

        // Create all preset GUI windows within the editor window
        generateGUIWindow(GameWindowTypes::EDITOR_WINDOW, GUIWindowTypes::SCENE_HIERARCHY,
            std::shared_ptr<sfg::Window>(sfg::Window::Create()));

        // Position the Viewport and the Editor where you want them.
        sfWindowMap[GameWindowTypes::EDITOR_WINDOW]->setPosition(sf::Vector2i(20, 400));
        sfWindowMap[GameWindowTypes::VIEWPORT_WINDOW]->setPosition(sf::Vector2i(400, 400));
    }

    std::shared_ptr<sf::RenderWindow> GUISystem::generateRenderWindow(std::string name, std::shared_ptr<sf::RenderWindow> window) {
        // Keep a record of this exact window
        sfWindowMap.insert(std::make_pair(name, window));
        // Create an associated event to poll for events triggered by the window
        eventMap.insert(std::make_pair(name, std::shared_ptr<sf::Event>(new sf::Event())));
        // Create an associated desktop to maintain and update GUI windows in the new RenderWindow
        desktopMap.insert(std::make_pair(name, std::shared_ptr<sfg::Desktop>(new sfg::Desktop())));

        return window;
    }

    std::shared_ptr<sfg::Window> GUISystem::generateGUIWindow(std::string renderWindowName, 
        std::string guiWindowName, std::shared_ptr<sfg::Window> window) {

        // Save the key
        std::pair<std::string, std::string> key = std::make_pair(renderWindowName, guiWindowName);
        // Keep a record of this exact window
        sfgWindowMap.insert(std::make_pair(key, window));
        // Apply the name specified to the GUI window's label
        window->SetTitle(guiWindowName);
        // Add it to the desktop so that it will be updated
        desktopMap[renderWindowName]->Add(sfgWindowMap[key]); 

        return window;
    }

    bool GUISystem::pollEvents() {
        for (std::pair<std::string, std::shared_ptr<sf::RenderWindow>> name_window : sfWindowMap) {
            std::shared_ptr<sf::Event> e = eventMap[name_window.first];
            if (!name_window.second->pollEvent(*e)) {
                return false;
            }
            desktopMap[name_window.first]->HandleEvent(*e);

            //-----------------------------------------------
            // Call a function that encapsulates the event.type switch statement and simply returns the corresponding action.
            // Use the value returned by this function to then call another function that will emit an event that triggers the
            // corresponding action on all entities listening for that action via a Pawn component.
            //-----------------------------------------------

        }
        return true;
    }

    void GUISystem::update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) {
        // Update all GUI widgets associated with the set of GUI windows in all Desktops
        for (std::pair<std::string, std::shared_ptr<sfg::Desktop>> name_desktop : desktopMap) {
            desktopMap[name_desktop.first]->Update((float)dt);
        }
    }
}

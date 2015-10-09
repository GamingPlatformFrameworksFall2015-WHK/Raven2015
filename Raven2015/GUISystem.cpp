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

    // Preset WidgetName definitions
    const std::string GUIWidgetNames::SCENE_HIERARCHY = "Scene Hierarchy";
    const std::string GUIWidgetNames::COMPONENT_LIST = "Component List";
    const std::string GUIWidgetNames::ENTITY_DESIGNER = "Entity Designer";
    const std::string GUIWidgetNames::PREFAB_LIST = "Prefab List";
    const std::string GUIWidgetNames::VIEWPORT = "Viewport";

    // Preset GUIWindowType definitions
    const std::string GUIWidgetTypes::WIDGET = "Widget";
    const std::string GUIWidgetTypes::WINDOW = "Window";
    const std::string GUIWidgetTypes::CANVAS = "Canvas";
    const std::string GUIWidgetTypes::BUTTON = "Button";
    const std::string GUIWidgetTypes::BOX = "Box";

    // The preset name for the main window of the engine
    const std::string GUISystem::MAIN_WINDOW_NAME = "Raven";

    // Perform initializations
    GUISystem::GUISystem(std::shared_ptr<InputSystem> inputSystem) :
        mainWindow(new sf::RenderWindow(sf::VideoMode(600, 400), MAIN_WINDOW_NAME)),
        event(new sf::Event()),
        desktop(new sfg::Desktop()),
        sfgui(new sfg::SFGUI()),
        input(inputSystem) {

        // Create all preset widgets within the editor window
        generateWidget(GUIWidgetNames::SCENE_HIERARCHY,
            std::shared_ptr<sfg::Window>(sfg::Window::Create()));

        // Position the Viewport and the Editor where you want them.
        mainWindow->setPosition(sf::Vector2i(400, 400));
    }

    std::shared_ptr<sfg::Widget> GUISystem::generateWidget(
        std::string guiWindowName, std::shared_ptr<sfg::Widget> window) {

        // Keep a record of this exact window
        WidgetMap.insert(std::make_pair(guiWindowName, window));
        // Add it to the desktop so that it will be updated
        desktop->Add(WidgetMap[guiWindowName]); 

        return window;
    }

    void GUISystem::pollEvents() {
        while (mainWindow->pollEvent(*event)) {
            desktop->HandleEvent(*event);

            //-----------------------------------------------
            // Call a function that encapsulates the event.type switch statement and simply returns the corresponding action.
            // Use the value returned by this function to then call another function that will emit an event that triggers the
            // corresponding action on all entities listening for that action via a Pawn component.
            //-----------------------------------------------

        }
    }

    void GUISystem::update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) {
        // Update all GUI widgets associated with the set of GUI windows in all Desktops
        desktop->Update((float)dt);
    }
}

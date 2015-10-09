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

    const std::string GUISystem::MAIN_WINDOW_NAME = "Raven";

    // Perform initializations
    GUISystem::GUISystem(std::shared_ptr<InputSystem> inputSystem) :
        mainWindow(new sf::RenderWindow(sf::VideoMode(600, 400), MAIN_WINDOW_NAME)),
        event(new sf::Event()),
        desktop(new sfg::Desktop()),
        sfgui(new sfg::SFGUI()),
        input(inputSystem) {

        // Create the mainGUIWindow
        mainGUIWindow = makeWidget<sfg::Window>(cmn::PrimaryWidgetNames::MASTER_WINDOW);
        // Extend the mainGUIWindow to fill and expand throughout the entire window

        // Create the base Table
        table = makeWidget<sfg::Table>(cmn::PrimaryWidgetNames::MASTER_TABLE);

        // Assign dimensions and settings to the table
        // TODO:

        // Create the various windows that will go inside the table and allocate regions of space for them
        // Don't forget to modify the RenderingSystem to also incorporate draw calls to the viewport Canvas
        // TODO:
        auto viewport = makeWidget<sfg::Canvas>(cmn::PrimaryWidgetNames::VIEWPORT);
        
        // Add all of the various windows to the table
        // TODO:

        // Add the filled table to the mainGUIWindow
        mainGUIWindow->Add(table);

        // Position the Viewport and the Editor where you want them.
        mainWindow->setPosition(sf::Vector2i(400, 400));
    }

    template <class T>
    std::shared_ptr<T> GUISystem::makeWidget(std::string widgetName) {
        // Create an instance <- works because all Widgets have a static Create() factory method
        std::shared_ptr<T> widget(T::Create());
        // Keep a record of this exact window
        WidgetMap.insert(std::make_pair(widgetName, widget));
        // Add it to the desktop so that it will be updated
        desktop->Add(widget); 
        // Return the factory-generated widget pointer
        return widget;
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

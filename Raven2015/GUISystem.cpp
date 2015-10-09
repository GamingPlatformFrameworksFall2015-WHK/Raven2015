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

    // Perform initializations of what we CAN
    GUISystem::GUISystem(std::shared_ptr<InputSystem> inputSystem) :
        mainWindow(new sf::RenderWindow(
            sf::VideoMode((unsigned int)cmn::WINDOW_WIDTH, (unsigned int)cmn::WINDOW_HEIGHT),
            MAIN_WINDOW_NAME)),
        sfgui(new sfg::SFGUI()),
        desktop(new sfg::Desktop()),
        event(new sf::Event()),
        input(inputSystem) {

        mainWindow->resetGLStates(); // Without this, items will not be rendered properly immediately
        mainWindow->setPosition(sf::Vector2i(cmn::WINDOW_XPOS, cmn::WINDOW_YPOS));

        // Create the mainGUIWindow
        mainGUIWindow = makeWidget<sfg::Window>(cmn::PrimaryWidgetNames::MASTER_WINDOW);
        mainGUIWindow->SetTitle(cmn::PrimaryWidgetNames::MASTER_WINDOW);
        mainGUIWindow->SetRequisition(sf::Vector2f(cmn::WINDOW_WIDTH, cmn::WINDOW_HEIGHT));
        // Extend the mainGUIWindow to fill and expand throughout the entire window

        // Create the base Table
        auto table = makeWidget<sfg::Table>(cmn::PrimaryWidgetNames::MASTER_TABLE);
        table->SetRequisition(sf::Vector2f(cmn::WINDOW_WIDTH, cmn::WINDOW_HEIGHT));

        // Create the various windows that will go inside the table and allocate regions of space for them
        // Implement a 6x5 table with the following structure
        // Scene Hierarchy(SH), Content(C), Entity Designer (ED), Prefab List(PL), Viewport(V), Toolbar(T)
        //    0  1  2  3  4
        // 0|SH| V| V| V|PL|
        // 1|SH| V| V| V|PL|
        // 2|SH| V| V| V|PL|
        // 3|SH| T| T| T|PL|
        // 4| C| C|ED|ED|ED|
        // 5| C| C|ED|ED|ED|
        // TODO:
        auto viewport = formatViewport(makeWidget<sfg::Canvas>(cmn::PrimaryWidgetNames::VIEWPORT));
        auto sceneHierarchy = formatSceneHierarchy(makeWidget<sfg::ScrolledWindow>(cmn::PrimaryWidgetNames::SCENE_HIERARCHY));
        auto content = formatContent(makeWidget<sfg::Notebook>(cmn::PrimaryWidgetNames::CONTENT));
        auto toolbar = formatToolbar(makeWidget<sfg::Box>(cmn::PrimaryWidgetNames::TOOLBAR));
        auto entityDesigner = formatEntityDesigner(makeWidget<sfg::ScrolledWindow>(cmn::PrimaryWidgetNames::ENTITY_DESIGNER));
        auto prefabList = formatPrefabList(makeWidget<sfg::ScrolledWindow>(cmn::PrimaryWidgetNames::PREFAB_LIST));
        
        // Add all of the various windows to the table, assigning dimensions and settings to the table
        // TODO:
        sfg::Table::AttachOption all = (sfg::Table::AttachOption) (sfg::Table::FILL | sfg::Table::EXPAND);
        table->Attach(sceneHierarchy, sf::Rect<sf::Uint32>(0, 0, 1, 4), all, all);
        table->Attach(viewport, sf::Rect<sf::Uint32>(1, 0, 3, 3), all, all);
        table->Attach(prefabList, sf::Rect<sf::Uint32>(4, 0, 1, 4), all, all);
        table->Attach(toolbar, sf::Rect<sf::Uint32>(1, 3, 3, 1), all, all);
        table->Attach(content, sf::Rect<sf::Uint32>(0, 4, 2, 2), all, all);
        table->Attach(entityDesigner, sf::Rect<sf::Uint32>(2, 4, 3, 2), all, all);

        // Add the filled table to the mainGUIWindow
        mainGUIWindow->Add(table);
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

    // Format the Scene Hierarchy widget
    std::shared_ptr<sfg::ScrolledWindow> GUISystem::formatSceneHierarchy(std::shared_ptr<sfg::ScrolledWindow>) {

    }

    // Format the Component List widget
    std::shared_ptr<sfg::ScrolledWindow> GUISystem::formatComponentList(std::shared_ptr<sfg::ScrolledWindow>) {

    }
    
    // Format the Resources List widget
    std::shared_ptr<sfg::ScrolledWindow> GUISystem::formatResourcesList(std::shared_ptr<sfg::ScrolledWindow>) {

    }

    // Format the Content widget
    std::shared_ptr<sfg::Notebook> GUISystem::formatContent(std::shared_ptr<sfg::Notebook>) {

    }

    // Format the Entity Designer widget
    std::shared_ptr<sfg::ScrolledWindow> GUISystem::formatEntityDesigner(std::shared_ptr<sfg::ScrolledWindow>) {

    }

    // Format the Prefab List widget
    std::shared_ptr<sfg::ScrolledWindow> GUISystem::formatPrefabList(std::shared_ptr<sfg::ScrolledWindow>) {

    }
    
    // Format the Viewport widget
    std::shared_ptr<sfg::Canvas> GUISystem::formatViewport(std::shared_ptr<sfg::Canvas>) {

    }

    // Format the Toolbar widget
    std::shared_ptr<sfg::Box> GUISystem::formatToolbar(std::shared_ptr<sfg::Box>) {

    }


}

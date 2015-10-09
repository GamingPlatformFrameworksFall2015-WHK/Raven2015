/* Classname:   Gaming Platform Frameworks
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

#include "Common.h"
#include "entityx/System.h"
#include "EventLibrary.h"
#include <map>
#include <queue>
#include "SFGUI/SFGUI.hpp"
#include "SFGUI/Widgets.hpp"
#include "InputSystem.h"

namespace Raven {

    // Create a static wrapper for the various preset GUI window types
    struct GUIWidgetNames {
        const static std::string SCENE_HIERARCHY;   // The name for the list of named entities present in the scene
        const static std::string COMPONENT_LIST;    // The name for the list of possible components to add to an entity
        const static std::string ENTITY_DESIGNER;   // The name for the window which displays information about the selected entity
        const static std::string PREFAB_LIST;       // The name for the list of currently defined prefabs
        const static std::string VIEWPORT;          // The name for the view into the scene 
    };

    struct GUIWidgetTypes {
        const static std::string WIDGET;            // The name of the base abstract Widget class
        const static std::string WINDOW;            // The name of the single-Widget container class
        const static std::string BOX;               // The name of the multi-Widget container class
        const static std::string CANVAS;            // The name of the special Widget that can be drawn to with SFML
        const static std::string BUTTON;            // The name of the Widget that can be clicked to trigger an action
    };

    // A system class that manages the creation of windows, the display of windows (excluding "draw" calls handled
    // by the RenderingSystem), and the updating of GUI widgets within "Desktop" elements linked to RenderWindows
    class GUISystem : public ex::System<GUISystem>,
        public ex::Receiver<GUISystem> {
    public:

        // Perform initializations
        explicit GUISystem(std::shared_ptr<InputSystem> inputSystem);
        
        void clear() {
            mainWindow->clear();
        }

        void display() {
            sfgui->Display(*mainWindow);
            mainWindow->display();
        }

        // Subscribe to events, if any
        void configure(ex::EventManager &event_manager) {

        }

        //process CollisionEvents
        //void receive(const CollisionEvent &event);

        // Add or remove textures & sprites dynamically, drawing sprites that are within view
        void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override;

        // Shortcut method for acquiring the SceneHierarchy, a list of entities in the scene
        std::shared_ptr<sfg::Widget> getGUISceneHierarchy() { return WidgetMap[GUIWidgetNames::SCENE_HIERARCHY]; }

        // Shortcut method for acquiring the ComponentList, a list of the different types of components that exist
        std::shared_ptr<sfg::Widget> getGUIComponentList() { return WidgetMap[GUIWidgetNames::COMPONENT_LIST]; }

        // Shortcut method for acquiring the EntityDesigner, a window for tweaking structure and values in an Entity
        std::shared_ptr<sfg::Widget> getGUIEntityDesigner() { return WidgetMap[GUIWidgetNames::ENTITY_DESIGNER]; }

        // Shortcut method for acquiring the PrefabList, a list of specifically named Entities with a recorded form
        std::shared_ptr<sfg::Widget> getGUIPrefabList() { return WidgetMap[GUIWidgetNames::PREFAB_LIST]; }

        std::shared_ptr<sfg::Canvas> getViewport() { return 
            std::shared_ptr<sfg::Canvas>((sfg::Canvas*) WidgetMap[GUIWidgetNames::VIEWPORT].get()); }

        // Simplifies the process of creating a GUI window and its associations with other classes
        std::shared_ptr<sfg::Widget> generateWidget(std::string guiWindowName, std::shared_ptr<sfg::Widget> window);

        // Verifies whether the main game window and editor window are both open
        bool isMainWindowOpen() {
            return mainWindow->isOpen();
        }

        // Processes the event stack and permits both RenderWindows and SFGUI widgets to react to events
        void pollEvents();

        //---------------------Member Variables----------------------

        std::shared_ptr<sf::RenderWindow> mainWindow;

        // A mapping between a two names and a GUI window to be displayed within a given RenderWindow.
        // The first name is the name of the RenderWindow to which the SFGUI window is assigned.
        // That same name acts as a key to the Desktop that updates the stored SFGUI window.
        // The second name is the name of the actual sfg::Widget GUI window.
        std::map<std::string, std::shared_ptr<sfg::Widget>> WidgetMap;

        // A mapping between a name and an event for polling events simultaneously between multiple windows
        std::shared_ptr<sf::Event> event;

        // A desktop that will manage GUI windows for the same-named RenderWindow
        std::shared_ptr<sfg::Desktop> desktop;

        // A mapping between a name and the SFGUI controller object that manages the dynamic display of sf::RenderWindows
        std::shared_ptr<sfg::SFGUI> sfgui;

        // A pointer to the InputSystem to process user-defined input actions
        std::shared_ptr<InputSystem> input;

        // The preset name for the main window of the engine
        const static std::string MAIN_WINDOW_NAME;
    };

}

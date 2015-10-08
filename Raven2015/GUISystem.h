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
#include "SFGUI/Window.hpp"
#include "SFGUI/Desktop.hpp"
#include "InputSystem.h"

namespace Raven {

    // Create a static wrapper for the various preset window types
    struct GameWindowTypes {

        // The name of the viewport window
        const static std::string VIEWPORT_WINDOW;
        // The name of the editor window
        const static std::string EDITOR_WINDOW;

    };

    // Create a static wrapper for the various preset GUI window types
    struct GUIWindowTypes {

        // The name of the editor window
        const static std::string SCENE_HIERARCHY;

    };

    // A system class that manages the creation of windows, the display of windows (excluding "draw" calls handled
    // by the RenderingSystem), and the updating of GUI widgets within "Desktop" elements linked to RenderWindows
    class GUISystem : public ex::System<GUISystem>,
        public ex::Receiver<GUISystem> {
    public:

        // Perform initializations
        explicit GUISystem(std::shared_ptr<InputSystem> inputSystem);
        
        void clear() {
            for (std::pair<std::string, std::shared_ptr<sf::RenderWindow>> name_window : sfWindowMap) {
                name_window.second->clear();
            }
        }

        void display() {
            for (std::pair<std::string, std::shared_ptr<sf::RenderWindow>> name_window : sfWindowMap) {
                sfgui->Display(*name_window.second);
                name_window.second->display();
            }
        }

        // Subscribe to events, if any
        void configure(ex::EventManager &event_manager) {

        }

        //process CollisionEvents
        //void receive(const CollisionEvent &event);

        // Add or remove textures & sprites dynamically, drawing sprites that are within view
        void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override;

        // Shortcut method for acquiring the window used for the game
        std::shared_ptr<sf::RenderWindow> getViewport() { return sfWindowMap[GameWindowTypes::VIEWPORT_WINDOW]; }

        // Shortcut method for acquiring the window used for the editor
        std::shared_ptr<sf::RenderWindow> getEditor() { return sfWindowMap[GameWindowTypes::EDITOR_WINDOW]; }

        // Shortcut method for acquiring the default GUI window used inside the editor window
        std::shared_ptr<sfg::Window> getGUISceneHierarchy() { return sfgWindowMap[
            std::make_pair(GameWindowTypes::EDITOR_WINDOW, GUIWindowTypes::SCENE_HIERARCHY)]; }

        // Simplifies the process of creating a render window and its associations with other classes
        std::shared_ptr<sf::RenderWindow> generateRenderWindow(std::string name, std::shared_ptr<sf::RenderWindow> window);

        // Simplifies the process of creating a GUI window and its associations with other classes
        std::shared_ptr<sfg::Window> generateGUIWindow(std::string renderWindowName, std::string guiWindowName, std::shared_ptr<sfg::Window> window);

        // Alternative function that adds a different parameter list for the same functionality as using 2 std::string's
        void generateGUIWindow(std::pair<std::string, std::string> renderName_guiName, std::shared_ptr<sfg::Window> window) {
            generateGUIWindow(renderName_guiName.first, renderName_guiName.second, window);
        }

        // Verifies whether the main game window and editor window are both open
        bool mainWindowsAreOpen() {
            return sfWindowMap[GameWindowTypes::VIEWPORT_WINDOW]->isOpen() &&
                sfWindowMap[GameWindowTypes::EDITOR_WINDOW]->isOpen();
        }

        // Processes the event stacks of each RenderWindow and updates the corresponding GUI desktop.
        // Also makes calls to the InputSystem to have it propagate Pawn input events.
        bool pollEvents();

        // A mapping between a name and a window used either by the Rendering or GUI systems
        std::map<std::string, std::shared_ptr<sf::RenderWindow>> sfWindowMap;

        // A mapping between a two names and a GUI window to be displayed within a given RenderWindow.
        // The first name is the name of the RenderWindow to which the SFGUI window is assigned.
        // That same name acts as a key to the Desktop that updates the stored SFGUI window.
        // The second name is the name of the actual sfg::Window GUI window.
        std::map<std::pair<std::string, std::string>, std::shared_ptr<sfg::Window>> sfgWindowMap;

        // A mapping between a name and an event for polling events simultaneously between multiple windows
        std::map<std::string, std::shared_ptr<sf::Event>> eventMap;

        // A mapping between a name and desktop that will manage GUI windows for the same-named RenderWindow
        std::map<std::string, std::shared_ptr<sfg::Desktop>> desktopMap;

        // A mapping between a name and the SFGUI controller object that manages the dynamic display of sf::RenderWindows
        std::shared_ptr<sfg::SFGUI> sfgui;

        // A pointer to the InputSystem to process user-defined input actions
        std::shared_ptr<InputSystem> input;
    };

}

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

    class GUISystem : public ex::System<GUISystem>,
        public ex::Receiver<GUISystem> {
    public:


        // Perform initializations
        explicit GUISystem(std::shared_ptr<InputSystem> inputSystem);
        
        void clear() {
            for (std::pair<int, std::shared_ptr<sf::RenderWindow>> id_window : sfWindowMap) {
                id_window.second->clear();
            }
        }

        void display() {
            for (std::pair<int, std::shared_ptr<sf::RenderWindow>> id_window : sfWindowMap) {
                sfgui->Display(*id_window.second);
                id_window.second->display();
            }
        }

        // Subscribe to events
        void configure(ex::EventManager &event_manager) {

        }

        //void receive(const CollisionEvent &event);

        // Add or remove textures & sprites dynamically, drawing sprites that are within view
        void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override;

        // Shortcut method for acquiring the window used for the game
        std::shared_ptr<sf::RenderWindow> getGameWindow() { return sfWindowMap[ESFWindow::GAME_WINDOW]; }

        // Shortcut method for acquiring the window used for the editor
        std::shared_ptr<sf::RenderWindow> getEditorWindow() { return sfWindowMap[ESFWindow::EDITOR_WINDOW]; }

        // Shortcut method for acquiring the default GUI window used inside the editor window
        std::shared_ptr<sfg::Window> getDefaultGUIWindow() { return sfgWindowMap[ESFGWindow::GUI_WINDOW]; }

        // Simplifies the process of creating a render window and its associations with other classes
        void generateRenderWindow(int id, std::shared_ptr<sf::RenderWindow> window);

        // Simplifies the process of creating a GUI window and its associations with other classes
        void generateGUIWindow(int id, std::shared_ptr<sfg::Window> window);

        bool mainWindowsAreOpen() {
            return sfWindowMap[ESFWindow::GAME_WINDOW]->isOpen() &&
                sfWindowMap[ESFWindow::EDITOR_WINDOW]->isOpen();
        }

        bool pollEvents();

        /*
         * Need to write a map that provides access to several different RenderWindows, or perhaps a
         * vector with some enum to provide access. The main will call a function from the GUISystem
         * that will first check whether ALL of the windows are open. If so, it will create one
         * sf::Event object for each window and begin polling events from each of them, GUI windows
         * first. Then it will handle the inputs accordingly (desktop->HandleEvent for GUI windows and
         * our InputSystem's method that will later be developed for the game window - again these are
         * all called from a single GUISystem method probably), and then the will all be drawn as they
         * are now.
         */

        // Create enum IDs for the various types of preset RenderWindows that can be accessed
        enum ESFWindow { GAME_WINDOW, EDITOR_WINDOW };

        // Create enum IDs for the various types of preset SFGUI Windows that can be accessed
        enum ESFGWindow { GUI_WINDOW };

        // A mapping between an ID and a window used either by the Rendering or GUI systems
        std::map<int, std::shared_ptr<sf::RenderWindow>> sfWindowMap;

        // A mapping between an ID and a GUI window to be displayed within a given RenderWindow
        std::map<int, std::shared_ptr<sfg::Window>> sfgWindowMap;

        // A mapping between an ID and an event for polling events simultaneously between multiple windows
        std::map<int, std::shared_ptr<sf::Event>> eventMap;

        // A pointer to the desktop that manages the GUI windows
        std::shared_ptr<sfg::Desktop> desktop;

        // A pointer to the SFGUI controller object that manages the dynamic display of sf::RenderWindows
        std::shared_ptr<sfg::SFGUI> sfgui;

        // A pointer to the InputSystem to process user-defined input actions
        std::shared_ptr<InputSystem> input;
    };

}

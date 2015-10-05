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

namespace Raven {

    class GUISystem : public ex::System<GUISystem>,
        public ex::Receiver<GUISystem> {
    public:


        // Perform initializations
        explicit GUISystem(std::shared_ptr<sf::RenderWindow> window = nullptr) : 
                gameWindow(std::shared_ptr<sf::RenderWindow>(
                    new sf::RenderWindow(sf::VideoMode(600,400),"Viewport"))),
                editorWindow(std::shared_ptr<sf::RenderWindow>(
                    new sf::RenderWindow(sf::VideoMode(600,400),"Editor"))),
                guiWindow(sfg::Window::Create()),
                desktop(new sfg::Desktop()), 
                sfgui(new sfg::SFGUI()) {

            guiWindow->SetPosition(sf::Vector2f(50, 50));
            editorWindow->setPosition(sf::Vector2i(20, 20));
            desktop->Add(guiWindow);

        }
        
        void clear() {
            //editorWindow->clear();
            gameWindow->clear();
        }

        void display() {
            sfgui->Display(*gameWindow);
            //editorWindow->display();
            gameWindow->display();
        }

        // Subscribe to events
        void configure(ex::EventManager &event_manager) {

        }

        //void receive(const CollisionEvent &event);

        // Add or remove textures & sprites dynamically, drawing sprites that are within view
        void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override;

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

        // A pointer to the window used for the game
        std::shared_ptr<sf::RenderWindow> gameWindow;

        // A pointer to the window used for the Editor
        std::shared_ptr<sf::RenderWindow> editorWindow;

        // A pointer to the gui "Window" embedded in the editorWindow to display gui contents
        std::shared_ptr<sfg::Window> guiWindow;

        // A pointer to the desktop that manages the GUI windows
        std::shared_ptr<sfg::Desktop> desktop;

        // A pointer to the SFGUI controller object that manages the dynamic display of sf::RenderWindows
        std::shared_ptr<sfg::SFGUI> sfgui;
    };

}

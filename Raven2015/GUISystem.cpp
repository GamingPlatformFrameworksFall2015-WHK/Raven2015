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

    // Perform initializations
    GUISystem::GUISystem(std::shared_ptr<InputSystem> inputSystem) : 
            desktop(new sfg::Desktop()), 
            sfgui(new sfg::SFGUI()),
            input(inputSystem) {

        // Create all preset types of windows
        generateRenderWindow(ESFWindow::GAME_WINDOW,
            std::shared_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(600, 400), "Viewport")));
        generateRenderWindow(ESFWindow::EDITOR_WINDOW,
            std::shared_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(600, 400), "Editor")));

        // Create all preset GUI windows within the editor window
        generateGUIWindow(ESFGWindow::GUI_WINDOW,
            std::shared_ptr<sfg::Window>(sfg::Window::Create()));

        // Position the Viewport and the Editor where you want them.
        sfWindowMap[ESFWindow::EDITOR_WINDOW]->setPosition(sf::Vector2i(20, 20));
        sfWindowMap[ESFWindow::GAME_WINDOW]->setPosition(sf::Vector2i(200, 200));
    }

    void GUISystem::generateRenderWindow(int id, std::shared_ptr<sf::RenderWindow> window) {
        // Keep a record of this exact window
        sfWindowMap.insert(std::make_pair(id, window));
        // Create an associated event to poll for events triggered by the window
        eventMap.insert(std::make_pair(id, std::shared_ptr<sf::Event>(new sf::Event())));
    }

    void GUISystem::generateGUIWindow(int id, std::shared_ptr<sfg::Window> window) {
        // Keep a record of this exact window
        sfgWindowMap.insert(std::make_pair(id, window));
        // Add it to the desktop so that it will be updated
        desktop->Add(sfgWindowMap[id]);
    }

    bool GUISystem::pollEvents() {
        for (std::pair<int, std::shared_ptr<sf::RenderWindow>> id_window : sfWindowMap) {
            sf::Event e = *eventMap[id_window.first];
            if (!id_window.second->pollEvent(e)) {
                return false;
            }
            desktop->HandleEvent(e);

            //-----------------------------------------------
            // Call a function that encapsulates the event.type switch statement and simply returns the corresponding action.
            // Use the value returned by this function to then call another function that will emit an event that triggers the
            // corresponding action on all entities listening for that action via a Pawn component.
            //-----------------------------------------------


            /* ---- LEGACY CODE ----

            // Handle game input using the InputSystem
            input->setEventType(event);
            switch (event.type) {
            case sf::Event::Closed:
                window->close();
                break;
            case sf::Event::KeyPressed: {
                //call a function that derives the action from 
                int speed = 10;
                if (input->getAction(event.key.code) == "move_right") {
                    //trigger
                }
                else if (input->getAction(event.key.code) == "move_down") {
                    //entity1.component<Transform>().get()->transform.y += speed;
                }
                else if (input->getAction(event.key.code) == "move_left") {
                    //entity1.component<Transform>().get()->transform.x -= speed;
                }
                else if (input->getAction(event.key.code) == "move_up") {
                    //entity1.component<Transform>().get()->transform.y -= speed;
                }
                else if (input->getAction(event.key.code) == "exit") {
                    window->close();
                }
                break;
            }
            case sf::Event::MouseButtonPressed:
                break;
            case sf::Event::JoystickButtonPressed:
                break;
            case sf::Event::JoystickMoved:
                break;
            }

            */

        }
        return true;
    }

    void GUISystem::update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) {
        // Update all GUI widgets associated with the set of GUI windows
        desktop->Update((float)dt);
    }

}

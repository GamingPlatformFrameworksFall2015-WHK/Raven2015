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

namespace Raven {

    class GUISystem : public ex::System<GUISystem>,
        public ex::Receiver<GUISystem> {
    public:


        // Perform initializations
        explicit GUISystem(std::shared_ptr<sf::RenderWindow> window = nullptr) : 
                gameWindow(window ? window : std::shared_ptr<sf::RenderWindow>(
                    new sf::RenderWindow(sf::VideoMode(600,400),"Viewport"))),
                editorWindow(sfg::Window::Create()) {

            editorWindow->SetPosition(sf::Vector2f(50, 50));
        }

        // Subscribe to events
        void configure(ex::EventManager &event_manager) {

        }

        //void receive(const CollisionEvent &event);

        // Add or remove textures & sprites dynamically, drawing sprites that are within view
        void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override;

        // A pointer to the window used for the game
        std::shared_ptr<sf::RenderWindow> gameWindow;

        // A pointer to the window used for the Editor
        std::shared_ptr<sfg::Window> editorWindow;

        // A Renderable min-heap for determining draw order of text, shapes, and sprites
        std::priority_queue<Renderable> renderableHeap;
    };

}

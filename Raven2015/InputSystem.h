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
#include "entityx\System.h"
#include "EventLibrary.h"
#include <string>

//Assuming there exists an event with this name in EventLibrary.h
//struct KeyboardEvent : public entityx::Event<InputEvent> { KeyboardEvent() {} };
namespace Raven {
    class InputSystem : public entityx::System<InputSystem>, public entityx::Receiver<InputSystem> {
    public:
        const std::string CONFIG = "config.cfg";

        //Perform initializations
        explicit InputSystem() {
            movementThresX = 0.0;
            movementThresY = 0.0;
            key = "";
            read_file(CONFIG);
        }

        //Subscribe to events
        void configure(entityx::EventManager &event_manager) {
            event_manager.subscribe<KeyboardEvent>(*this);
        }

        //Update data and perform logic every "tick"
        void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override;
                        
        // Receives KeyboardEvents
        void receive(const KeyboardEvent &event);

        /*
        * Returns the string of the action depending on the keyboard key
        */
        std::string getAction(sf::Keyboard::Key key) {
            return key_map[key];
        }

        std::string getAction(sf::Mouse::Button button) {
            return mouse_map[button];
        }

        int handleEvent(sf::Event);

    private:
        std::map<sf::Keyboard::Key, std::string> key_map{
            { sf::Keyboard::A,		"move_left" },
            { sf::Keyboard::D,		"move_right" },
            { sf::Keyboard::W,		"move_up" },
            { sf::Keyboard::S,		"move_down" },
            { sf::Keyboard::Left,	"move_left" },
            { sf::Keyboard::Right,	"move_right" },
            { sf::Keyboard::Up,		"move_up" },
            { sf::Keyboard::Down,	"move_down" },
            { sf::Keyboard::Escape, "exit" }
        };

        std::map<sf::Mouse::Button, std::string> mouse_map{
            { sf::Mouse::Button::Left,	"left_click"},
        };

        float movementThresX;
        float movementThresY;
        std::string key;

        void insert_input(sf::Keyboard::Key key, std::string action);

        void read_file(std::string);
    };
}
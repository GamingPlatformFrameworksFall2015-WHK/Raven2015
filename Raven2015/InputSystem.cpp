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


#include "InputSystem.h"
#include <fstream>

using namespace Raven;


void InputSystem::update(ex::EntityManager &es, ex::EventManager &events,
	ex::TimeDelta dt) {
}

void InputSystem::insert_input(sf::Keyboard::Key key, std::string action) {
	try {
		if (key_map.at(key) != action) {
			key_map[key] = action;
		}
	}
	catch (std::exception e) {
		//If k does not match the key of any element in the container, the function throws an out_of_range exception.
		key_map.insert(std::pair<sf::Keyboard::Key, std::string>(key, action));
	}
}

void InputSystem::read_file(std::string filename) {
	std::ifstream file;
	file.open(filename);
	if (!file.is_open()) {
		//FILENAME IS BAD
		return;
	}
	std::string action, key;
	while (file >> action) {
		file >> key;
		if ("A" == key) { insert_input(sf::Keyboard::A, action); break; }
		else if ("B" == key) { insert_input(sf::Keyboard::B, action); break; }
		else if ("C" == key) { insert_input(sf::Keyboard::C, action); break; }
		else if ("D" == key) { insert_input(sf::Keyboard::D, action); break; }
		else if ("E" == key) { insert_input(sf::Keyboard::E, action); break; }
		else if ("F" == key) { insert_input(sf::Keyboard::F, action); break; }
		else if ("G" == key) { insert_input(sf::Keyboard::G, action); break; }
		else if ("H" == key) { insert_input(sf::Keyboard::H, action); break; }
		else if ("I" == key) { insert_input(sf::Keyboard::I, action); break; }
		else if ("J" == key) { insert_input(sf::Keyboard::J, action); break; }
		else if ("K" == key) { insert_input(sf::Keyboard::K, action); break; }
		else if ("L" == key) { insert_input(sf::Keyboard::L, action); break; }
		else if ("M" == key) { insert_input(sf::Keyboard::M, action); break; }
		else if ("N" == key) { insert_input(sf::Keyboard::N, action); break; }
		else if ("O" == key) { insert_input(sf::Keyboard::O, action); break; }
		else if ("P" == key) { insert_input(sf::Keyboard::P, action); break; }
		else if ("Q" == key) { insert_input(sf::Keyboard::Q, action); break; }
		else if ("R" == key) { insert_input(sf::Keyboard::R, action); break; }
		else if ("S" == key) { insert_input(sf::Keyboard::S, action); break; }
		else if ("T" == key) { insert_input(sf::Keyboard::T, action); break; }
		else if ("U" == key) { insert_input(sf::Keyboard::U, action); break; }
		else if ("V" == key) { insert_input(sf::Keyboard::V, action); break; }
		else if ("W" == key) { insert_input(sf::Keyboard::W, action); break; }
		else if ("X" == key) { insert_input(sf::Keyboard::X, action); break; }
		else if ("Y" == key) { insert_input(sf::Keyboard::Y, action); break; }
		else if ("Z" == key) { insert_input(sf::Keyboard::Z, action); break; }
		else if ("Num0" == key) { insert_input(sf::Keyboard::Num0, action); break; }
		else if ("Num1" == key) { insert_input(sf::Keyboard::Num1, action); break; }
		else if ("Num2" == key) { insert_input(sf::Keyboard::Num2, action); break; }
		else if ("Num3" == key) { insert_input(sf::Keyboard::Num3, action); break; }
		else if ("Num4" == key) { insert_input(sf::Keyboard::Num4, action); break; }
		else if ("Num5" == key) { insert_input(sf::Keyboard::Num5, action); break; }
		else if ("Num6" == key) { insert_input(sf::Keyboard::Num6, action); break; }
		else if ("Num7" == key) { insert_input(sf::Keyboard::Num7, action); break; }
		else if ("Num8" == key) { insert_input(sf::Keyboard::Num8, action); break; }
		else if ("Num9" == key) { insert_input(sf::Keyboard::Num9, action); break; }
		else if ("Escape" == key) { insert_input(sf::Keyboard::Escape, action); break; }
		else if ("LControl" == key) { insert_input(sf::Keyboard::LControl, action); break; }
		else if ("LShift" == key) { insert_input(sf::Keyboard::LShift, action); break; }
		else if ("LAlt" == key) { insert_input(sf::Keyboard::LAlt, action); break; }
		else if ("Left" == key) { insert_input(sf::Keyboard::Left, action); break; }
		else if ("Right" == key) { insert_input(sf::Keyboard::Right, action); break; }
		else if ("Up" == key) { insert_input(sf::Keyboard::Up, action); break; }
		else if ("Down" == key) { insert_input(sf::Keyboard::Down, action); break; }
	}

}
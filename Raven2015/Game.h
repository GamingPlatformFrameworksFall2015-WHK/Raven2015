#pragma once
#include "entityx/entityx.h"
#include "Common.h"
#include "SFML/Graphics.hpp"

namespace Raven {

    class Game : public ex::EntityX {
    public:
        explicit Game(sf::RenderTarget &target);
        explicit Game();

        void update(ex::TimeDelta dt);
    };

}
#pragma once

#include "../Common.h"
#include "entityx\System.h"
#include "../EventLibrary.h"
#include <map>
#include <queue>
#include "GUISystem.h"
#include "DataAssetLibrary.h"

namespace Raven {

    class RenderingSystem : public entityx::System<RenderingSystem>, 
        public ex::Receiver<RenderingSystem> {
    public:
        // Perform initializations
        explicit RenderingSystem(std::shared_ptr<GUISystem> system, Assets* assets)
            : renderWindow(system->mainWindow), canvas(system->canvas), assets(assets) {}

        // Subscribe to events
        void configure(entityx::EventManager &event_manager) {
            event_manager.subscribe<GUIRegisterTextureEvent>(*this);
        }

        // Registers texture assets for usage
        void receive(const GUIRegisterTextureEvent& e);

        // Add or remove textures & sprites dynamically, drawing sprites that are within view
        void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override;
            
        // A pointer to the window that displays the widgets
        std::shared_ptr<sf::RenderWindow> renderWindow;

        // A pointer to the widget used for rendering
        std::shared_ptr<Canvas> canvas;

        // A Renderable min-heap for determining draw order of text, shapes, and sprites
        std::priority_queue<Renderable> renderableHeap;

        // A mapping of texture file paths (derived from Assets) to sf::Texture objects
        std::map<std::string, sf::Texture> textureMap;

        // A pointer to the assets contained within the XMLSystem
        Assets* assets;
    };

}

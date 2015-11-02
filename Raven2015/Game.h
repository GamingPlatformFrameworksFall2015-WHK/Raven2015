#pragma once
#include "entityx/entityx.h"
#include "Common.h"
#include "SFML/Graphics.hpp"

namespace Raven {

    class Game : public ex::EntityX {
    public:
        explicit Game(sf::RenderTarget &target);
        explicit Game();

        void initialize();
        void loadLevel(std::string levelFilePath, sf::Vector2f levelOffset, bool clearEntitiesBeforehand);
        void saveLevel();
        void addLevel(std::string levelFilePath);
        void removeLevel(std::string levelFilePath);

        // The mapping of entities names to entity shared pointers
        std::set<ex::Entity> getEntitySet();
        // The set of level file paths
        std::set<std::string> getLevelList();
        // The top-level rendering window for the editor
        std::shared_ptr<sf::RenderWindow> getWindow();
        void pollEvents();
        bool isMainWindowOpen();
        void clearWindow();
        void displayWindow();

        void updateGameMode(ex::TimeDelta dt);
        void updateEditMode(ex::TimeDelta dt);

        // Provides custom method for assigning default components to an entity
        ex::Entity makeEntity();
        // Creates a default entity and assigns it the given name
        ex::Entity makeEntity(std::string name);
        // Creates an instance of the prefab and assigns it the given name
        ex::Entity instantiatePrefab(std::string name, std::string prefabName);
        // Serializes game content into the Raven XML document
        void save();
        // Deserializes game content from the Raven XML document
        void load();

        // The name of the level to which newly made entities are currently assigned
        std::string currentLevelPath;

        // The name of the level that is loaded upon engine start-up
        std::string defaultLevelPath;

        // Used to create unique entity names and track the number of entities that have been created thus far
        unsigned int entityCounter;

        // Whether we are currently in edit mode
        bool editMode;


    private:
        void clearEntities();
    };

}
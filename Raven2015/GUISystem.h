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
#include "SFGUI/Widgets.hpp"
#include "InputSystem.h"
#include "WidgetLibrary.h"

using namespace sfg;

namespace Raven {

    // A system class that manages the creation of windows, the display of windows (excluding "draw" calls handled
    // by the RenderingSystem), and the updating of GUI widgets within "Desktop" elements linked to RenderWindows
    class GUISystem : public ex::System<GUISystem>,
        public ex::Receiver<GUISystem> {
    public:

        // Perform initializations
        explicit GUISystem(std::shared_ptr<InputSystem> inputSystem);
        
        void clear() {
            mainWindow->clear();
        }

        void display() {
            mainWindow->setActive(true);
            sfgui->Display(*mainWindow);
            mainWindow->display();
        }

        // Subscribe to events, if any
        void configure(ex::EventManager &event_manager) {

        }

        //process CollisionEvents
        //void receive(const CollisionEvent &event);

        // Add or remove textures & sprites dynamically, drawing sprites that are within view
        void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override;

        // Format the Master Table widget
        //std::shared_ptr<Table> formatMasterTable(std::shared_ptr<Table>);
        // Format the Scene Hierarchy widget
        SCENE_HIERARCHY_WTYPE_SPTR formatSceneHierarchy(SCENE_HIERARCHY_WTYPE_SPTR);
        // Format the Component List widget
        COMPONENT_LIST_WTYPE_SPTR formatComponentList(COMPONENT_LIST_WTYPE_SPTR);
        // Format the Texture List widget
        TEXTURE_LIST_WTYPE_SPTR formatTextureList(TEXTURE_LIST_WTYPE_SPTR);
        // Format the Music List widget
        MUSIC_LIST_WTYPE_SPTR formatMusicList(MUSIC_LIST_WTYPE_SPTR);
        // Format the Sound List widget
        SOUND_LIST_WTYPE_SPTR formatSoundList(SOUND_LIST_WTYPE_SPTR);
        // Format the Font List widget
        FONT_LIST_WTYPE_SPTR formatFontList(FONT_LIST_WTYPE_SPTR);
        // Format the Content widget
        CONTENT_WTYPE_SPTR formatContent(CONTENT_WTYPE_SPTR);
        // Format the Entity Designer widget
        ENTITY_DESIGNER_WTYPE_SPTR formatEntityDesigner(ENTITY_DESIGNER_WTYPE_SPTR);
        // Format the Prefab List widget
        PREFAB_LIST_WTYPE_SPTR formatPrefabList(PREFAB_LIST_WTYPE_SPTR);
        // Format the Canvas widget
        CANVAS_WTYPE_SPTR formatCanvas(CANVAS_WTYPE_SPTR);
        // Format the Toolbar widget
        TOOLBAR_WTYPE_SPTR formatToolbar(TOOLBAR_WTYPE_SPTR);

        // Verifies whether the main game window and editor window are both open
        bool isMainWindowOpen() {
            return mainWindow->isOpen();
        }

        // Processes the event stack and permits both RenderWindows and SFGUI widgets to react to events
        void pollEvents();

        //--------------------Callback Methods-----------------------

        // Switches the currentBrush based on the button clicked, thereby changing the canvasClickHandler operation
        void brushToolbarButtonHandler(Button::Ptr clickedButton);

        // Displays the selected Entity's component list in the EntityDesigner panel
        void sceneHierachySelectButtonHandler(Button::Ptr clickedButton);

        // Destroys the Entity instance and removes its record from the LevelMap
        void sceneHierachyDeleteButtonHandler(Button::Ptr clickedButton);

        // Reorders the children of the sceneHierarchyBox so that the clicked entity moves up one
        void sceneHierachyMoveUpButtonHandler(Button::Ptr clickedButton);

        // Reorders the children of the sceneHierarchyBox so that the clicked entity moves down one
        void sceneHierachyMoveDownButtonHandler(Button::Ptr clickedButton);

        // When modified, updates the name of the given entity
        void sceneHierachyEntryHandler(Entry::Ptr updatedEntry);

        // Displays the selected Entity's component list in the EntityDesigner panel
        void prefabListSelectButtonHandler(Button::Ptr clickedButton);

        // Destroys the Entity instance and removes its record from the LevelMap
        void prefabListDeleteButtonHandler(Button::Ptr clickedButton);

        // Reorders the children of the prefabListBox so that the clicked prefab moves up one
        void prefabListMoveUpButtonHandler(Button::Ptr clickedButton);

        // Reorders the children of the prefabListBox so that the clicked prefab moves down one
        void prefabListMoveDownButtonHandler(Button::Ptr clickedButton);

        // When modified, updates the name of the given prefab
        void prefabListEntryHandler(Entry::Ptr updatedEntry);

        // Performs an operation on the Canvas based on the current brush mode
        void canvasClickHandler();


        //---------------------Member Variables----------------------

        // A pointer to the RenderWindow representing the actual window created from SFML
        std::shared_ptr<sf::RenderWindow> mainWindow;

        // A mapping between a name and an event for polling events simultaneously between multiple windows
        std::shared_ptr<sf::Event> event;

        // A desktop that will manage GUI windows for the same-named RenderWindow
        std::shared_ptr<Desktop> desktop;

        // A mapping between a name and the SFGUI controller object that manages the dynamic display of sf::RenderWindows
        std::shared_ptr<SFGUI> sfgui;

        // A pointer to the InputSystem to process user-defined input actions
        std::shared_ptr<InputSystem> input;

        // A pointer to the top-level GUI container for the entire editor
        Window::Ptr mainGUIWindow;

        // A pointer to the table organizing the content in the mainGUIWindow
        Table::Ptr table;

        //-------------Top level widget panels and their sub-widget-containers------------------

        // The window upon which the game is drawn
        Canvas::Ptr canvas;
        // The panel displaying entities currently in the level
        ScrolledWindow::Ptr sceneHierarchy;
        // The procedurally generated box managed by the Scene Hierarchy. Contains a list of the entities in the level
        Box::Ptr sceneHierarchyBox; 
        // The tabbed list of assets available to the user
        Notebook::Ptr content;
        // The list of commands available to the user when interacting with the Canvas
        Box::Ptr toolbar;
        // The window allowing for the user to modify which components are on an entity and modify their member values
        ScrolledWindow::Ptr entityDesigner;
        // The panel displaying entities are currently stored as prefabs
        ScrolledWindow::Ptr prefabList;
        // The procedurally generated box managed by the PrefabList. Contains a list of the prefabs that can be instantiated
        Box::Ptr prefabListBox; 

        //Notebook'd SubPanels in Content
        ScrolledWindow::Ptr componentList;
        Box::Ptr componentListBox;
        ScrolledWindow::Ptr textureList;
        Box::Ptr textureListBox;
        ScrolledWindow::Ptr musicList;
        Box::Ptr musicListBox;
        ScrolledWindow::Ptr soundList;
        Box::Ptr soundListBox;
        ScrolledWindow::Ptr fontList;
        Box::Ptr fontListBox;

        // The label for the specific brush currently in use
        Label::Ptr currentBrush = Label::Create("Create");

        // The preset name for the main window of the engine
        const static std::string MAIN_WINDOW_NAME;
    };

}

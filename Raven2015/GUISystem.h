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

        // Shortcut method for acquiring the SceneHierarchy, a list of entities in the scene
        std::shared_ptr<sfg::ScrolledWindow> getGUISceneHierarchy() { return 
            std::shared_ptr<sfg::ScrolledWindow>((sfg::ScrolledWindow*) WidgetMap[cmn::GUIWidgetNames::SCENE_HIERARCHY].get()); }

        // Shortcut method for acquiring the ComponentList, a list of the different types of components that exist
        std::shared_ptr<sfg::ScrolledWindow> getGUIComponentList() { return 
            std::shared_ptr<sfg::ScrolledWindow>((sfg::ScrolledWindow*) WidgetMap[cmn::GUIWidgetNames::COMPONENT_LIST].get()); }

        // Shortcut method for acquiring the TextureList, a list of the different types of texture assets that exist
        std::shared_ptr<sfg::ScrolledWindow> getGUITextureList() { return 
            std::shared_ptr<sfg::ScrolledWindow>((sfg::ScrolledWindow*) WidgetMap[cmn::GUIWidgetNames::TEXTURE_LIST].get()); }

        // Shortcut method for acquiring the MusicList, a list of the different types of music assets that exist
        std::shared_ptr<sfg::ScrolledWindow> getGUIMusicList() { return 
            std::shared_ptr<sfg::ScrolledWindow>((sfg::ScrolledWindow*) WidgetMap[cmn::GUIWidgetNames::MUSIC_LIST].get()); }

        // Shortcut method for acquiring the SoundList, a list of the different types of sound assets that exist
        std::shared_ptr<sfg::ScrolledWindow> getGUISoundList() { return 
            std::shared_ptr<sfg::ScrolledWindow>((sfg::ScrolledWindow*) WidgetMap[cmn::GUIWidgetNames::SOUND_LIST].get()); }

        // Shortcut method for acquiring the FontList, a list of the different types of font assets that exist
        std::shared_ptr<sfg::ScrolledWindow> getGUIFontList() { return 
            std::shared_ptr<sfg::ScrolledWindow>((sfg::ScrolledWindow*) WidgetMap[cmn::GUIWidgetNames::FONT_LIST].get()); }

        // Shortcut method for acquiring the Content notebook, a tabbed list of the different types of assets (content) available
        std::shared_ptr<sfg::Notebook> getGUIContent() { return 
            std::shared_ptr<sfg::Notebook>((sfg::Notebook*) WidgetMap[cmn::GUIWidgetNames::CONTENT].get()); }

        // Shortcut method for acquiring the EntityDesigner, a window for tweaking structure and values in an Entity
        std::shared_ptr<sfg::ScrolledWindow> getGUIEntityDesigner() { return 
            std::shared_ptr<sfg::ScrolledWindow>((sfg::ScrolledWindow*) WidgetMap[cmn::GUIWidgetNames::ENTITY_DESIGNER].get()); }

        // Shortcut method for acquiring the PrefabList, a list of specifically named Entities with a recorded form
        std::shared_ptr<sfg::ScrolledWindow> getGUIPrefabList() { return 
            std::shared_ptr<sfg::ScrolledWindow>((sfg::ScrolledWindow*) WidgetMap[cmn::GUIWidgetNames::PREFAB_LIST].get()); }

        // Shortcut method for acquiring the Canvas, a renderable screen into the game space
        std::shared_ptr<sfg::Canvas> getCanvas() { return
                std::shared_ptr<sfg::Canvas>((sfg::Canvas*) WidgetMap[cmn::GUIWidgetNames::CANVAS].get()); }

        // Shortcut method for acquiring the Toolbar, a sequence of buttons that provide access to Brushes for Canvas interaction
        std::shared_ptr<sfg::Box> getToolbar() { return
                std::shared_ptr<sfg::Box>((sfg::Box*) WidgetMap[cmn::GUIWidgetNames::TOOLBAR].get()); }

        // Simplifies the process of creating a GUI window and its associations with other classes
        template <class T>
        std::shared_ptr<T> makeWidget(std::string widgetName);

        // Format the Master Table widget
        //std::shared_ptr<sfg::Table> formatMasterTable(std::shared_ptr<sfg::Table>);
        // Format the Scene Hierarchy widget
        std::shared_ptr<sfg::ScrolledWindow> formatSceneHierarchy(std::shared_ptr<sfg::ScrolledWindow>);
        // Format the Component List widget
        std::shared_ptr<sfg::ScrolledWindow> formatComponentList(std::shared_ptr<sfg::ScrolledWindow>);
        // Format the Texture List widget
        std::shared_ptr<sfg::ScrolledWindow> formatTextureList(std::shared_ptr<sfg::ScrolledWindow>);
        // Format the Music List widget
        std::shared_ptr<sfg::ScrolledWindow> formatMusicList(std::shared_ptr<sfg::ScrolledWindow>);
        // Format the Sound List widget
        std::shared_ptr<sfg::ScrolledWindow> formatSoundList(std::shared_ptr<sfg::ScrolledWindow>);
        // Format the Font List widget
        std::shared_ptr<sfg::ScrolledWindow> formatFontList(std::shared_ptr<sfg::ScrolledWindow>);
        // Format the Content widget
        std::shared_ptr<sfg::Notebook> formatContent(std::shared_ptr<sfg::Notebook>);
        // Format the Entity Designer widget
        std::shared_ptr<sfg::ScrolledWindow> formatEntityDesigner(std::shared_ptr<sfg::ScrolledWindow>);
        // Format the Prefab List widget
        std::shared_ptr<sfg::ScrolledWindow> formatPrefabList(std::shared_ptr<sfg::ScrolledWindow>);
        // Format the Canvas widget
        std::shared_ptr<sfg::Canvas> formatCanvas(std::shared_ptr<sfg::Canvas>);
        // Format the Toolbar widget
        std::shared_ptr<sfg::Box> formatToolbar(std::shared_ptr<sfg::Box>);

        // Verifies whether the main game window and editor window are both open
        bool isMainWindowOpen() {
            return mainWindow->isOpen();
        }

        // Processes the event stack and permits both RenderWindows and SFGUI widgets to react to events
        void pollEvents();

        //---------------------Member Variables----------------------

        // A pointer to the RenderWindow representing the actual window created from SFML
        std::shared_ptr<sf::RenderWindow> mainWindow;

        // A mapping between a name and an event for polling events simultaneously between multiple windows
        std::shared_ptr<sf::Event> event;

        // A desktop that will manage GUI windows for the same-named RenderWindow
        std::shared_ptr<sfg::Desktop> desktop;

        // A mapping between a name and the SFGUI controller object that manages the dynamic display of sf::RenderWindows
        std::shared_ptr<sfg::SFGUI> sfgui;

        // A pointer to the InputSystem to process user-defined input actions
        std::shared_ptr<InputSystem> input;

        // A pointer to the top-level GUI container for the entire editor
        std::shared_ptr<sfg::Window> mainGUIWindow;

        // A pointer to the table organizing the content in the mainGUIWindow
        std::shared_ptr<sfg::Table> table;

        // The preset name for the main window of the engine
        const static std::string MAIN_WINDOW_NAME;

		void brushToolbarButtonClick(sfg::Button::Ptr clickedButton);
		void GUISystem::sceneHierachyButton(sfg::Button::Ptr clickedButton);
		void canvasClickHandler();
		sfg::Label::Ptr currentBrush = sfg::Label::Create("Create");
    };

}

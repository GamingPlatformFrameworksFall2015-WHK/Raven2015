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

        // Shortcut method for acquiring the SceneHierarchy, a list of entities in the scene
        SCENE_HIERARCHY_WTYPE_SPTR getSceneHierarchy() {
            std::pair<std::string, std::string> p = std::make_pair(TO_STR(SCENE_HIERARCHY_WTYPE), SCENE_HIERARCHY_NAME);
            return WIDGET_CAST(WidgetMap[p], SCENE_HIERARCHY_WTYPE);
        }

        // Shortcut method for acquiring the ComponentList, a list of the different types of components that exist
        COMPONENT_LIST_WTYPE_SPTR getComponentList() {
            std::pair<std::string, std::string> p = std::make_pair(TO_STR(COMPONENT_LIST_WTYPE), COMPONENT_LIST_NAME);
            return WIDGET_CAST(WidgetMap[p], COMPONENT_LIST_WTYPE);
        }

        // Shortcut method for acquiring the TextureList, a list of the different types of texture assets that exist
        TEXTURE_LIST_WTYPE_SPTR getTextureList() {
            std::pair<std::string, std::string> p = std::make_pair(TO_STR(TEXTURE_LIST_WTYPE), TEXTURE_LIST_NAME);
            return WIDGET_CAST(WidgetMap[p], TEXTURE_LIST_WTYPE);
        }

        // Shortcut method for acquiring the MusicList, a list of the different types of music assets that exist
        MUSIC_LIST_WTYPE_SPTR getMusicList() { 
            std::pair<std::string, std::string> p = std::make_pair(TO_STR(MUSIC_LIST_WTYPE), MUSIC_LIST_NAME);
            return WIDGET_CAST(WidgetMap[p], MUSIC_LIST_WTYPE);
        }

        // Shortcut method for acquiring the SoundList, a list of the different types of sound assets that exist
        SOUND_LIST_WTYPE_SPTR getSoundList() {
            std::pair<std::string, std::string> p = std::make_pair(TO_STR(SOUND_LIST_WTYPE), SOUND_LIST_NAME);
            return WIDGET_CAST(WidgetMap[p], SOUND_LIST_WTYPE);
        }

        // Shortcut method for acquiring the FontList, a list of the different types of font assets that exist
        FONT_LIST_WTYPE_SPTR getFontList() {
            std::pair<std::string, std::string> p = std::make_pair(TO_STR(FONT_LIST_WTYPE), FONT_LIST_NAME);
            return WIDGET_CAST(WidgetMap[p], FONT_LIST_WTYPE);
        }

        // Shortcut method for acquiring the Content notebook, a tabbed list of the different types of assets (content) available
        CONTENT_WTYPE_SPTR getContent() {
            std::pair<std::string, std::string> p = std::make_pair(TO_STR(CONTENT_WTYPE), CONTENT_NAME);
            return WIDGET_CAST(WidgetMap[p], CONTENT_WTYPE);
        }

        // Shortcut method for acquiring the EntityDesigner, a window for tweaking structure and values in an Entity
        ENTITY_DESIGNER_WTYPE_SPTR getEntityDesigner() {
            std::pair<std::string, std::string> p = std::make_pair(TO_STR(ENTITY_DESIGNER_WTYPE), ENTITY_DESIGNER_NAME);
            return WIDGET_CAST(WidgetMap[p], ENTITY_DESIGNER_WTYPE);
        }

        // Shortcut method for acquiring the PrefabList, a list of specifically named Entities with a recorded form
        PREFAB_LIST_WTYPE_SPTR getPrefabList() {
            std::pair<std::string, std::string> p = std::make_pair(TO_STR(PREFAB_LIST_WTYPE), PREFAB_LIST_NAME);
            return WIDGET_CAST(WidgetMap[p], PREFAB_LIST_WTYPE);
        }

        // Shortcut method for acquiring the Canvas, a renderable screen into the game space
        CANVAS_WTYPE_SPTR getCanvas() {
            std::pair<std::string, std::string> p = std::make_pair(TO_STR(CANVAS_WTYPE), CANVAS_NAME);
            return WIDGET_CAST(WidgetMap[p], CANVAS_WTYPE);
        }

        // Shortcut method for acquiring the Toolbar, a sequence of buttons that provide access to Brushes for Canvas interaction
        TOOLBAR_WTYPE_SPTR getToolbar() {
            std::pair<std::string, std::string> p = std::make_pair(TO_STR(TOOLBAR_WTYPE), TOOLBAR_NAME);
            return WIDGET_CAST(WidgetMap[p], TOOLBAR_WTYPE);
        }

        // Makes a GUI widget, giving it a name, storing it in the WidgetMap, and adding it to the Desktop
        template <class T>
        std::shared_ptr<T> makeWidget(std::string widgetType, std::string widgetName);

        // Makes a GUI widget, but without giving it a name / storing it in the WidgetMap. For when you don't need direct access.
        template <class T>
        std::shared_ptr<T> makeWidget();

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
        std::shared_ptr<Window> mainGUIWindow;

        // A pointer to the table organizing the content in the mainGUIWindow
        std::shared_ptr<Table> table;

        // The preset name for the main window of the engine
        const static std::string MAIN_WINDOW_NAME;

        private:
        // A mapping between a two names and a widget shared pointer.
        // The first name is the name of the class type of the widget.
        // The second name is the name of the identifying label for the widget.
        // Example:
        // WidgetMap.insert(std::make_pair(std::make_pair("Button", "CreateBrushButton"), Button::Create());
        //
        // More often used with WidgetLibrary.h macros for particular widget types:
        // WidgetMap.insert(std::mak_pair(std::make_pair(TO_STR(MYBUTTON_WTYPE), MYBUTTON_NAME), Button::Create());
        // where the following exists...
        // #define MYBUTTON_WTYPE Button
        // #define MYBUTTON_NAME "CreateBrushButton"
        std::map<std::pair<std::string, std::string>, std::shared_ptr<Widget>> WidgetMap;

    };

}

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
    class GUISystem : public ex::System<GUISystem>, public ex::Receiver<GUISystem> {
    public:

        // Perform initializations
        explicit GUISystem(std::shared_ptr<InputSystem> inputSystem, ex::Entity* editingEntity);
        
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

        // Add or remove textures & sprites dynamically, drawing sprites that are within view
        void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override;

        // Format the Scene Hierarchy widget
        SCENE_HIERARCHY_WTYPE_SPTR formatSceneHierarchy(SCENE_HIERARCHY_WTYPE_SPTR);
        // Format the Content widget
        CONTENT_WTYPE_SPTR formatContent(CONTENT_WTYPE_SPTR);
        // Format the Texture List widget
        void formatTextureList(TEXTURE_LIST_WTYPE_SPTR);
        // Format the Music List widget
        void formatMusicList(MUSIC_LIST_WTYPE_SPTR);
        // Format the Sound List widget
        void formatSoundList(SOUND_LIST_WTYPE_SPTR);
        // Format the Font List widget
        void formatFontList(FONT_LIST_WTYPE_SPTR);
        // Format the Level List widget
        void formatLevelList(LEVEL_LIST_WTYPE_SPTR);
        // Format the Animation List widget
        void formatAnimationList(ANIMATION_LIST_WTYPE_SPTR);
        // Format the Text List widget
        void formatTextList(TEXT_LIST_WTYPE_SPTR);
        // Format the Text List widget
        void formatRectangleList(RECTANGLE_LIST_WTYPE_SPTR);
        // Format the Text List widget
        void formatCircleList(CIRCLE_LIST_WTYPE_SPTR);
        // Format the Text List widget
        void formatSpriteList(SPRITE_LIST_WTYPE_SPTR);
        // Helper function to consolidate operations for Asset List formatting
        template <typename T>
        Box::Ptr formatAssetListHelper(ScrolledWindow::Ptr list, Box::Ptr listBox, Button::Ptr addNewButton);
        // Format the Entity Designer widget
        ENTITY_DESIGNER_WTYPE_SPTR formatEntityDesigner(ENTITY_DESIGNER_WTYPE_SPTR);
        // Format the Component List widget
        COMPONENT_LIST_WTYPE_SPTR formatComponentList(COMPONENT_LIST_WTYPE_SPTR componentList);
        // Format the Component List widget, Base Case
        template <typename C>
        void formatComponentListHelper(C* c);
        // Format the Component List widget, Recursive Case
        template <typename C, typename... Components>
        void formatComponentListHelper(C* c, Components*... components);
        // Helper function to consolidate operations for Component List formatting
        template <typename C>
        void formatComponentListItem(C* c);
        // Format the Prefab List widget
        PREFAB_LIST_WTYPE_SPTR formatPrefabList(PREFAB_LIST_WTYPE_SPTR);
        // Format the Canvas widget
        CANVAS_WTYPE_SPTR formatCanvas(CANVAS_WTYPE_SPTR);
        // Format the Toolbar widget
        TOOLBAR_WTYPE_SPTR formatToolbar(TOOLBAR_WTYPE_SPTR);

        // Prefab List Manipulation
        void populatePrefabList(XMLDocument& prefabsDoc);
        void addItemToPrefabList(std::string itemName);
        void removeItemFromPrefabList(std::string itemName);
        // Scene Hierarchy Manipulation
        void populateSceneHierarchy(std::set<ex::Entity>& entitySet);
        void addItemToSceneHierarchy(std::string itemName);
        void removeItemFromSceneHierarchy(std::string itemName);
        // Asset List Manipulation
        void populateTextureList(std::set<std::string> assetList);
        void populateMusicList(std::set<std::string> assetList);
        void populateSoundList(std::set<std::string> assetList);
        void populateFontList(std::set<std::string> assetList);
        void populateLevelList(std::set<std::string> assetList);
        void populateAnimationList(std::map<std::string, std::shared_ptr<Animation>>& map);
        void populateTextList(std::map<std::string, std::shared_ptr<RenderableText>>& map);
        void populateRectangleList(std::map<std::string, std::shared_ptr<RenderableRectangle>>& map);
        void populateCircleList(std::map<std::string, std::shared_ptr<RenderableCircle>>& map);
        void populateSpriteList(std::map<std::string, std::shared_ptr<RenderableSprite>>& map);
        template <typename T>
        void populateAssetList(Box::Ptr assetListWidget, std::set<std::string>& assetList);
        template <typename T>
        void addItemToAssetList(Box::Ptr assetListWidget, std::string itemName, void(*formatter)(Box::Ptr));
        template <typename T>
        void removeItemFromAssetList(Box::Ptr assetListWidget, std::string itemName);
        template <typename T, typename Asett>
        void populateComplexAssetList(Box::Ptr assetMapWidget, std::map<std::string, Asett>& assetMap);
        template <typename T, typename Asett>
        void addItemToComplexAssetList(Box::Ptr assetMapWidget, std::string itemName);
        template <typename T, typename Asett>
        void removeItemFromComplexAssetList(Box::Ptr assetMapWidget, std::string itemName);

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

        // Destroys the Entity instance and removes its record from the entitySet
        void sceneHierachyDeleteButtonHandler(Button::Ptr clickedButton);

        // Reorders the children of the sceneHierarchyBox so that the clicked entity moves up one
        void sceneHierachyMoveUpButtonHandler(Button::Ptr clickedButton);

        // Reorders the children of the sceneHierarchyBox so that the clicked entity moves down one
        void sceneHierachyMoveDownButtonHandler(Button::Ptr clickedButton);

        // When modified, updates the name of the given entity
        void sceneHierachyEntryHandler(Entry::Ptr updatedEntry);

        // Displays the selected Entity's component list in the EntityDesigner panel
        void prefabListSelectButtonHandler(Button::Ptr clickedButton);

        // Destroys the Entity instance and removes its record from the entitySet
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

        // A mapping between widgets and entity objects
        std::map<Widget::Ptr, ex::Entity> entityWidgetMap;

        //-------------Top level widget panels and their sub-widget-containers------------------

        //----The window upon which the game is drawn----
        Canvas::Ptr canvas;

        //----The panel displaying entities currently in the level----
        ScrolledWindow::Ptr sceneHierarchy;
        // The procedurally generated box managed by the Scene Hierarchy. Contains a list of the entities in the level
        Box::Ptr sceneHierarchyBox;

        //----The tabbed list of assets available to the user----
        Notebook::Ptr content;

        //  //  // Texture List //  //  //
        ScrolledWindow::Ptr textureList;
        Box::Ptr textureListBox;
        Button::Ptr addNewTextureButton;

        //  //  // Music List //  //  //
        ScrolledWindow::Ptr musicList;
        Box::Ptr musicListBox;
        Button::Ptr addNewMusicButton;

        //  //  // Sound List //  //  //
        ScrolledWindow::Ptr soundList;
        Box::Ptr soundListBox;
        Button::Ptr addNewSoundButton;

        //  //  // Font List //  //  //
        ScrolledWindow::Ptr fontList;
        Box::Ptr fontListBox;
        Button::Ptr addNewFontButton;

        //  //  // Level List //  //  //
        ScrolledWindow::Ptr levelList;
        Box::Ptr levelListBox;
        Button::Ptr addNewLevelButton;

        //  //  // Animation List //  //  //
        ScrolledWindow::Ptr animationList;
        Box::Ptr animationListBox;
        Button::Ptr addNewAnimationButton;

        //  //  // Text List //  //  //
        ScrolledWindow::Ptr textList;
        Box::Ptr textListBox;
        Button::Ptr addNewTextButton;

        //  //  // Rectangle List //  //  //
        ScrolledWindow::Ptr rectangleList;
        Box::Ptr rectangleListBox;
        Button::Ptr addNewRectangleButton;

        //  //  // Circle List //  //  //
        ScrolledWindow::Ptr circleList;
        Box::Ptr circleListBox;
        Button::Ptr addNewCircleButton;

        //  //  // Sprite List //  //  //
        ScrolledWindow::Ptr spriteList;
        Box::Ptr spriteListBox;
        Button::Ptr addNewSpriteButton;

        //----The list of commands available to the user when interacting with the Canvas----
        Box::Ptr toolbar;

        //----The window containing a list of components present in the currently exposed Entity-------
        ScrolledWindow::Ptr componentList;
        Box::Ptr componentListBox;
        std::map<ComponentType, Button::Ptr> openComponentButtons;           // For quickly adding callback methods
        std::map<ComponentType, CheckButton::Ptr> toggleComponentCheckButtons; // For quickly adding callback methods
        
        //----The window allowing for the user to modify which components are on an entity and modify their member values----
        ScrolledWindow::Ptr entityDesigner;
        Box::Ptr entityDesignerBox;
        Button::Ptr entityDesignerSaveChangesButton;
        ex::Entity* editingEntity; // The entity instance currently being edited (DO NOT DELETE)

        //  //  // Component Editor //  //  //
        ScrolledWindow::Ptr componentEditor;
        Box::Ptr componentEditorBox;

        //  //  // Prefab Operations Menu //  //  //
        // The horizontal box containing the following prefab manipulation buttons
        Box::Ptr prefabOperationsMenuBox;
        // Overwrites the prefab with the current entity
        Button::Ptr prefabSyncButton;
        // Makes the entity once again become a copy of the prefab
        Button::Ptr prefabRevertButton;
        // Creates a whole new prefab associated with the current entity. Fails if not modified
        Button::Ptr prefabDivertButton;

        //----The panel displaying entities are currently stored as prefabs----
        ScrolledWindow::Ptr prefabList;
        // The procedurally generated box managed by the PrefabList. Contains a list of the prefabs that can be instantiated
        Box::Ptr prefabListBox; 
        // The button used for adding new prefabs
        Button::Ptr addNewPrefabButton;

        //-----------------------Toolbar Member Variables----------------------------------

        // The label for the specific brush currently in use
        Label::Ptr currentBrush = Label::Create("Create");

        //--------------------------Formatters---------------------------------------------

        // Format a given asset list item
        void(*formatPrefabListItem)(Box::Ptr box) = [](Box::Ptr box) {
            // pointers retrieved from any GetChildren() operation MUST be acquired as, casted as, and used as RAW pointers.
            // Using shared pointers with addresses retrieved from GetChildren will result in system crashes!
            Entry* e = (Entry*) box->GetChildren()[0].get();
            std::string temp = e->GetText();
            temp = temp.substr(0, temp.size() - (temp.size() - temp.find_last_of(' ')));
            e->SetText(temp.c_str());
            e->SetRequisition(sf::Vector2f(100.f, 20.f));
            Button* binstantiate = (Button*)box->GetChildren()[1].get();
            binstantiate->SetLabel("Instantiate"); // For selecting the Entity
            Button* bduplicate = (Button*)box->GetChildren()[2].get();
            bduplicate->SetLabel("Duplicate"); // For duplicating the Entity
            Button* bdelete = (Button*)box->GetChildren()[3].get();
            bduplicate->Show(false);
            bdelete->SetLabel("X");      // For deleting the Entity
            Button* bmoveup = (Button*)box->GetChildren()[4].get();
            bmoveup->SetLabel("+");      // For moving the Entity up in the list
            bmoveup->Show(false);
            Button* bmovedown = (Button*)box->GetChildren()[5].get();
            bmovedown->SetLabel("-");      // For moving the Entity down in the list
            bmovedown->Show(false);
        };

        // Format a given asset list item
        void(*formatSceneHierarchyListItem)(Box::Ptr box) = [](Box::Ptr box) {
            // pointers retrieved from any GetChildren() operation MUST be acquired as, casted as, and used as RAW pointers.
            // Using shared pointers with addresses retrieved from GetChildren will result in system crashes!
            Entry* e = (Entry*) box->GetChildren()[0].get();
            e->SetRequisition(sf::Vector2f(100.f, 20.f));
            Button* bopen= (Button*)box->GetChildren()[1].get();
            bopen->SetLabel("Open"); // For selecting the Entity
            Button* bduplicate = (Button*)box->GetChildren()[2].get();
            bduplicate->SetLabel("Duplicate"); // For duplicating the Entity
            Button* bdelete = (Button*)box->GetChildren()[3].get();
			bdelete->GetSignal(Widget::OnLeftClick).Connect(std::bind(&GUISystem::sceneHierachyDeleteButtonHandler, cmn::game->systems.system<GUISystem>().get()->entityWidgetMap.at(0), bdelete));
            bdelete->SetLabel("X");      // For deleting the Entity
            Button* bmoveup = (Button*)box->GetChildren()[4].get();
            bmoveup->SetLabel("+");      // For moving the Entity up in the list
            bmoveup->Show(false);
            Button* bmovedown = (Button*)box->GetChildren()[5].get();
            bmovedown->SetLabel("-");      // For moving the Entity down in the list
            bmovedown->Show(false);
        };

        // Format a given asset list item
        void(*formatAssetListItem)(Box::Ptr box) = [](Box::Ptr box) {
            // pointers retrieved from any GetChildren() operation MUST be acquired as, casted as, and used as RAW pointers.
            // Using shared pointers with addresses retrieved from GetChildren will result in system crashes!
            Entry* e = (Entry*) box->GetChildren()[0].get();
            std::string temp = e->GetText();
            temp = temp.substr(0, temp.size() - (temp.size() - temp.find_last_of(' ')));
            e->SetText(temp.c_str());
            e->SetRequisition(sf::Vector2f(350.f, 20.f));
            Button* bselect = (Button*)box->GetChildren()[1].get();
            bselect->SetLabel("Select"); // For selecting the asset
            Button* bhide = (Button*)box->GetChildren()[2].get();
            bselect->Show(false);
            bhide->SetLabel(""); // For selecting the asset
            bhide->Show(false);
            Button* bdelete = (Button*)box->GetChildren()[3].get();
            bdelete->SetLabel("X");      // For deleting the asset
            Button* bmoveup = (Button*)box->GetChildren()[4].get();
            bmoveup->SetLabel("+");      // For moving the asset up in the list
            bmoveup->Show(false);
            Button* bmovedown = (Button*)box->GetChildren()[5].get();
            bmovedown->SetLabel("-");      // For moving the asset down in the list
            bmovedown->Show(false);
        };

        // Format a given asset map item
        void(*formatComplexAssetListItem)(Box::Ptr box) = [](Box::Ptr box) {
            // pointers retrieved from any GetChildren() operation MUST be acquired as, casted as, and used as RAW pointers.
            // Using shared pointers with addresses retrieved from GetChildren will result in system crashes!
            Entry* e = (Entry*) box->GetChildren()[0].get();
            std::string temp = e->GetText();
            temp = temp.substr(0, temp.size() - (temp.size() - temp.find_last_of(' ')));
            e->SetText(temp.c_str());
            e->SetRequisition(sf::Vector2f(350.f, 20.f));
            Button* bselect = (Button*)box->GetChildren()[1].get();
            bselect->SetLabel("Select"); // For selecting the asset
            Button* bopen = (Button*)box->GetChildren()[2].get();
            bselect->Show(false);
            bopen->SetLabel("Open"); // For selecting the asset
            Button* bdelete = (Button*)box->GetChildren()[3].get();
            bdelete->SetLabel("X");      // For deleting the asset
            Button* bmoveup = (Button*)box->GetChildren()[4].get();
            bmoveup->SetLabel("+");      // For moving the asset up in the list
            bmoveup->Show(false);
            Button* bmovedown = (Button*)box->GetChildren()[5].get();
            bmovedown->SetLabel("-");      // For moving the asset down in the list
            bmovedown->Show(false);
        };


        //---------------------------Constants---------------------------------------------

        // The preset name for the main window of the engine
        const static std::string MAIN_WINDOW_NAME;
    };

}

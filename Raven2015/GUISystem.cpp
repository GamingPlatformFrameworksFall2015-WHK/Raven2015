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
#include "GUISystem.h"
#include "WidgetLibrary.h"
#include "EntityLibrary.h"
#include "ComponentLibrary.h"

using namespace sfg;

namespace Raven {

    const std::string GUISystem::MAIN_WINDOW_NAME = "Raven";

    // Perform initializations of what we CAN
    GUISystem::GUISystem(std::shared_ptr<InputSystem> inputSystem, Assets* assets, ex::Entity* editingEntity) :
        mainWindow(new sf::RenderWindow(sf::VideoMode::getDesktopMode(), MAIN_WINDOW_NAME)),
        /*sf::VideoMode((unsigned int)cmn::WINDOW_WIDTH, (unsigned int)cmn::WINDOW_HEIGHT),
        MAIN_WINDOW_NAME, sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize)),*/
        sfgui(new SFGUI()),
        desktop(new Desktop()),
        event(new sf::Event()),
        input(inputSystem),
        assets(assets),
        editingEntity(editingEntity) {

        mainWindow->resetGLStates(); // Without this, items will not be rendered properly immediately
        //mainWindow->setPosition(sf::Vector2i(cmn::WINDOW_XPOS, cmn::WINDOW_YPOS));
        mainWindow->setSize(sf::Vector2u(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height - 100));

        // Create the mainGUIWindow
        mainGUIWindow = Window::Create();
        mainGUIWindow->SetStyle(MASTER_WINDOW_WTYPE::Style::BACKGROUND | MASTER_WINDOW_WTYPE::Style::RESIZE);
        mainGUIWindow->SetTitle(MASTER_WINDOW_NAME);
        mainGUIWindow->SetRequisition(sf::Vector2f((float)sf::VideoMode::getDesktopMode().width,
            (float)sf::VideoMode::getDesktopMode().height - 100.f));

        // Create the base Table
        auto table = Table::Create();

        // Create the various windows that will go inside the table and allocate regions of space for them
        canvas = formatCanvas(Canvas::Create("Canvas"));
        sceneHierarchy = formatSceneHierarchy(ScrolledWindow::Create());
        content = formatContent(Notebook::Create());
        toolbar = formatToolbar(Box::Create(Box::Orientation::VERTICAL));
        entityDesigner = formatEntityDesigner(ScrolledWindow::Create());
        componentList = formatComponentList(ScrolledWindow::Create());
        prefabList = formatPrefabList(ScrolledWindow::Create());

        // Guarantee equal sizes for entity lists
        Box::Ptr upperBox = Box::Create(Box::Orientation::HORIZONTAL);
        upperBox->Pack(sceneHierarchy);
        upperBox->Pack(prefabList);

        // Group together relevant entity design panels
        Box::Ptr lowerBox = Box::Create(Box::Orientation::HORIZONTAL);
        lowerBox->Pack(componentList);
        lowerBox->Pack(entityDesigner, true, true);

        // Distinguish editing content from game content
        Box::Ptr leftBox = Box::Create(Box::Orientation::VERTICAL);
        leftBox->Pack(upperBox);
        leftBox->Pack(lowerBox, true, true);

        Box::Ptr rightBox = Box::Create(Box::Orientation::HORIZONTAL);
        rightBox->Pack(content, true, true);
        rightBox->Pack(toolbar, true, true);

        // Add all of the various windows to the table, assigning dimensions and settings to the table
        Table::AttachOption all = (Table::AttachOption) (Table::FILL | Table::EXPAND);
        table->Attach(leftBox,  sf::Rect<sf::Uint32>( 0,  0, 11, 20));
        table->Attach(canvas,   sf::Rect<sf::Uint32>(11,  0,  9, 17));
        table->Attach(rightBox, sf::Rect<sf::Uint32>(11, 17,  9,  3));

        // Add the filled table to the mainGUIWindow
        mainGUIWindow->Add(table);
        desktop->Add(mainGUIWindow);
    }

    void GUISystem::pollEvents() {
        while (mainWindow->pollEvent(*event)) {
            desktop->HandleEvent(*event);
            if (event->type == sf::Event::Closed) {
                mainWindow->close();
            }
            //-----------------------------------------------
            // Call a function that encapsulates the event.type switch statement and simply returns the corresponding action.
            // Use the value returned by this function to then call another function that will emit an event that triggers the
            // corresponding action on all entities listening for that action via a Pawn component.
            //-----------------------------------------------
            input->handleEvent(*event);
        }
    }

    void GUISystem::update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) {
        // Update all GUI widgets associated with the set of GUI windows in all Desktops
        desktop->Update((float)dt);
    }

#pragma region Formatting Methods

    // Format the Scene Hierarchy widget
    SCENE_HIERARCHY_WTYPE_SPTR GUISystem::formatSceneHierarchy(SCENE_HIERARCHY_WTYPE_SPTR sh) {
        sh->SetScrollbarPolicy(ScrolledWindow::HORIZONTAL_AUTOMATIC | ScrolledWindow::VERTICAL_AUTOMATIC);

        // For some vertical padding at the top
        Box::Ptr top = Box::Create(Box::Orientation::VERTICAL, 5.f);
        Label::Ptr l = Label::Create("Scene Hierarchy\n");
        top->Pack(l, true, true);

        sceneHierarchyBox = WidgetLibrary::WidgetList<WidgetLibrary::SceneHierarchyPanel, ASSET_LIST_WIDGET_SEQUENCE>::Create();
        top->Pack(sceneHierarchyBox, true, true);

        Button::Ptr saveLevelButton = Button::Create("Save Level");
        saveLevelButton->GetSignal(Button::OnLeftClick).Connect([]() {
            cmn::game->saveLevel();
        });
        top->Pack(saveLevelButton, true, true);

        sh->AddWithViewport(top);
        return sh;
    }

    // Format the Texture List widget
    void GUISystem::formatTextureList(TEXTURE_LIST_WTYPE_SPTR tl) {
        textureListBox = formatAssetListHelper<WidgetLibrary::TextureListPanel>(textureList = tl, textureListBox, addNewTextureButton);
    }

    // Format the Music List widget
    void GUISystem::formatMusicList(MUSIC_LIST_WTYPE_SPTR ml) {
        musicListBox = formatAssetListHelper<WidgetLibrary::MusicListPanel>(musicList = ml, musicListBox, addNewMusicButton);
    }

    // Format the Sound List widget
    void GUISystem::formatSoundList(SOUND_LIST_WTYPE_SPTR sl) {
        soundListBox = formatAssetListHelper<WidgetLibrary::SoundListPanel>(soundList = sl, soundListBox, addNewSoundButton);
    }

    // Format the Font List widget
    void GUISystem::formatFontList(FONT_LIST_WTYPE_SPTR fl) {
        fontListBox = formatAssetListHelper<WidgetLibrary::FontListPanel>(fontList = fl, fontListBox, addNewFontButton);
    }

    // Format the Level List widget
    void GUISystem::formatLevelList(LEVEL_LIST_WTYPE_SPTR ll) {
        levelListBox = formatAssetListHelper<WidgetLibrary::LevelListPanel>(levelList = ll, levelListBox, addNewLevelButton);
    }

    // Format the Animation List widget
    void GUISystem::formatAnimationList(ANIMATION_LIST_WTYPE_SPTR al) {
        animationListBox = formatAssetListHelper<WidgetLibrary::AnimationListPanel>(animationList = al, animationListBox, addNewAnimationButton);
    }

    void GUISystem::formatTextList(TEXT_LIST_WTYPE_SPTR tl) {
        textListBox = formatAssetListHelper<WidgetLibrary::TextListPanel>(textList = tl, textListBox, addNewTextButton);
    }

    void GUISystem::formatRectangleList(RECTANGLE_LIST_WTYPE_SPTR rl) {
        rectangleListBox = formatAssetListHelper<WidgetLibrary::RectangleListPanel>(rectangleList = rl, rectangleListBox, addNewRectangleButton);
    }

    void GUISystem::formatCircleList(CIRCLE_LIST_WTYPE_SPTR cl) {
        circleListBox = formatAssetListHelper<WidgetLibrary::CircleListPanel>(circleList = cl, circleListBox, addNewCircleButton);
    }

    void GUISystem::formatSpriteList(SPRITE_LIST_WTYPE_SPTR sl) {
        spriteListBox = formatAssetListHelper<WidgetLibrary::SpriteListPanel>(spriteList = sl, spriteListBox, addNewSpriteButton);
    }

    template <typename T>
    Box::Ptr GUISystem::formatAssetListHelper(ScrolledWindow::Ptr list, Box::Ptr listBox, Button::Ptr addNewButton) {

        // Setup scroll bars
        list->SetScrollbarPolicy(ScrolledWindow::HORIZONTAL_AUTOMATIC | ScrolledWindow::VERTICAL_AUTOMATIC);

        // For some vertical padding at the top
        Box::Ptr top = Box::Create(Box::Orientation::VERTICAL, 5.f);
        Label::Ptr l = Label::Create("\n");
        top->Pack(l, true, true);

        // Create the dynamic widget
        listBox = WidgetLibrary::WidgetList<T, ASSET_LIST_WIDGET_SEQUENCE>::Create();

        top->Pack(listBox, true, true);

        // Create a button for adding new items
        addNewButton = Button::Create("Add New...");

        top->Pack(addNewButton);

        list->AddWithViewport(top);

        return listBox;
    }

    // Format the Content widget
    CONTENT_WTYPE_SPTR GUISystem::formatContent(CONTENT_WTYPE_SPTR c) {
        formatTextureList(ScrolledWindow::Create());
        formatMusicList(ScrolledWindow::Create());
        formatSoundList(ScrolledWindow::Create());
        formatFontList(ScrolledWindow::Create());
        formatLevelList(ScrolledWindow::Create());
        formatAnimationList(ScrolledWindow::Create());
        formatTextList(ScrolledWindow::Create());
        formatRectangleList(ScrolledWindow::Create());
        formatCircleList(ScrolledWindow::Create());
        formatSpriteList(ScrolledWindow::Create());
        c->AppendPage(textureList, Label::Create(TEXTURE_LIST_NAME));
        c->AppendPage(musicList, Label::Create(MUSIC_LIST_NAME));
        c->AppendPage(soundList, Label::Create(SOUND_LIST_NAME));
        c->AppendPage(fontList, Label::Create(FONT_LIST_NAME));
        c->AppendPage(levelList, Label::Create(LEVEL_LIST_NAME));
        c->AppendPage(animationList, Label::Create(ANIMATION_LIST_NAME));
        c->AppendPage(textList, Label::Create(TEXT_LIST_NAME));
        c->AppendPage(rectangleList, Label::Create(RECTANGLE_LIST_NAME));
        c->AppendPage(circleList, Label::Create(CIRCLE_LIST_NAME));
        c->AppendPage(spriteList, Label::Create(SPRITE_LIST_NAME));

        return c;
    }

    // Format the Entity Designer widget
    ENTITY_DESIGNER_WTYPE_SPTR GUISystem::formatEntityDesigner(ENTITY_DESIGNER_WTYPE_SPTR ed) {
        ed->SetScrollbarPolicy(ScrolledWindow::HORIZONTAL_AUTOMATIC | ScrolledWindow::VERTICAL_AUTOMATIC);

        // For some vertical padding at the top
        Box::Ptr top = Box::Create(Box::Orientation::VERTICAL, 5.f);
        top->SetRequisition(sf::Vector2f(200.f, 0.f));
        Label::Ptr l = Label::Create("Entity Designer\n");
        top->Pack(l, true, true);

        prefabOperationsMenuBox = Box::Create(Box::Orientation::HORIZONTAL);
        prefabSyncButton = Button::Create("Sync");
        prefabDivertButton = Button::Create("Divert");
        prefabRevertButton = Button::Create("Revert");
        prefabSyncButton->GetSignal(Button::OnLeftClick).Connect(std::bind(&GUISystem::prefabSyncButtonHandler, this, prefabSyncButton.get()));
        prefabDivertButton->GetSignal(Button::OnLeftClick).Connect(std::bind(&GUISystem::prefabDivertButtonHandler, this, prefabDivertButton.get()));
        prefabRevertButton->GetSignal(Button::OnLeftClick).Connect(std::bind(&GUISystem::prefabRevertButtonHandler, this, prefabRevertButton.get()));
        prefabOperationsMenuBox->Pack(prefabSyncButton, true, true);
        prefabOperationsMenuBox->Pack(prefabDivertButton, true, true);
        prefabOperationsMenuBox->Pack(prefabRevertButton, true, true);
        top->Pack(prefabOperationsMenuBox, true, true);

        entityDesignerBox = WidgetLibrary::WidgetList<WidgetLibrary::EntityDesignerPanel, ED_ASSET_LIST_WIDGET_SEQUENCE>::Create();

        entityDesignerSaveChangesButton = Button::Create("Save");
        entityDesignerSaveChangesButton->GetSignal(Button::OnLeftClick).Connect(std::bind(&GUISystem::entityDesignerSaveChangesButtonHandler, this, entityDesignerSaveChangesButton.get()));
        entityDesignerSaveChangesButton->SetRequisition(sf::Vector2f(200.f, 0.f));

        Table::Ptr table = Table::Create();
        Table::AttachOption all = (Table::AttachOption) (Table::FILL | Table::EXPAND);
        table->Attach(top,                              sf::Rect<sf::Uint32>(0, 0, 1, 1), all, all);
        table->Attach(entityDesignerBox,                sf::Rect<sf::Uint32>(0, 1, 1, 7), all, all);
        table->Attach(entityDesignerSaveChangesButton,  sf::Rect<sf::Uint32>(0, 8, 1, 1), all, all);

        ed->AddWithViewport(table);

        return ed;
    }

    COMPONENT_LIST_WTYPE_SPTR GUISystem::formatComponentList(COMPONENT_LIST_WTYPE_SPTR cl) {
        cl->SetScrollbarPolicy(ScrolledWindow::HORIZONTAL_AUTOMATIC | ScrolledWindow::VERTICAL_AUTOMATIC);

        formatComponentListHelper<COMPONENT_TYPE_LIST>(COMPONENT_TYPES(::getNullPtrToType()));

        cl->AddWithViewport(componentListBox);
        return cl;
    }

    // Format the Component List widget, Base Case
    template <typename C>
    void GUISystem::formatComponentListHelper(C* c) {
        formatComponentListItem<C>(c);
    }

    // Format the Component List widget, Recursive Case
    template <typename C, typename... Components>
    void GUISystem::formatComponentListHelper(C* c, Components*... components) {
        // Initialize the box if necessary
        if (sizeof...(Components) == (ComponentType::NumComponentTypes - 1)) {
            componentListBox = Box::Create(Box::Orientation::VERTICAL);
        }
        formatComponentListItem<C>(c);
        formatComponentListHelper<Components...>(components...);
    }

    template <typename C>
    void GUISystem::formatComponentListItem(C* c) {
        Box::Ptr box = Box::Create(Box::Orientation::HORIZONTAL);
        Label::Ptr l = Label::Create(c->getElementName());
        openComponentButtons.insert(std::make_pair(c->getType(), Button::Create("Open")));
        l->SetRequisition(sf::Vector2f(150.0f, 10.f));
        box->Pack(l);
        box->Pack(openComponentButtons[c->getType()]);

        if (c->getType() < NUM_REQUIRED_COMPONENTS) {
            box->Pack(Label::Create("    "));
        }
        else {
            toggleComponentCheckButtons.insert(std::make_pair(c->getType(), CheckButton::Create("")));
            box->Pack(toggleComponentCheckButtons[c->getType()]);
        }

        componentListBox->Pack(box);
    }

    // Format the Prefab List widget
    PREFAB_LIST_WTYPE_SPTR GUISystem::formatPrefabList(PREFAB_LIST_WTYPE_SPTR pl) {
        pl->SetScrollbarPolicy(ScrolledWindow::HORIZONTAL_AUTOMATIC | ScrolledWindow::VERTICAL_AUTOMATIC);

        // For some vertical padding at the top
        Box::Ptr top = Box::Create(Box::Orientation::VERTICAL, 5.f);
        top->SetRequisition(sf::Vector2f(200.f, 0.f));
        Label::Ptr l = Label::Create("Prefab List\n");
        top->Pack(l, true, true);

        prefabListBox = WidgetLibrary::WidgetList<WidgetLibrary::PrefabListPanel, ASSET_LIST_WIDGET_SEQUENCE>::Create();

        top->Pack(prefabListBox, true, true);

        pl->AddWithViewport(top);

        return pl;
    }

    // Format the Canvas widget
    CANVAS_WTYPE_SPTR GUISystem::formatCanvas(CANVAS_WTYPE_SPTR c) {
        c->GetSignal(Widget::OnLeftClick).Connect(std::bind(&GUISystem::canvasClickHandler, this));
        return c;
    }

    // Format the Toolbar widget
    TOOLBAR_WTYPE_SPTR GUISystem::formatToolbar(TOOLBAR_WTYPE_SPTR t) {

        auto brushList = Box::Create(Box::Orientation::HORIZONTAL);
        // Create the various Brush modes we will enter into
        auto createBrush = Button::Create("Create");
        createBrush->GetSignal(Widget::OnLeftClick).Connect(std::bind(&GUISystem::brushToolbarButtonHandler, this, createBrush.get()));
        auto deleteBrush = Button::Create("Delete");
        deleteBrush->GetSignal(Widget::OnLeftClick).Connect(std::bind(&GUISystem::brushToolbarButtonHandler, this, deleteBrush.get()));
        auto moveBrush = Button::Create("Move");
        moveBrush->GetSignal(Widget::OnLeftClick).Connect(std::bind(&GUISystem::brushToolbarButtonHandler, this, moveBrush.get()));
        // Add those brushes to our list of brushes
        brushList->Pack(createBrush, true, true);
        brushList->Pack(deleteBrush, true, true);
        brushList->Pack(moveBrush, true, true);
        t->Pack(currentBrush);
        t->Pack(brushList);

        return t;
    }

#pragma endregion

#pragma region Dynamic Panel Manipulation

    void GUISystem::populatePrefabList(XMLDocument& prefabsDoc) {
        prefabListBox->RemoveAll();
        XMLElement* top = prefabsDoc.RootElement();
        XMLElement* entityNode = top->FirstChildElement("Entity");
        while (entityNode) {
            WidgetLibrary::WidgetList<WidgetLibrary::PrefabListPanel, ASSET_LIST_WIDGET_SEQUENCE>::appendWidget(
                prefabListBox, entityNode->FirstChildElement("Data")->FirstChildElement("Name")->GetText(), formatPrefabListItem);

            entityNode = entityNode->NextSiblingElement("Entity");
        }
        configureWidgetList(prefabListBox, formatPrefabListItem);
    }

    void GUISystem::addItemToPrefabList(std::string itemName) {
        WidgetLibrary::WidgetList<WidgetLibrary::PrefabListPanel, ASSET_LIST_WIDGET_SEQUENCE>::appendWidget(prefabListBox, itemName, formatPrefabListItem);
    }

    void GUISystem::removeItemFromPrefabList(std::string itemName) {
        WidgetLibrary::WidgetList<WidgetLibrary::PrefabListPanel, ASSET_LIST_WIDGET_SEQUENCE>::removeWidget(prefabListBox, itemName);
    }

    void GUISystem::updateXMLPrefabName(Entry* entry, std::string previousName) {

        XMLElement* top = assets->prefabsDoc->FirstChildElement("ASSETS");
        XMLElement* ent = top->FirstChildElement("Entity");
        while (ent) {

            XMLElement* nameNode = ent->FirstChildElement("Data")->FirstChildElement("Name");

            if (nameNode->GetText() == previousName.c_str()) {
                nameNode->SetText(entry->GetText().toAnsiString().c_str());
                return;
            }

            ent = ent->NextSiblingElement("Entity");
        }
    }

    void GUISystem::populateSceneHierarchy(std::set<ex::Entity>& entitySet) {
        sceneHierarchyBox->RemoveAll();
        assets->entitiesByWidget->clear();
        for (auto entity : entitySet) {
            Box::Ptr box = WidgetLibrary::WidgetList<WidgetLibrary::PrefabListPanel, ASSET_LIST_WIDGET_SEQUENCE>::appendWidget(
                sceneHierarchyBox, entity.component<Data>()->name, formatSceneHierarchyListItem);
            assets->entitiesByWidget->insert(std::make_pair(box,entity));
        }
        configureWidgetList(sceneHierarchyBox, formatSceneHierarchyListItem);
    }

    void GUISystem::addItemToSceneHierarchy(std::string itemName) {
        WidgetLibrary::WidgetList<WidgetLibrary::SceneHierarchyPanel, ASSET_LIST_WIDGET_SEQUENCE>::appendWidget(sceneHierarchyBox, itemName, formatSceneHierarchyListItem);
    }

    void GUISystem::removeItemFromSceneHierarchy(std::string itemName) {
        WidgetLibrary::WidgetList<WidgetLibrary::SceneHierarchyPanel, ASSET_LIST_WIDGET_SEQUENCE>::removeWidget(sceneHierarchyBox, itemName);
    }

    void GUISystem::populateTextureList(std::set<std::string> assetList) {
        populateAssetList<WidgetLibrary::TextureListPanel>(textureListBox, assetList);
    }

    void GUISystem::populateMusicList(std::set<std::string> assetList) {
        populateAssetList<WidgetLibrary::MusicListPanel>(musicListBox, assetList);
    }

    void GUISystem::populateSoundList(std::set<std::string> assetList) {
        populateAssetList<WidgetLibrary::SoundListPanel>(soundListBox, assetList);
    }

    void GUISystem::populateFontList(std::set<std::string> assetList) {
        populateAssetList<WidgetLibrary::FontListPanel>(fontListBox, assetList);
    }

    void GUISystem::populateLevelList(std::set<std::string> assetList) {
        levelListBox->RemoveAll();
        for (auto level : assetList) {
            WidgetLibrary::WidgetList<WidgetLibrary::AnimationListPanel, ASSET_LIST_WIDGET_SEQUENCE>::appendWidget(
                levelListBox, level, formatComplexAssetListItem);
        }
    }

    void GUISystem::populateAnimationList(std::map<std::string, std::shared_ptr<Animation>>& map) {
        animationListBox->RemoveAll();
        for (auto name_animation : map) {
            WidgetLibrary::WidgetList<WidgetLibrary::AnimationListPanel, ASSET_LIST_WIDGET_SEQUENCE>::appendWidget(
                animationListBox, name_animation.first, formatComplexAssetListItem);
        }
    }

    void GUISystem::populateTextList(std::map<std::string, std::shared_ptr<RenderableText>>& map) {
        textListBox->RemoveAll();
        for (auto name_text : map) {
            WidgetLibrary::WidgetList<WidgetLibrary::TextListPanel, ASSET_LIST_WIDGET_SEQUENCE>::appendWidget(
                textListBox, name_text.first, formatComplexAssetListItem);
        }
    }

    void GUISystem::populateRectangleList(std::map<std::string, std::shared_ptr<RenderableRectangle>>& map) {
        rectangleListBox->RemoveAll();
        for (auto name_rectangle : map) {
            WidgetLibrary::WidgetList<WidgetLibrary::RectangleListPanel, ASSET_LIST_WIDGET_SEQUENCE>::appendWidget(
                rectangleListBox, name_rectangle.first, formatComplexAssetListItem);
        }
    }

    void GUISystem::populateCircleList(std::map<std::string, std::shared_ptr<RenderableCircle>>& map) {
        circleListBox->RemoveAll();
        for (auto name_circle : map) {
            WidgetLibrary::WidgetList<WidgetLibrary::CircleListPanel, ASSET_LIST_WIDGET_SEQUENCE>::appendWidget(
                circleListBox, name_circle.first, formatComplexAssetListItem);
        }
    }

    void GUISystem::populateSpriteList(std::map<std::string, std::shared_ptr<RenderableSprite>>& map) {
        spriteListBox->RemoveAll();
        for (auto name_sprite : map) {
            WidgetLibrary::WidgetList<WidgetLibrary::SpriteListPanel, ASSET_LIST_WIDGET_SEQUENCE>::appendWidget(
                spriteListBox, name_sprite.first, formatComplexAssetListItem);
        }
    }

    template <typename T>
    void GUISystem::populateAssetList(Box::Ptr assetListWidget, std::set<std::string>& assetList) {
        assetListWidget->RemoveAll();
        for (auto filePath : assetList) {
            WidgetLibrary::WidgetList<T, ASSET_LIST_WIDGET_SEQUENCE>::appendWidget(
                assetListWidget, filePath, formatAssetListItem);
        }
    }

    template <typename T>
    void GUISystem::addItemToAssetList(Box::Ptr assetListWidget, std::string itemName, void(*formatter)(Box::Ptr)) {
        WidgetLibrary::WidgetList<T, ASSET_LIST_WIDGET_SEQUENCE>::appendWidget(assetListWidget, itemName, formatter);
    }

    template <typename T>
    void GUISystem::removeItemFromAssetList(Box::Ptr assetListWidget, std::string itemName) {
        WidgetLibrary::WidgetList<T, ASSET_LIST_WIDGET_SEQUENCE>::removeWidget(assetListWidget, itemName);
    }

    template <typename T, typename Asett>
    void populateComplexAssetList(Box::Ptr assetMapWidget, std::map<std::string, Asett>& assetMap) {
        assetListWidget->RemoveAll();
        for (auto name_asset : assetMap) {
            WidgetLibrary::WidgetList<T, ASSET_LIST_WIDGET_SEQUENCE>::appendWidget(
                assetMapWidget, name_asset.second, formatComplexAssetListItem);
        }
    }

    template <typename T, typename Asett>
    void addItemToComplexAssetList(Box::Ptr assetMapWidget, std::string itemName) {
        WidgetLibrary::WidgetList<T, ASSET_LIST_WIDGET_SEQUENCE>::appendWidget(assetMapWidget, itemName, formatComplexAssetListItem);
    }

    template <typename T, typename Asett>
    void removeItemFromComplexAssetList(Box::Ptr assetMapWidget, std::string itemName) {
        WidgetLibrary::WidgetList<T, ASSET_LIST_WIDGET_SEQUENCE>::removeWidget(assetMapWidget, itemName);
    }


#pragma endregion

#pragma region Callback Methods

    void GUISystem::brushToolbarButtonHandler(Button* button) {
        cout << button->GetLabel().toAnsiString() << " Button Clicked" << endl;
        currentBrush->SetText(button->GetLabel().toAnsiString());
    }

    void GUISystem::sceneHierarchyEntryHandler(Entry* entry) {
        assets->entitiesByWidget->at(entry->GetParent()).component<Data>()->name = entry->GetText();
    }

    void GUISystem::sceneHierarchyOpenButtonHandler(Button* button) {
        cout << button->GetLabel().toAnsiString() << " Button Clicked" << endl;
    }

    void GUISystem::sceneHierarchyDuplicateButtonHandler(Button* button) {
        ex::Entity e = assets->entitiesByWidget->at(button->GetParent());
        ex::Entity e2 = EntityLibrary::Create::Entity();
        EntityLibrary::copyEntity(e2, e);
        assets->entities->insert(e2);
        WidgetLibrary::WidgetList<WidgetLibrary::SceneHierarchyPanel, ASSET_LIST_WIDGET_SEQUENCE>::appendWidget(
            sceneHierarchyBox, e2.component<Data>()->name, formatSceneHierarchyListItem);
    }

    void GUISystem::sceneHierarchyDeleteButtonHandler(Button* button) {
        ex::Entity e = assets->entitiesByWidget->at(button->GetParent());
        assets->entities->erase(e);
        e.destroy();
    }

    void GUISystem::sceneHierarchyMoveUpButtonHandler(Button* button) { //ignored for now

    }

    void GUISystem::sceneHierarchyMoveDownButtonHandler(Button* button) { //ignored for now

    }

    void GUISystem::prefabListInstantiateButtonHandler(Button* button) {
        cout << button->GetLabel().toAnsiString() << " Button Clicked" << endl;
        /*for (auto child : getEntityDesigner()->GetChildren()) {
            cout << child->GetName() << endl;
        }*/
        //change things in the entity designer and display things according to the
        //clickedButton->GetLabel().toAnsiString() which is the name that is display to user
    }

    void GUISystem::prefabListDeleteButtonHandler(Button* button) {

    }

    void GUISystem::prefabListMoveUpButtonHandler(Button* button) { //ignored for now

    }

    void GUISystem::prefabListMoveDownButtonHandler(Button* button) { //ignored for now

    }

    void GUISystem::prefabListEntryHandler(Entry* entry, std::string previousName) {
        updateXMLPrefabName(entry, previousName);
    }

    void GUISystem::canvasClickHandler() {
        sf::Vector2i position = sf::Mouse::getPosition();
        cout << canvas->GetAbsolutePosition().x << " " << canvas->GetAbsolutePosition().y << endl;
        if (currentBrush->GetText().toAnsiString() == "Create") {
            ex::Entity entity = EntityLibrary::Create::Entity("Spawned Tracker");
            cmn::game->events.emit<XMLLogEntityEvent>(entity);
            addItemToAssetList<WidgetLibrary::SceneHierarchyPanel>(
                sceneHierarchyBox, entity.component<Data>()->name, formatSceneHierarchyListItem);
            configureWidgetListItem(sceneHierarchyBox, sceneHierarchyBox->GetChildren().size() - 1, formatSceneHierarchyListItem);
            assets->entitiesByWidget->insert(std::make_pair(sceneHierarchyBox, entity));
            entity.assign<Tracker>();
            entity.assign<BoxCollider>()->collisionSettings.insert(COLLISION_LAYER_SETTINGS_SOLID);
            auto transform = entity.component<Transform>();
            transform->transform.x = (float)position.x - canvas->GetAbsolutePosition().x;
            transform->transform.y = (float)position.y - canvas->GetAbsolutePosition().y;
            cout << "Created entity @ (" + std::to_string(transform->transform.x) + ", " + std::to_string(transform->transform.y) + ")" << endl;
            ex::ComponentHandle<rvn::Renderer> renderer = entity.assign<rvn::Renderer>();
            renderer->sprites["PlayerSprite"] = std::shared_ptr<RenderableSprite>(new RenderableSprite(*cmn::game->getAssets()->sprites->at("PlayerSprite")));
        }
    }

    void GUISystem::prefabSyncButtonHandler(Button* button) {

    }


    void GUISystem::prefabDivertButtonHandler(Button* button) {

    }


    void GUISystem::prefabRevertButtonHandler(Button* button) {

    }


    void GUISystem::assignAssetEditorWidget(Button* button) {

    }

    void GUISystem::entityDesignerSaveChangesButtonHandler(Button* button) {

    }

#pragma endregion

    //----------------------------Helper methods for widget lists---------------------
    Entry* GUISystem::getAssetNameEntry(Box::Ptr box, size_t position) {
        return (Entry*)((Box*)box->GetChildren()[position].get())->GetChildren()[0].get();
    }

    Button* GUISystem::getAssetOpenButton(Box::Ptr box, size_t position) {
        return (Button*)((Box*)box->GetChildren()[position].get())->GetChildren()[1].get();
    }

    Button* GUISystem::getAssetDuplicateButton(Box::Ptr box, size_t position) {
        return (Button*)((Box*)box->GetChildren()[position].get())->GetChildren()[2].get();
    }

    Button* GUISystem::getAssetDeleteButton(Box::Ptr box, size_t position) {
        return (Button*)((Box*)box->GetChildren()[position].get())->GetChildren()[3].get();
    }

    void GUISystem::configureWidgetListItem(Box::Ptr verticalBoxForList, size_t position, void(*formatter)(Box::Ptr)) {
        if (formatter == formatSceneHierarchyListItem) {
            Entry* e = getAssetNameEntry(sceneHierarchyBox, position);
            e->GetSignal(Entry::OnTextChanged).Connect(std::bind(&GUISystem::sceneHierarchyEntryHandler, this, e));
            Button* bopen = getAssetOpenButton(sceneHierarchyBox, position);
            bopen->GetSignal(Button::OnLeftClick).Connect(std::bind(&GUISystem::sceneHierarchyOpenButtonHandler, this, bopen));
            Button* bduplicate = getAssetDuplicateButton(sceneHierarchyBox, position);
            bduplicate->GetSignal(Button::OnLeftClick).Connect(std::bind(&GUISystem::sceneHierarchyDuplicateButtonHandler, this, bduplicate));
            Button* bdelete = getAssetDeleteButton(sceneHierarchyBox, position);
            bdelete->GetSignal(Button::OnLeftClick).Connect(std::bind(&GUISystem::sceneHierarchyDeleteButtonHandler, this, bdelete));
        }
        else if (formatter == formatPrefabListItem) {
            Entry* e = getAssetNameEntry(prefabListBox, position);
            //e->GetSignal(Entry::OnTextChanged).Connect(std::bind(&GUISystem::prefabListEntryHandler, this, e, e->GetText()));
        }
        else if (formatter == formatAssetListItem) {

        }
        else if (formatter == formatComplexAssetListItem) {

        }

    }

    void GUISystem::configureWidgetList(Box::Ptr verticalBoxForList, void(*formatter)(Box::Ptr)) {
        for (int i = 0; i < verticalBoxForList->GetChildren().size(); ++i) {
            configureWidgetListItem(verticalBoxForList, i, formatter);
        }
    }

}

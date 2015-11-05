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
    GUISystem::GUISystem(std::shared_ptr<InputSystem> inputSystem, ex::Entity* editingEntity) :
        mainWindow(new sf::RenderWindow(sf::VideoMode::getDesktopMode(), MAIN_WINDOW_NAME)),
        /*sf::VideoMode((unsigned int)cmn::WINDOW_WIDTH, (unsigned int)cmn::WINDOW_HEIGHT),
        MAIN_WINDOW_NAME, sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize)),*/
        sfgui(new SFGUI()),
        desktop(new Desktop()),
        event(new sf::Event()),
        input(inputSystem),
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
        // Implement a 6x5 table with the following structure
        // Scene Hierarchy(SH), Content(C), ComponentList (CL), Entity Designer (ED), Prefab List(PL), Canvas(V), Toolbar(T)
        //    0  1  2  3  4  5  6  7  8
        // 0|SH|SH|PL|PL| V| V| V| V| V|
        // 1|SH|SH|PL|PL| V| V| V| V| V|
        // 2|SH|SH|PL|PL| V| V| V| V| V|
        // 3|SH|SH|PL|PL| V| V| V| V| V|
        // 4|SH|SH|PL|PL| V| V| V| V| V|
        // 5|SH|SH|PL|PL| T| T| T| T| T|
        // 6| C|CL|CL|ED|ED|ED|ED|ED|ED|
        // 7| C|CL|CL|ED|ED|ED|ED|ED|ED|
        // 8| C|CL|CL|ED|ED|ED|ED|ED|ED|
        canvas = formatCanvas(Canvas::Create("Canvas"));
        sceneHierarchy = formatSceneHierarchy(ScrolledWindow::Create());
        content = formatContent(Notebook::Create());
        toolbar = formatToolbar(Box::Create(Box::Orientation::VERTICAL));
        entityDesigner = formatEntityDesigner(ScrolledWindow::Create());
        componentList = formatComponentList<COMPONENT_TYPE_LIST>(ScrolledWindow::Create(), COMPONENT_TYPES(::getNullPtrToType()));
        prefabList = formatPrefabList(ScrolledWindow::Create());
        
        /*
        // Original Layout. Screen is too small + failed attempts to merge conflicting panel dimensions
        table->Attach(sceneHierarchy, sf::Rect<sf::Uint32>(0, 0, 1, 6), all, all);
        table->Attach(canvas, sf::Rect<sf::Uint32>(1, 0, 5, 5));
        table->Attach(prefabList, sf::Rect<sf::Uint32>(6, 0, 3, 6));
        table->Attach(toolbar, sf::Rect<sf::Uint32>(1, 5, 5, 1));
        table->Attach(content, sf::Rect<sf::Uint32>(0, 6, 1, 3));
        table->Attach(componentList, sf::Rect<sf::Uint32>(1, 6, 2, 3));
        table->Attach(entityDesigner, sf::Rect<sf::Uint32>(3, 6, 6, 3));
        */

        // Guarantee equal sizes for entity lists
        Box::Ptr upperBox = Box::Create(Box::Orientation::HORIZONTAL);
        upperBox->Pack(sceneHierarchy);
        upperBox->Pack(prefabList);

        // Group together relevant entity design panels
        Box::Ptr lowerBox = Box::Create(Box::Orientation::HORIZONTAL);
        lowerBox->Pack(componentList);
        lowerBox->Pack(entityDesigner);

        // Distinguish editing content from game content
        Box::Ptr leftBox = Box::Create(Box::Orientation::VERTICAL);
        leftBox->Pack(upperBox, true, true);
        leftBox->Pack(lowerBox, true, true);

        // Add all of the various windows to the table, assigning dimensions and settings to the table
        Table::AttachOption all = (Table::AttachOption) (Table::FILL | Table::EXPAND);
        table->Attach(leftBox,  sf::Rect<sf::Uint32>(0, 0, 14, 20));
        table->Attach(canvas,   sf::Rect<sf::Uint32>(14, 0, 6, 15));
        table->Attach(toolbar,  sf::Rect<sf::Uint32>(14, 14, 6, 1));
        table->Attach(content,  sf::Rect<sf::Uint32>(14, 16, 6, 4));

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

        addNewEntityButton = Button::Create("Add New...");

        top->Pack(addNewEntityButton);

        /*
        auto vbox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5);
        auto hbox1 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
		auto button = sfg::Button::Create("Entity 1 I HAVE A SERIOUSLY LONG NAME RIGHT NAO");
		button->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&GUISystem::sceneHierachyButton, this, button));
        hbox1->Pack(button, true, false);
        auto hbox2 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
		button = sfg::Button::Create("Entity 2");
		button->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&GUISystem::sceneHierachyButton, this, button));
        hbox2->Pack(button, true, false);
		button = sfg::Button::Create("Entity 3");
		button->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&GUISystem::sceneHierachyButton, this, button));
        auto hbox3 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
        hbox3->Pack(button, true, false);
        vbox->Pack(hbox1, true, true);
        vbox->Pack(hbox2, true, true);
        vbox->Pack(hbox3, true, true);
        */

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

        return ed;
    }

    // Format the Component List widget, Base Case
    template <typename C>
    COMPONENT_LIST_WTYPE_SPTR GUISystem::formatComponentList(COMPONENT_LIST_WTYPE_SPTR cl, C* c) {
        cl->SetScrollbarPolicy(ScrolledWindow::HORIZONTAL_AUTOMATIC | ScrolledWindow::VERTICAL_AUTOMATIC);

        formatComponentListHelper<C>(c);

        cl->AddWithViewport(componentListBox);
        return cl;
    }

    // Format the Component List widget, Recursive Case
    template <typename C, typename... Components>
    COMPONENT_LIST_WTYPE_SPTR GUISystem::formatComponentList(COMPONENT_LIST_WTYPE_SPTR cl, C* c, Components*... components) {
        cl->SetScrollbarPolicy(ScrolledWindow::HORIZONTAL_AUTOMATIC | ScrolledWindow::VERTICAL_AUTOMATIC);

        // Initialize the box if necessary
        if (sizeof...(Components) == (ComponentType::NumComponentTypes - 1)) {
            componentListBox = Box::Create(Box::Orientation::VERTICAL);
        }

        formatComponentListHelper<C>(c);

        // recurse
        return formatComponentList<Components...>(cl, components...);
    }

    template <typename C>
    void GUISystem::formatComponentListHelper(C* c) {
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
        top->SetRequisition(sf::Vector2f(400.f, 0.f));
        Label::Ptr l = Label::Create("Prefab List\n");
        top->Pack(l, true, true);

        prefabListBox = WidgetLibrary::WidgetList<WidgetLibrary::PrefabListPanel, ASSET_LIST_WIDGET_SEQUENCE>::Create();

        top->Pack(prefabListBox, true, true);

        addNewPrefabButton = Button::Create("Add New...");

        top->Pack(addNewPrefabButton);

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
        createBrush->GetSignal(Widget::OnLeftClick).Connect(std::bind(&GUISystem::brushToolbarButtonHandler, this, createBrush));
        auto deleteBrush = Button::Create("Delete");
        deleteBrush->GetSignal(Widget::OnLeftClick).Connect(std::bind(&GUISystem::brushToolbarButtonHandler, this, deleteBrush));
        auto moveBrush = Button::Create("Move");
        moveBrush->GetSignal(Widget::OnLeftClick).Connect(std::bind(&GUISystem::brushToolbarButtonHandler, this, moveBrush));
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
    }

    void GUISystem::addItemToPrefabList(std::string itemName) {
        WidgetLibrary::WidgetList<WidgetLibrary::PrefabListPanel, ASSET_LIST_WIDGET_SEQUENCE>::appendWidget(prefabListBox, itemName, formatPrefabListItem);
    }

    void GUISystem::removeItemFromPrefabList(std::string itemName) {
        WidgetLibrary::WidgetList<WidgetLibrary::PrefabListPanel, ASSET_LIST_WIDGET_SEQUENCE>::removeWidget(prefabListBox, itemName);
    }

    void GUISystem::populateSceneHierarchy(std::set<ex::Entity>& entitySet) {
        sceneHierarchyBox->RemoveAll();
        for (auto entity : entitySet) {
            WidgetLibrary::WidgetList<WidgetLibrary::PrefabListPanel, ASSET_LIST_WIDGET_SEQUENCE>::appendWidget(
                sceneHierarchyBox, entity.component<Data>()->name, formatSceneHierarchyListItem);
        }
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
        populateAssetList<WidgetLibrary::LevelListPanel>(levelListBox, assetList);
    }

    void GUISystem::populateAnimationList(std::map<std::string, std::shared_ptr<Animation>>& map) {
        for (auto name_animation : map) {
            WidgetLibrary::WidgetList<WidgetLibrary::AnimationListPanel, ASSET_LIST_WIDGET_SEQUENCE>::appendWidget(
                animationListBox, name_animation.first, formatComplexAssetListItem);
        }
    }

    void GUISystem::populateTextList(std::map<std::string, std::shared_ptr<RenderableText>>& map) {
        for (auto name_text : map) {
            WidgetLibrary::WidgetList<WidgetLibrary::TextListPanel, ASSET_LIST_WIDGET_SEQUENCE>::appendWidget(
                textListBox, name_text.first, formatComplexAssetListItem);
        }
    }

    void GUISystem::populateRectangleList(std::map<std::string, std::shared_ptr<RenderableRectangle>>& map) {
        for (auto name_rectangle : map) {
            WidgetLibrary::WidgetList<WidgetLibrary::RectangleListPanel, ASSET_LIST_WIDGET_SEQUENCE>::appendWidget(
                rectangleListBox, name_rectangle.first, formatComplexAssetListItem);
        }
    }

    void GUISystem::populateCircleList(std::map<std::string, std::shared_ptr<RenderableCircle>>& map) {
        for (auto name_circle : map) {
            WidgetLibrary::WidgetList<WidgetLibrary::CircleListPanel, ASSET_LIST_WIDGET_SEQUENCE>::appendWidget(
                circleListBox, name_circle.first, formatComplexAssetListItem);
        }
    }

    void GUISystem::populateSpriteList(std::map<std::string, std::shared_ptr<RenderableSprite>>& map) {
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

    void GUISystem::brushToolbarButtonHandler(Button::Ptr button) {
        cout << button->GetLabel().toAnsiString() << " Button Clicked" << endl;
        currentBrush->SetText(button->GetLabel().toAnsiString());
    }

    void GUISystem::sceneHierachySelectButtonHandler(Button::Ptr button) {
        cout << button->GetLabel().toAnsiString() << " Button Clicked" << endl;
        /*for (auto child : getEntityDesigner()->GetChildren()) {
            cout << child->GetName() << endl;
        }*/
        //change things in the entity designer and display things according to the
        //clickedButton->GetLabel().toAnsiString() which is the name that is display to user
    }

    void GUISystem::sceneHierachyDeleteButtonHandler(Button::Ptr button) {

    }

    void GUISystem::sceneHierachyMoveUpButtonHandler(Button::Ptr button) {

    }

    void GUISystem::sceneHierachyMoveDownButtonHandler(Button::Ptr button) {

    }

    void GUISystem::sceneHierachyEntryHandler(Entry::Ptr entry) {
        cout << entry->GetText().toAnsiString() << " Button Clicked" << endl;
        /*for (auto child : getEntityDesigner()->GetChildren()) {
            cout << child->GetName() << endl;
        }*/
        //change things in the entity designer and display things according to the
        //clickedButton->GetLabel().toAnsiString() which is the name that is display to user
    }

    void GUISystem::prefabListSelectButtonHandler(Button::Ptr button) {
        cout << button->GetLabel().toAnsiString() << " Button Clicked" << endl;
        /*for (auto child : getEntityDesigner()->GetChildren()) {
            cout << child->GetName() << endl;
        }*/
        //change things in the entity designer and display things according to the
        //clickedButton->GetLabel().toAnsiString() which is the name that is display to user
    }

    void GUISystem::prefabListDeleteButtonHandler(Button::Ptr button) {

    }

    void GUISystem::prefabListMoveUpButtonHandler(Button::Ptr button) {

    }

    void GUISystem::prefabListMoveDownButtonHandler(Button::Ptr button) {

    }

    void GUISystem::prefabListEntryHandler(Entry::Ptr entry) {
        cout << entry->GetText().toAnsiString() << " Button Clicked" << endl;
        /*for (auto child : getEntityDesigner()->GetChildren()) {
            cout << child->GetName() << endl;
        }*/
        //change things in the entity designer and display things according to the
        //clickedButton->GetLabel().toAnsiString() which is the name that is display to user
	}

	void GUISystem::canvasClickHandler() {
		sf::Vector2i position = sf::Mouse::getPosition();
        cout << canvas->GetAbsolutePosition().x << " " << canvas->GetAbsolutePosition().y << endl;
		if (currentBrush->GetText().toAnsiString() == "Create") {
            ex::Entity entity = EntityLibrary::Create::Entity("Spawned Tracker");
            cmn::game->events.emit<XMLLogEntityEvent>(entity);
            addItemToAssetList<WidgetLibrary::SceneHierarchyPanel>(
                sceneHierarchyBox, entity.component<Data>()->name, formatSceneHierarchyListItem);
			entity.assign<Tracker>();
			entity.assign<BoxCollider>()->collisionSettings.insert(COLLISION_LAYER_SETTINGS_SOLID);
            auto transform = entity.component<Transform>();
            transform->transform.x = (float)position.x - canvas->GetAbsolutePosition().x;
            transform->transform.y = (float)position.y - canvas->GetAbsolutePosition().y;
            cout << "Created entity @ (" + std::to_string(transform->transform.x) + ", " + std::to_string(transform->transform.y) + ")" << endl;
			ex::ComponentHandle<rvn::Renderer> renderer = entity.assign<rvn::Renderer>();
            renderer->sprites["BlueDot"] = std::shared_ptr<RenderableSprite>(new RenderableSprite(*cmn::game->getAssets()->sprites->at("PlayerSprite")));
		}
	}

#pragma endregion

}

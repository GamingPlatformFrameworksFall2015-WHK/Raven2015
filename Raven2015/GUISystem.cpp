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

using namespace sfg;

namespace Raven {

    const std::string GUISystem::MAIN_WINDOW_NAME = "Raven";

    // Perform initializations of what we CAN
    GUISystem::GUISystem(std::shared_ptr<InputSystem> inputSystem) :
        mainWindow(new sf::RenderWindow(sf::VideoMode::getDesktopMode(), MAIN_WINDOW_NAME)),
            /*sf::VideoMode((unsigned int)cmn::WINDOW_WIDTH, (unsigned int)cmn::WINDOW_HEIGHT),
            MAIN_WINDOW_NAME, sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize)),*/
        sfgui(new SFGUI()),
        desktop(new Desktop()),
        event(new sf::Event()),
        input(inputSystem) {

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
        // Scene Hierarchy(SH), Content(C), Entity Designer (ED), Prefab List(PL), Canvas(V), Toolbar(T)
        //    0  1  2  3  4  5  6  7  8
        // 0|SH|SH| V| V| V| V| V|PL|PL|
        // 1|SH|SH| V| V| V| V| V|PL|PL|
        // 2|SH|SH| V| V| V| V| V|PL|PL|
        // 3|SH|SH| V| V| V| V| V|PL|PL|
        // 4|SH|SH| V| V| V| V| V|PL|PL|
        // 5|SH|SH| T| T| T| T| T|PL|PL|
        // 6| C| C| C| C|ED|ED|ED|ED|ED|
        // 7| C| C| C| C|ED|ED|ED|ED|ED|
        // 8| C| C| C| C|ED|ED|ED|ED|ED|
        canvas = formatCanvas(Canvas::Create("Canvas"));
        sceneHierarchy = formatSceneHierarchy(ScrolledWindow::Create());
        content = formatContent(Notebook::Create());
        toolbar = formatToolbar(Box::Create(Box::Orientation::VERTICAL));
        entityDesigner = formatEntityDesigner(ScrolledWindow::Create());
        prefabList = formatPrefabList(ScrolledWindow::Create());
        
        // Add all of the various windows to the table, assigning dimensions and settings to the table
        Table::AttachOption all = (Table::AttachOption) (Table::FILL | Table::EXPAND);
        table->Attach(sceneHierarchy,   sf::Rect<sf::Uint32>(0, 0, 2, 6), all, all);
        table->Attach(canvas,           sf::Rect<sf::Uint32>(2, 0, 4, 5));
        table->Attach(prefabList,       sf::Rect<sf::Uint32>(6, 0, 3, 6));
        table->Attach(toolbar,          sf::Rect<sf::Uint32>(2, 5, 4, 1));
        table->Attach(content,          sf::Rect<sf::Uint32>(0, 6, 4, 3));
        table->Attach(entityDesigner,   sf::Rect<sf::Uint32>(4, 6, 5, 3));

        // Add the filled table to the mainGUIWindow
        mainGUIWindow->Add(table);
        desktop->Add(mainGUIWindow);
    }

    void GUISystem::pollEvents() {
        while (mainWindow->pollEvent(*event)) {
            desktop->HandleEvent(*event);
            if (event->type == sf::Event::Closed) {
                mainWindow.get()->close();
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

        sceneHierarchyBox = WidgetLibrary::WidgetList<WidgetLibrary::SceneHierarchyPanel, ENTITY_LIST_LIST_ITEM_TEMPLATE>::Create();

        auto formatListItem = [](Box::Ptr aBox) {
            // pointers retrieved from any GetChildren() operation MUST be acquired as, casted as, and used as RAW pointers.
            // Using shared pointers with addresses retrieved from GetChildren will result in system crashes!
            Entry* e = (Entry*) aBox->GetChildren()[0].get();
            e->SetRequisition(sf::Vector2f(160.f, 20.f));
            Button* b1 = (Button*)aBox->GetChildren()[1].get();
            b1->SetLabel("Select"); // For selecting the Entity
            Button* b2 = (Button*)aBox->GetChildren()[2].get();
            b2->SetLabel("X");      // For deleting the Entity
            Button* b3 = (Button*)aBox->GetChildren()[3].get();
            b3->SetLabel("+");      // For moving the Entity up in the list
            Button* b4 = (Button*)aBox->GetChildren()[4].get();
            b4->SetLabel("-");      // For moving the Entity down in the list
        };

        // For testing purposes
        // These should be added by "creating" entities with the create brush
        // They will be removed by clicking on the X button next to the entity name
        Box::Ptr b = WidgetLibrary::WidgetList<WidgetLibrary::SceneHierarchyPanel, ENTITY_LIST_LIST_ITEM_TEMPLATE>::appendWidget(
            sceneHierarchyBox, "Entity", formatListItem);
        Box::Ptr c = WidgetLibrary::WidgetList<WidgetLibrary::SceneHierarchyPanel, ENTITY_LIST_LIST_ITEM_TEMPLATE>::appendWidget(
            sceneHierarchyBox, "Entity", formatListItem);
        Box::Ptr d = WidgetLibrary::WidgetList<WidgetLibrary::SceneHierarchyPanel, ENTITY_LIST_LIST_ITEM_TEMPLATE>::appendWidget(
            sceneHierarchyBox, "Entity", formatListItem);
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

    // Format the Component List widget
    COMPONENT_LIST_WTYPE_SPTR GUISystem::formatComponentList(COMPONENT_LIST_WTYPE_SPTR cl) {
        cl->SetScrollbarPolicy(ScrolledWindow::HORIZONTAL_AUTOMATIC | ScrolledWindow::VERTICAL_AUTOMATIC);

        //cl->AddWithViewport(ButtonList::Create());

        return cl;
    }
    
    // Format the Texture List widget
    TEXTURE_LIST_WTYPE_SPTR GUISystem::formatTextureList(TEXTURE_LIST_WTYPE_SPTR tl) {
        tl->SetScrollbarPolicy(ScrolledWindow::HORIZONTAL_AUTOMATIC | ScrolledWindow::VERTICAL_AUTOMATIC);

        //tl->AddWithViewport(ButtonList::Create());
        
        return tl;
    }

    // Format the Music List widget
    MUSIC_LIST_WTYPE_SPTR GUISystem::formatMusicList(MUSIC_LIST_WTYPE_SPTR ml) {
        ml->SetScrollbarPolicy(ScrolledWindow::HORIZONTAL_AUTOMATIC | ScrolledWindow::VERTICAL_AUTOMATIC);

        //ml->AddWithViewport(ButtonList::Create());
        
        return ml;
    }

    // Format the Sound List widget
    SOUND_LIST_WTYPE_SPTR GUISystem::formatSoundList(SOUND_LIST_WTYPE_SPTR sl) {
        sl->SetScrollbarPolicy(ScrolledWindow::HORIZONTAL_AUTOMATIC | ScrolledWindow::VERTICAL_AUTOMATIC);
        
        //sl->AddWithViewport(ButtonList::Create());
        
        return sl;
    }

    // Format the Font List widget
    FONT_LIST_WTYPE_SPTR GUISystem::formatFontList(FONT_LIST_WTYPE_SPTR fl) {
        fl->SetScrollbarPolicy(ScrolledWindow::HORIZONTAL_AUTOMATIC | ScrolledWindow::VERTICAL_AUTOMATIC);

        //fl->AddWithViewport(ButtonList::Create());
        
        return fl;
    }

    // Format the Content widget
    CONTENT_WTYPE_SPTR GUISystem::formatContent(CONTENT_WTYPE_SPTR c) {
        componentList = formatComponentList(ScrolledWindow::Create());
        textureList = formatTextureList(ScrolledWindow::Create());
        musicList = formatMusicList(ScrolledWindow::Create());
        soundList = formatSoundList(ScrolledWindow::Create());
        fontList = formatFontList(ScrolledWindow::Create());
        //auto ll = formatLevelList(makeWidget<LEVEL_LIST_WTYPE>(TO_STR(LEVEL_LIST_WTYPE), LEVEL_LIST_NAME));
        c->AppendPage(componentList, Label::Create(COMPONENT_LIST_NAME));
        c->AppendPage(textureList, Label::Create(TEXTURE_LIST_NAME));
        c->AppendPage(musicList, Label::Create(MUSIC_LIST_NAME));
        c->AppendPage(soundList, Label::Create(SOUND_LIST_NAME));
        c->AppendPage(fontList, Label::Create(FONT_LIST_NAME));
        //c->AppendPage(ll, Label::Create(LEVEL_LIST_NAME));

        return c;
    }

    // Format the Entity Designer widget
    ENTITY_DESIGNER_WTYPE_SPTR GUISystem::formatEntityDesigner(ENTITY_DESIGNER_WTYPE_SPTR ed) {
        ed->SetScrollbarPolicy(ScrolledWindow::HORIZONTAL_AUTOMATIC | ScrolledWindow::VERTICAL_AUTOMATIC);

        return ed;
    }

    // Format the Prefab List widget
    PREFAB_LIST_WTYPE_SPTR GUISystem::formatPrefabList(PREFAB_LIST_WTYPE_SPTR pl) {
        pl->SetScrollbarPolicy(ScrolledWindow::HORIZONTAL_AUTOMATIC | ScrolledWindow::VERTICAL_AUTOMATIC);

        // For some vertical padding at the top
        Box::Ptr top = Box::Create(Box::Orientation::VERTICAL, 5.f);
        top->SetRequisition(sf::Vector2f(400.f,0.f));
        Label::Ptr l = Label::Create("Prefab List\n");
        top->Pack(l, true, true);

        prefabListBox = WidgetLibrary::WidgetList<WidgetLibrary::PrefabListPanel, ENTITY_LIST_LIST_ITEM_TEMPLATE>::Create();

        auto formatListItem = [](Box::Ptr aBox) {
            // pointers retrieved from any GetChildren() operation MUST be acquired as, casted as, and used as RAW pointers.
            // Using shared pointers with addresses retrieved from GetChildren will result in system crashes!
            Entry* e = (Entry*) aBox->GetChildren()[0].get();
            e->SetRequisition(sf::Vector2f(160.f, 20.f));
            Button* b1 = (Button*)aBox->GetChildren()[1].get();
            b1->SetLabel("Select"); // For selecting the Entity
            Button* b2 = (Button*)aBox->GetChildren()[2].get();
            b2->SetLabel("X");      // For deleting the Entity
            Button* b3 = (Button*)aBox->GetChildren()[3].get();
            b3->SetLabel("+");      // For moving the Entity up in the list
            Button* b4 = (Button*)aBox->GetChildren()[4].get();
            b4->SetLabel("-");      // For moving the Entity down in the list
        };

        //cmn::game->events.emit<GUIPopulatePrefabList>(prefabListBox);

        // For testing purposes
        // These should be added by "creating" entities with the create brush
        // They will be removed by clicking on the X button next to the entity name
        Box::Ptr b = WidgetLibrary::WidgetList<WidgetLibrary::PrefabListPanel, ENTITY_LIST_LIST_ITEM_TEMPLATE>::appendWidget(prefabListBox, "Entity", formatListItem);
        Box::Ptr c = WidgetLibrary::WidgetList<WidgetLibrary::PrefabListPanel, ENTITY_LIST_LIST_ITEM_TEMPLATE>::appendWidget(prefabListBox, "Entity", formatListItem);
        Box::Ptr d = WidgetLibrary::WidgetList<WidgetLibrary::PrefabListPanel, ENTITY_LIST_LIST_ITEM_TEMPLATE>::appendWidget(prefabListBox, "Entity", formatListItem);
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
        sf::Vector2i position = sf::Mouse::getPosition(*mainWindow.get());
        if (currentBrush->GetText().toAnsiString() == "Create") {
            ex::Entity entity = EntityLibrary::Create::Entity("Spawned Tracker");
            entity.assign<Tracker>();
            entity.assign<BoxCollider>()->collisionSettings.insert(COLLISION_LAYER_SETTINGS_SOLID);
            auto transform = entity.component<Transform>();
			transform->transform.x = (float)position.x - canvas->GetAbsolutePosition().x;
            transform->transform.y = (float)position.y - canvas->GetAbsolutePosition().y;
            cout << "Created entity at " << transform->transform.x << " " << transform->transform.y << endl;
            ex::ComponentHandle<rvn::Renderer> renderer = entity.assign<rvn::Renderer>();
            renderer->sprites["BlueDot"].reset(new RenderableSprite(
                "Resources/Textures/BlueDot_vibrating.png", "BlueDotIdle", 0, 0.f, 0.f, cmn::ERenderingLayer::Foreground, 0));
        }
    }

#pragma endregion

}

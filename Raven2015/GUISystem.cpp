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

using namespace sfg;

namespace Raven {

    const std::string GUISystem::MAIN_WINDOW_NAME = "Raven";

    // Perform initializations of what we CAN
    GUISystem::GUISystem(std::shared_ptr<InputSystem> inputSystem) :
        mainWindow(new sf::RenderWindow(
            sf::VideoMode((unsigned int)cmn::WINDOW_WIDTH, (unsigned int)cmn::WINDOW_HEIGHT),
            MAIN_WINDOW_NAME, sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize)),
        sfgui(new SFGUI()),
        desktop(new Desktop()),
        event(new sf::Event()),
        input(inputSystem) {

        mainWindow->resetGLStates(); // Without this, items will not be rendered propetly immediately
        mainWindow->setPosition(sf::Vector2i(cmn::WINDOW_XPOS, cmn::WINDOW_YPOS));

        // Create the mainGUIWindow
        mainGUIWindow = makeWidget<MASTER_WINDOW_WTYPE>(TO_STR(MASTER_WINDOW_WTYPE), MASTER_WINDOW_NAME);
        mainGUIWindow->SetStyle(MASTER_WINDOW_WTYPE::Style::BACKGROUND | MASTER_WINDOW_WTYPE::Style::RESIZE);
        mainGUIWindow->SetTitle(MASTER_WINDOW_NAME);
        mainGUIWindow->SetRequisition(sf::Vector2f(cmn::WINDOW_WIDTH,
            cmn::WINDOW_HEIGHT));

        // Create the base Table
        auto table = makeWidget<MASTER_TABLE_WTYPE>(TO_STR(MASTER_TABLE_WTYPE), MASTER_TABLE_NAME);

        // Create the various windows that will go inside the table and allocate regions of space for them
        // Implement a 6x5 table with the following structure
        // Scene Hierarchy(SH), Content(C), Entity Designer (ED), Prefab List(PL), Canvas(V), Toolbar(T)
        //    0  1  2  3  4
        // 0|SH| V| V| V|PL|
        // 1|SH| V| V| V|PL|
        // 2|SH| V| V| V|PL|
        // 3|SH| V| V| V|PL|
        // 4|SH| T| T| T|PL|
        // 5| C| C|ED|ED|ED|
        // 6| C| C|ED|ED|ED|
        auto canvas = formatCanvas(makeWidget<CANVAS_WTYPE>(TO_STR(CANVAS_WTYPE), CANVAS_NAME));
        auto sceneHierarchy = formatSceneHierarchy(makeWidget<SCENE_HIERARCHY_WTYPE>(TO_STR(SCENE_HIERARCHY_WTYPE), SCENE_HIERARCHY_NAME));
        auto content = formatContent(makeWidget<CONTENT_WTYPE>(TO_STR(CONTENT_WTYPE), CONTENT_NAME));
        auto toolbar = formatToolbar(makeWidget<TOOLBAR_WTYPE>(TO_STR(TOOLBAR_WTYPE), TOOLBAR_NAME));
        auto entityDesigner = formatEntityDesigner(makeWidget<ENTITY_DESIGNER_WTYPE>(TO_STR(ENTITY_DESIGNER_WTYPE), ENTITY_DESIGNER_NAME));
        auto prefabList = formatPrefabList(makeWidget<PREFAB_LIST_WTYPE>(TO_STR(PREFAB_LIST_WTYPE), PREFAB_LIST_NAME));
        
        // Add all of the various windows to the table, assigning dimensions and settings to the table
        Table::AttachOption all = (Table::AttachOption) (Table::FILL | Table::EXPAND);
        table->Attach(sceneHierarchy, sf::Rect<sf::Uint32>(0, 0, 1, 5), all, all);
        table->Attach(canvas, sf::Rect<sf::Uint32>(1, 0, 3, 4), all, all);
        table->Attach(prefabList, sf::Rect<sf::Uint32>(4, 0, 1, 5), all, all);
        table->Attach(toolbar, sf::Rect<sf::Uint32>(1, 4, 3, 1), all, all);
        table->Attach(content, sf::Rect<sf::Uint32>(0, 5, 2, 2), all, all);
        table->Attach(entityDesigner, sf::Rect<sf::Uint32>(2, 5, 3, 2), all, all);
        // Add the filled table to the mainGUIWindow
        mainGUIWindow->Add(table);
    }

    template <class T>
    std::shared_ptr<T> GUISystem::makeWidget(std::string widgetType, std::string widgetName) {
        // Create an instance <- works because all Widgets have a static Create() factory method
        std::shared_ptr<T> widget(T::Create());
        // Keep a record of this exact window
        widgetMap.insert(std::make_pair(widgetName, widget));
        // Add it to the desktop so that it will be updated
        desktop->Add(widget); 
        // Return the factory-generated widget pointer
        return widget;
    }

    template <class T>
    std::shared_ptr<T> GUISystem::makeWidget() {
        // Create an instance <- works because all Widgets have a static Create() factory method
        std::shared_ptr<T> widget(T::Create());
        // Add it to the desktop so that it will be updated
        desktop->Add(widget); 
        // Return the factory-generated widget pointer
        return widget;
    }

    void GUISystem::pollEvents() {
        while (mainWindow->pollEvent(*event)) {
            desktop->HandleEvent(*event);
            if (event->type == sf::Event::Closed) {
                //desktop->Remove(getCanvas());
                mainWindow->close();
            }
            //-----------------------------------------------
            // Call a function that encapsulates the event.type switch statement and simply returns the corresponding action.
            // Use the value returned by this function to then call another function that will emit an event that triggers the
            // corresponding action on all entities listening for that action via a Pawn component.
            //-----------------------------------------------
            //if(widgetMap["Canvas"]->OnMouseLeftPress)
            //cout << widgetMap["Canvas"]->GetAbsolutePosition().x << " " << widgetMap["Canvas"]->GetAbsolutePosition().y << endl;
            input->handleEvent(*event);
        }
    }

    void GUISystem::update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) {
        // Update all GUI widgets associated with the set of GUI windows in all Desktops
        desktop->Update((float)dt);
    }

    // Format the Scene Hierarchy widget
    SCENE_HIERARCHY_WTYPE_SPTR GUISystem::formatSceneHierarchy(SCENE_HIERARCHY_WTYPE_SPTR sh) {
        sh->SetScrollbarPolicy(ScrolledWindow::HORIZONTAL_AUTOMATIC | ScrolledWindow::VERTICAL_AUTOMATIC);

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

        sh->AddWithViewport(vbox);

        /*auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
        auto button1 = sfg::Button::Create();
        auto button2 = sfg::Button::Create();
        auto label = sfg::Label::Create();
        button1->SetLabel("Foo");
        button2->SetLabel("Bar");
        label->SetText("Baz");
        box->Pack(button1);
        box->Pack(label);
        box->Pack(button2);
        box->SetSpacing(0.5f);
        sh->AddWithViewport(box);*/

        return sh;
    }

    // Format the Component List widget
    COMPONENT_LIST_WTYPE_SPTR GUISystem::formatComponentList(COMPONENT_LIST_WTYPE_SPTR cl) {
        cl->SetScrollbarPolicy(ScrolledWindow::HORIZONTAL_AUTOMATIC | ScrolledWindow::VERTICAL_AUTOMATIC);

        cl->AddWithViewport(WidgetList<Entry>::Create());

        return cl;
    }
    
    // Format the Texture List widget
    TEXTURE_LIST_WTYPE_SPTR GUISystem::formatTextureList(TEXTURE_LIST_WTYPE_SPTR tl) {
        tl->SetScrollbarPolicy(ScrolledWindow::HORIZONTAL_AUTOMATIC | ScrolledWindow::VERTICAL_AUTOMATIC);

        tl->AddWithViewport(WidgetList<Entry>::Create());
        
        return tl;
    }

    // Format the Music List widget
    MUSIC_LIST_WTYPE_SPTR GUISystem::formatMusicList(MUSIC_LIST_WTYPE_SPTR ml) {
        ml->SetScrollbarPolicy(ScrolledWindow::HORIZONTAL_AUTOMATIC | ScrolledWindow::VERTICAL_AUTOMATIC);

        ml->AddWithViewport(WidgetList<Entry>::Create());
        
        return ml;
    }

    // Format the Sound List widget
    SOUND_LIST_WTYPE_SPTR GUISystem::formatSoundList(SOUND_LIST_WTYPE_SPTR sl) {
        sl->SetScrollbarPolicy(ScrolledWindow::HORIZONTAL_AUTOMATIC | ScrolledWindow::VERTICAL_AUTOMATIC);

        sl->AddWithViewport(WidgetList<Entry>::Create());
        
        return sl;
    }

    // Format the Font List widget
    FONT_LIST_WTYPE_SPTR GUISystem::formatFontList(FONT_LIST_WTYPE_SPTR fl) {
        fl->SetScrollbarPolicy(ScrolledWindow::HORIZONTAL_AUTOMATIC | ScrolledWindow::VERTICAL_AUTOMATIC);

        auto entryList = WidgetList<Entry>::Create();
        auto table = Table::Create();
        Table::AttachOption all = (Table::AttachOption) (Table::FILL | Table::EXPAND);
        table->Attach(entryList, sf::Rect<sf::Uint32>(0, 0, 1, 5), all, all);

        fl->AddWithViewport(entryList);
        WidgetList<Entry>::appendWidget(entryList, "Label 1");
        
        return fl;
    }

    // Format the Content widget
    CONTENT_WTYPE_SPTR GUISystem::formatContent(CONTENT_WTYPE_SPTR c) {
        auto cl = formatComponentList(makeWidget<COMPONENT_LIST_WTYPE>(TO_STR(COMPONENT_LIST_WTYPE), COMPONENT_LIST_NAME));
        auto tl = formatTextureList(makeWidget<TEXTURE_LIST_WTYPE>(TO_STR(TEXTURE_LIST_WTYPE), TEXTURE_LIST_NAME));
        auto ml = formatMusicList(makeWidget<MUSIC_LIST_WTYPE>(TO_STR(MUSIC_LIST_WTYPE), MUSIC_LIST_NAME));
        auto sl = formatSoundList(makeWidget<SOUND_LIST_WTYPE>(TO_STR(SOUND_LIST_WTYPE), SOUND_LIST_NAME));
        auto fl = formatFontList(makeWidget<FONT_LIST_WTYPE>(TO_STR(FONT_LIST_WTYPE), FONT_LIST_NAME));
        //auto ll = formatLevelList(makeWidget<LEVEL_LIST_WTYPE>(TO_STR(LEVEL_LIST_WTYPE), LEVEL_LIST_NAME));
        c->AppendPage(cl, Label::Create(COMPONENT_LIST_NAME));
        c->AppendPage(tl, Label::Create(TEXTURE_LIST_NAME));
        c->AppendPage(ml, Label::Create(MUSIC_LIST_NAME));
        c->AppendPage(sl, Label::Create(SOUND_LIST_NAME));
        c->AppendPage(fl, Label::Create(FONT_LIST_NAME));
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

        pl->AddWithViewport(WidgetList<Button>::Create());

        return pl;
    }
    
    // Format the Canvas widget
    CANVAS_WTYPE_SPTR GUISystem::formatCanvas(CANVAS_WTYPE_SPTR c) {
        c->GetSignal(Widget::OnLeftClick).Connect(std::bind(&GUISystem::canvasClickHandler, this));
        return c;
    }

    // Format the Toolbar widget
    TOOLBAR_WTYPE_SPTR GUISystem::formatToolbar(TOOLBAR_WTYPE_SPTR t) {

        t = Box::Create(Box::Orientation::VERTICAL);
        auto brushList = Box::Create(Box::Orientation::HORIZONTAL);
        // Create the various Brush modes we will enter into
        auto createBrush = Button::Create("Create");
        createBrush->GetSignal(Widget::OnLeftClick).Connect(std::bind(&GUISystem::brushToolbarButtonClick, this, createBrush));
        auto deleteBrush = Button::Create("Delete");
        deleteBrush->GetSignal(Widget::OnLeftClick).Connect(std::bind(&GUISystem::brushToolbarButtonClick, this, deleteBrush));
        auto moveBrush = Button::Create("Move");
        moveBrush->GetSignal(Widget::OnLeftClick).Connect(std::bind(&GUISystem::brushToolbarButtonClick, this, moveBrush));
        auto toggleGridTracking = CheckButton::Create("Grid Tracking Enabled");
        // Add those brushes to our list of brushes
        brushList->Pack(createBrush, true, true);
        brushList->Pack(deleteBrush, true, true);
        brushList->Pack(moveBrush, true, true);
        brushList->Pack(Label::Create("   "), true, true); // For the next toggle's padding
        brushList->Pack(toggleGridTracking, true, true);
        t->Pack(currentBrush);
        t->Pack(brushList);

        return t;
    }

    void GUISystem::brushToolbarButtonClick(sfg::Button::Ptr clickedButton) {
        cout << clickedButton->GetLabel().toAnsiString() << " Button Clicked" << endl;
        currentBrush->SetText(clickedButton->GetLabel().toAnsiString());
    }

    void GUISystem::canvasClickHandler() {
        sf::Vector2i position = sf::Mouse::getPosition();
        cout << currentBrush->GetText().toAnsiString() << " at " << position.x << " " << position.y << endl;
    }

    void GUISystem::sceneHierachyButton(sfg::Button::Ptr clickedButton) {
        cout << clickedButton->GetLabel().toAnsiString() << " Button Clicked" << endl;
        //change things in the entity designer and display things according to the
        //clickedButton->GetLabel().toAnsiString() which is the name that is display to user
    }
}

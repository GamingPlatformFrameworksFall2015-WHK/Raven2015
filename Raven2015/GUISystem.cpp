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

namespace Raven {

    const std::string GUISystem::MAIN_WINDOW_NAME = "Raven";

    // Perform initializations of what we CAN
    GUISystem::GUISystem(std::shared_ptr<InputSystem> inputSystem) :
        mainWindow(new sf::RenderWindow(
            sf::VideoMode((unsigned int)cmn::WINDOW_WIDTH, (unsigned int)cmn::WINDOW_HEIGHT),
            MAIN_WINDOW_NAME)),
        sfgui(new sfg::SFGUI()),
        desktop(new sfg::Desktop()),
        event(new sf::Event()),
        input(inputSystem) {

        mainWindow->resetGLStates(); // Without this, items will not be rendered propetly immediately
        mainWindow->setPosition(sf::Vector2i(cmn::WINDOW_XPOS, cmn::WINDOW_YPOS));

        // Create the mainGUIWindow
        mainGUIWindow = makeWidget<sfg::Window>(cmn::GUIWidgetNames::MASTER_WINDOW);
        //mainGUIWindow->SetPosition(sf::Vector2f(cmn::WINDOW_XPOS-20, cmn::WINDOW_YPOS-20));
        mainGUIWindow->SetStyle(sfg::Window::Style::BACKGROUND | sfg::Window::Style::RESIZE);
        mainGUIWindow->SetTitle(cmn::GUIWidgetNames::MASTER_WINDOW);
        mainGUIWindow->SetRequisition(sf::Vector2f(cmn::WINDOW_WIDTH,
            cmn::WINDOW_HEIGHT));

        // Create the base Table
        auto table = makeWidget<sfg::Table>(cmn::GUIWidgetNames::MASTER_TABLE);

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
        auto Canvas = formatCanvas(makeWidget<sfg::Canvas>(cmn::GUIWidgetNames::CANVAS));
        auto sceneHierarchy = formatSceneHierarchy(makeWidget<sfg::ScrolledWindow>(cmn::GUIWidgetNames::SCENE_HIERARCHY));
        auto content = formatContent(makeWidget<sfg::Notebook>(cmn::GUIWidgetNames::CONTENT));
        auto toolbar = formatToolbar(makeWidget<sfg::Box>(cmn::GUIWidgetNames::TOOLBAR));
        auto entityDesigner = formatEntityDesigner(makeWidget<sfg::ScrolledWindow>(cmn::GUIWidgetNames::ENTITY_DESIGNER));
        auto prefabList = formatPrefabList(makeWidget<sfg::ScrolledWindow>(cmn::GUIWidgetNames::PREFAB_LIST));
        
        // Add all of the various windows to the table, assigning dimensions and settings to the table
        sfg::Table::AttachOption all = (sfg::Table::AttachOption) (sfg::Table::FILL | sfg::Table::EXPAND);
        table->Attach(sceneHierarchy, sf::Rect<sf::Uint32>(0, 0, 1, 5), all, all);
        table->Attach(Canvas, sf::Rect<sf::Uint32>(1, 0, 3, 4), all, all);
        table->Attach(prefabList, sf::Rect<sf::Uint32>(4, 0, 1, 5), all, all);
        table->Attach(toolbar, sf::Rect<sf::Uint32>(1, 4, 3, 1), all, all);
        table->Attach(content, sf::Rect<sf::Uint32>(0, 5, 2, 2), all, all);
        table->Attach(entityDesigner, sf::Rect<sf::Uint32>(2, 5, 3, 2), all, all);
        // Add the filled table to the mainGUIWindow
        mainGUIWindow->Add(table);
        //mainGUIWindow->
    }

    template <class T>
    std::shared_ptr<T> GUISystem::makeWidget(std::string widgetName) {
        // Create an instance <- works because all Widgets have a static Create() factory method
        std::shared_ptr<T> widget(T::Create());
        // Keep a record of this exact window
        WidgetMap.insert(std::make_pair(widgetName, widget));
        // Add it to the desktop so that it will be updated
        desktop->Add(widget); 
        // Return the factory-generated widget pointer
        return widget;
    }

    void GUISystem::pollEvents() {
        while (mainWindow->pollEvent(*event)) {
            desktop->HandleEvent(*event);

            //-----------------------------------------------
            // Call a function that encapsulates the event.type switch statement and simply returns the corresponding action.
            // Use the value returned by this function to then call another function that will emit an event that triggers the
            // corresponding action on all entities listening for that action via a Pawn component.
            //-----------------------------------------------

        }
    }

    void GUISystem::update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) {
        // Update all GUI widgets associated with the set of GUI windows in all Desktops
        desktop->Update((float)dt);
    }

    // Format the Scene Hierarchy widget
    std::shared_ptr<sfg::ScrolledWindow> GUISystem::formatSceneHierarchy(std::shared_ptr<sfg::ScrolledWindow> sh) {
        sh->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC);

        sh->AddWithViewport(ButtonList::Create());

        return sh;
    }

    // Format the Component List widget
    std::shared_ptr<sfg::ScrolledWindow> GUISystem::formatComponentList(std::shared_ptr<sfg::ScrolledWindow> cl) {
        cl->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC);

        cl->AddWithViewport(ButtonList::Create());

        return cl;
    }
    
    // Format the Texture List widget
    std::shared_ptr<sfg::ScrolledWindow> GUISystem::formatTextureList(std::shared_ptr<sfg::ScrolledWindow> tl) {
        tl->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC);

        tl->AddWithViewport(ButtonList::Create());
        
        return tl;
    }

    // Format the Music List widget
    std::shared_ptr<sfg::ScrolledWindow> GUISystem::formatMusicList(std::shared_ptr<sfg::ScrolledWindow> ml) {
        ml->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC);

        ml->AddWithViewport(ButtonList::Create());
        
        return ml;
    }

    // Format the Sound List widget
    std::shared_ptr<sfg::ScrolledWindow> GUISystem::formatSoundList(std::shared_ptr<sfg::ScrolledWindow> sl) {
        sl->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC);

        sl->AddWithViewport(ButtonList::Create());
        
        return sl;
    }

    // Format the Font List widget
    std::shared_ptr<sfg::ScrolledWindow> GUISystem::formatFontList(std::shared_ptr<sfg::ScrolledWindow> fl) {
        fl->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC);

        fl->AddWithViewport(ButtonList::Create());
        
        return fl;
    }

    // Format the Content widget
    std::shared_ptr<sfg::Notebook> GUISystem::formatContent(std::shared_ptr<sfg::Notebook> c) {
        auto cl = formatComponentList(makeWidget<sfg::ScrolledWindow>(cmn::GUIWidgetNames::COMPONENT_LIST));
        auto tl = formatTextureList(makeWidget<sfg::ScrolledWindow>(cmn::GUIWidgetNames::TEXTURE_LIST));
        auto ml = formatMusicList(makeWidget<sfg::ScrolledWindow>(cmn::GUIWidgetNames::MUSIC_LIST));
        auto sl = formatSoundList(makeWidget<sfg::ScrolledWindow>(cmn::GUIWidgetNames::SOUND_LIST));
        auto fl = formatFontList(makeWidget<sfg::ScrolledWindow>(cmn::GUIWidgetNames::FONT_LIST));
        c->AppendPage(cl, sfg::Label::Create(cmn::GUIWidgetNames::COMPONENT_LIST));
        c->AppendPage(tl, sfg::Label::Create(cmn::GUIWidgetNames::TEXTURE_LIST));
        c->AppendPage(ml, sfg::Label::Create(cmn::GUIWidgetNames::MUSIC_LIST));
        c->AppendPage(sl, sfg::Label::Create(cmn::GUIWidgetNames::SOUND_LIST));
        c->AppendPage(fl, sfg::Label::Create(cmn::GUIWidgetNames::FONT_LIST));

        return c;
    }

    // Format the Entity Designer widget
    std::shared_ptr<sfg::ScrolledWindow> GUISystem::formatEntityDesigner(std::shared_ptr<sfg::ScrolledWindow> ed) {
        ed->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC);

        return ed;
    }

    // Format the Prefab List widget
    std::shared_ptr<sfg::ScrolledWindow> GUISystem::formatPrefabList(std::shared_ptr<sfg::ScrolledWindow> pl) {
        pl->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC);

        return pl;
    }
    
    // Format the Canvas widget
    std::shared_ptr<sfg::Canvas> GUISystem::formatCanvas(std::shared_ptr<sfg::Canvas> v) {

        return v;
    }

    // Format the Toolbar widget
    std::shared_ptr<sfg::Box> GUISystem::formatToolbar(std::shared_ptr<sfg::Box> t) {

        t = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
        auto currentBrush = sfg::Label::Create("Create");
        auto brushList = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
        // Create the various Brush modes we will enter into
        auto createBrush = sfg::Button::Create("Create");
        auto deleteBrush = sfg::Button::Create("Delete");
        auto moveBrush = sfg::Button::Create("Move");
        auto toggleGridTracking = sfg::CheckButton::Create("Grid Tracking Enabled");
        // Add those brushes to our list of brushes
        brushList->Pack(createBrush, true, true);
        brushList->Pack(deleteBrush, true, true);
        brushList->Pack(moveBrush, true, true);
        brushList->Pack(sfg::Label::Create("   "), true, true); // For the next toggle's padding
        brushList->Pack(toggleGridTracking, true, true);
        t->Pack(currentBrush);
        t->Pack(brushList);

        return t;
    }


}

#pragma once
#include "Common.h"
#include "SFGUI/Widgets.hpp"

using namespace sfg;

// Shared pointer to a given type t
#define SPTR(t) std::shared_ptr<t>
// Shared pointer to an Widget
#define WSPTR SPTR(Widget)
// Convert contents into something within a string
#define TO_STR(s) #s
// Statically casts an instance of std::shared_ptr<Widget> wptr to a shared pointer of the given Widget type t
// For example: WIDGET_CAST(vector<std::shared_ptr<Widget>>().front(), Box) provides a shared pointer to an Box
//     IF the first item in the vector is, in fact, an Box.
// Use only when you are absolutely sure of wptr's runtime type
#define WIDGET_CAST(wptr, t) SPTR(t)( (t*) wptr.get() )

//////// Type Macros //////////

///// Widget Types
#define WIDGET_T Widget
#define WINDOW_T Window
#define SCROLLEDWINDOW_T ScrolledWindow
#define NOTEBOOK_T Notebook
#define BOX_T Box
#define TABLE_T Table
#define CANVAS_T Canvas
#define BUTTON_T Button

///// High-Level Widgets

// MasterWindow
#define MASTER_WINDOW_WTYPE WINDOW_T
#define MASTER_WINDOW_WTYPE_SPTR SPTR(WINDOW_T)
#define MASTER_WINDOW_NAME "Master Window"

// MasterTable
#define MASTER_TABLE_WTYPE TABLE_T
#define MASTER_TABLE_WTYPE_SPTR SPTR(TABLE_T)
#define MASTER_TABLE_NAME "Layout Table"

// SceneHierarchy
#define SCENE_HIERARCHY_WTYPE SCROLLEDWINDOW_T
#define SCENE_HIERARCHY_WTYPE_SPTR SPTR(SCENE_HIERARCHY_WTYPE)
#define SCENE_HIERARCHY_NAME "Scene Hierarchy"

// ComponentList
#define COMPONENT_LIST_WTYPE SCROLLEDWINDOW_T
#define COMPONENT_LIST_WTYPE_SPTR SPTR(COMPONENT_LIST_WTYPE)
#define COMPONENT_LIST_NAME "Components"

// TextureList
#define TEXTURE_LIST_WTYPE SCROLLEDWINDOW_T
#define TEXTURE_LIST_WTYPE_SPTR SPTR(TEXTURE_LIST_WTYPE)
#define TEXTURE_LIST_NAME "Textures"

// MusicList
#define MUSIC_LIST_WTYPE SCROLLEDWINDOW_T
#define MUSIC_LIST_WTYPE_SPTR SPTR(MUSIC_LIST_WTYPE)
#define MUSIC_LIST_NAME "Music"

// SoundList
#define SOUND_LIST_WTYPE SCROLLEDWINDOW_T
#define SOUND_LIST_WTYPE_SPTR SPTR(SOUND_LIST_WTYPE)
#define SOUND_LIST_NAME "Sounds"

// FontList
#define FONT_LIST_WTYPE SCROLLEDWINDOW_T
#define FONT_LIST_WTYPE_SPTR SPTR(FONT_LIST_WTYPE)
#define FONT_LIST_NAME "Fonts"

// Content
#define CONTENT_WTYPE NOTEBOOK_T
#define CONTENT_WTYPE_SPTR SPTR(CONTENT_WTYPE)
#define CONTENT_NAME "Content"

// EntityDesigner
#define ENTITY_DESIGNER_WTYPE SCROLLEDWINDOW_T
#define ENTITY_DESIGNER_WTYPE_SPTR SPTR(ENTITY_DESIGNER_WTYPE)
#define ENTITY_DESIGNER_NAME "Entity Designer"

// PrefabList
#define PREFAB_LIST_WTYPE SCROLLEDWINDOW_T
#define PREFAB_LIST_WTYPE_SPTR SPTR(PREFAB_LIST_WTYPE)
#define PREFAB_LIST_NAME "Prefab List"

// Canvas
#define CANVAS_WTYPE CANVAS_T
#define CANVAS_WTYPE_SPTR SPTR(CANVAS_WTYPE)
#define CANVAS_NAME "Canvas"

// Toolbar
#define TOOLBAR_WTYPE BOX_T
#define TOOLBAR_WTYPE_SPTR SPTR(TOOLBAR_WTYPE)
#define TOOLBAR_NAME "Toolbar"

///// Low-Level Widgets

// ButtonList
#define BUTTON_LIST_WTYPE BOX_T
#define BUTTON_LIST_WTYPE_SPTR SPTR(BUTTON_LIST_WTYPE)

/////////////////////

namespace Raven {

    // --------------------------------------------------------------------------------------------
    // To interface with any custom Widget class, you must treat it purely as a utility class.
    // This is because SFGUI manages its own memory regarding Widgets. Instead, you must
    // treat custom Widgets more like how the algorithms of <algorithm> work, taking a container
    // as a parameter rather than operating from a particular container instance.
    // 
    // Each class should have all static functions, no non-static member variables, a 
    // private constructor (so instances cannot be made), and a creation function:
    // static [top-level widget type here] Create() {...}
    // ...where [top-level widget type] is a type deriving from Widget
    //
    // All interactions with the widget pointer created by the Create() function must then involve
    // a pointer to the top-level widget type as a first parameter
    // static void performCustomWidgetOperation([top-level widget type], param1, param2, ...) {...}
    //
    // If operations are performed on the created type without the use of the custom Widget class
    // static functions, the resulting state of the widget from all future operations is undefined.
    // --------------------------------------------------------------------------------------------

    // ButtonList is used to create a vertical box that adds and removes Button widgets dynamically
    //
    // Example:
    // auto buttonList = ButtonList::Create();
    // ButtonList::appendButton(buttonList, "My Button Label");
    // 
    template <typename W>
    class WidgetList {
    public:

        static void insertWidget(SPTR(Box) type, size_t position, std::string labelName) {
            appendWidget(type, labelName);
            type->ReorderChild(type->GetChildren().back(), position);
        }

        static void appendWidget(SPTR(Box) type, std::string labelName) {
            type->PackEnd(makeWidgetBox(labelName), true, true);
        }

        static void prependWidget(SPTR(Box) type, std::string labelName) {
            type->PackStart(makeWidgetBox(labelName), true, true);
        }

        static void removeWidget(SPTR(Box) type, size_t position) {
            type->Remove(type->GetChildren()[position]);
        }

        static SPTR(Box) getWidgetBoxAt(SPTR(Box) type, size_t position) {
            return WIDGET_CAST(type->GetChildren()[position], Box);
        }

        static size_t length(SPTR(Box) type) {
            return type->GetChildren().size();
        }

        static SPTR(Box) Create() {
            auto vbox = Box::Create(Box::Orientation::VERTICAL);
            auto button = Button::Create("+");
            button->GetSignal(Button::OnLeftClick).Connect(std::bind(&addNewButtonClick, vbox, "Button" + std::to_string(counter++)));
            vbox->Pack(button, true, true);

            return vbox;
        }

    private:
        WidgetList();

        static SPTR(Box) makeWidgetBox(std::string labelName) {
            auto type = Box::Create(Box::Orientation::HORIZONTAL);
            auto widget = W::Create(labelName);
            type->Pack(widget, true, false);
            return type;
        }

        static void addNewButtonClick(SPTR(Box) type, std::string name) {
            appendWidget(type, name);
        }

        static unsigned int counter;
    };

}

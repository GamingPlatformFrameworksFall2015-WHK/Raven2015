#pragma once
#include "Common.h"
#include "SFGUI/Widgets.hpp"
#include <algorithm>

using namespace sfg;

namespace Raven {

    struct WidgetLibrary {

        // Shared pointer to a given type t
#define SPTR(t) std::shared_ptr<t>
// Shared pointer to an Widget
#define WSPTR SPTR(Widget)
// Convert contents into something within a string
#define TO_STR(s) #s

//////// Type Macros //////////

///// High-Level Widgets

// MasterWindow
#define MASTER_WINDOW_WTYPE Window 
#define MASTER_WINDOW_WTYPE_SPTR SPTR(WINDOW_T)
#define MASTER_WINDOW_NAME "Master Window"

// MasterTable
#define MASTER_TABLE_WTYPE Table
#define MASTER_TABLE_WTYPE_SPTR SPTR(TABLE_T)
#define MASTER_TABLE_NAME "Layout Table"

// SceneHierarchy
#define SCENE_HIERARCHY_WTYPE ScrolledWindow
class SceneHierarchyPanel {};
#define SCENE_HIERARCHY_WTYPE_SPTR SPTR(SCENE_HIERARCHY_WTYPE)
#define SCENE_HIERARCHY_NAME "Scene Hierarchy"

// ComponentList
#define COMPONENT_LIST_WTYPE ScrolledWindow
class ComponentListPanel {};
#define COMPONENT_LIST_WTYPE_SPTR SPTR(COMPONENT_LIST_WTYPE)
#define COMPONENT_LIST_NAME "Components"

// TextureList
#define TEXTURE_LIST_WTYPE ScrolledWindow
class TextureListPanel {};
#define TEXTURE_LIST_WTYPE_SPTR SPTR(TEXTURE_LIST_WTYPE)
#define TEXTURE_LIST_NAME "Textures"

// MusicList
#define MUSIC_LIST_WTYPE ScrolledWindow
class MusicListPanel {};
#define MUSIC_LIST_WTYPE_SPTR SPTR(MUSIC_LIST_WTYPE)
#define MUSIC_LIST_NAME "Music"

// SoundList
#define SOUND_LIST_WTYPE ScrolledWindow
class SoundListPanel {};
#define SOUND_LIST_WTYPE_SPTR SPTR(SOUND_LIST_WTYPE)
#define SOUND_LIST_NAME "Sounds"

// FontList
#define FONT_LIST_WTYPE ScrolledWindow
class FontListPanel {};
#define FONT_LIST_WTYPE_SPTR SPTR(FONT_LIST_WTYPE)
#define FONT_LIST_NAME "Fonts"

// Content
#define CONTENT_WTYPE Notebook
class ContentPanel {};
#define CONTENT_WTYPE_SPTR SPTR(CONTENT_WTYPE)
#define CONTENT_NAME "Content"

// EntityDesigner
#define ENTITY_DESIGNER_WTYPE ScrolledWindow
class EntityDesignerPanel {};
#define ENTITY_DESIGNER_WTYPE_SPTR SPTR(ENTITY_DESIGNER_WTYPE)
#define ENTITY_DESIGNER_NAME "Entity Designer"

// PrefabList
#define PREFAB_LIST_WTYPE ScrolledWindow
class PrefabListPanel {};
#define PREFAB_LIST_WTYPE_SPTR SPTR(PREFAB_LIST_WTYPE)
#define PREFAB_LIST_NAME "Prefab List"

            // Canvas
#define CANVAS_WTYPE Canvas
class CanvasPanel {};
#define CANVAS_WTYPE_SPTR SPTR(CANVAS_WTYPE)
#define CANVAS_NAME "Canvas"

        // Toolbar
#define TOOLBAR_WTYPE Box
class ToolbarPanel {};
#define TOOLBAR_WTYPE_SPTR SPTR(TOOLBAR_WTYPE)
#define TOOLBAR_NAME "Toolbar"

        ///// Low-Level Widgets

#define ASSET_LIST_WIDGET_SEQUENCE Entry, Button, Button, Button, Button, Button


/////////////////////

        // --------------------------------------------------------------------------------------------
        // To interface with any custom Widget class, you must treat it purely as a utility class.
        // This is because SFGUI manages its own memory regarding Widgets. Instead, you must
        // treat custom Widgets more like how the algorithms of <algorithm> work, taking a container
        // as a parameter rather than operating from a particular container instance.
        // 
        // Each "class" should have all static functions, no non-static member variables, a 
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
        template <typename PanelType, typename... Widgets> // where Widgets is the types of Widgets to be embedded in the List items
        class WidgetList {
        public:
            static Box::Ptr insertWidget(Box::Ptr type, size_t position, std::string labelName, void(*listItemFormatter)(Box::Ptr)) {
                Box::Ptr b = makeWidgetBox<Widgets...>(labelName);
                type->Pack(b, true, true);
                listItemFormatter(b);
                type->ReorderChild(box, position);
                //sortList(type);
                return b;
            }

            static Box::Ptr appendWidget(Box::Ptr type, std::string labelName, void(*listItemFormatter)(Box::Ptr)) {
                Box::Ptr b = makeWidgetBox<Widgets...>(labelName);
                type->Pack(b, true, true);
                listItemFormatter(b);
                //sortList(type);
                return b;
            }

            static Box::Ptr prependWidget(Box::Ptr type, std::string labelName, void(*listItemFormatter)(Box::Ptr)) {
                Box::Ptr b = makeWidgetBox<Widgets...>(labelName);
                type->PackStart(b, true, true);
                listItemFormatter(b);
                //sortList(type);
                return b;
            }

            static void removeWidget(Box::Ptr type, std::string labelName) {
                for (int i = 0; i < type->GetChildren().size(); ++i) {
                    if (labelName == type->GetChildren()[i]->GetName()) {
                        type->Remove(type->GetChildren()[i]);
                        break;
                    }
                }
                //sortList(type);
            }

            static void sortList(Box::Ptr type) {
                std::sort(type->GetChildren().begin(), type->GetChildren().end(), compareWidgets);
            }

            static Box::Ptr getWidgetBox(Box::Ptr type, size_t position) {
                // Not sure this will work since I was getting errors when casting 
                // the return value of GetChildren()[#] to Box::Ptr in a different function.
                return (Box::Ptr) type->GetChildren()[position];
            }

            static size_t length(Box::Ptr type) {
                return type->GetChildren().size();
            }

            static Box::Ptr Create() {
                return Box::Create(Box::Orientation::VERTICAL, 15.f);
            }

        private:
            WidgetList(); //Cannot be instantiated

            static unsigned int counter;

            // Base case for recursive parameter pack widget box creation. Should be called initially with just the labelName
            template <typename W>
            static Box::Ptr makeWidgetBox(std::string labelName, Box::Ptr box = nullptr, std::shared_ptr<std::tuple<W>> t = nullptr) {
                if (!box) {
                    box = Box::Create(Box::Orientation::HORIZONTAL, 5.f);
                }
                auto widget = W::Create(labelName + " " + std::to_string(counter++));
                box->Pack(widget, true, true);
                Box::Ptr finalBox = box; // save the address of the box
                return finalBox;
            }

            // Recursive case for recursive parameter pack widget box creation. Should be called initially with just the labelName
            // Example:
            // Box::Ptr myBoxWithALabelAndAButton = makeWidgetBox<Label, Button>("MyAwesomeBox");
            template <typename W, typename... Widgets>
            static Box::Ptr makeWidgetBox(std::string labelName, Box::Ptr box = nullptr, std::shared_ptr<std::tuple<Widgets...>> t = nullptr) {
                if (!box) {
                    box = Box::Create(Box::Orientation::HORIZONTAL, 5.f);
                }
                auto widget = W::Create(labelName + " " + std::to_string(counter));
                box->Pack(widget, true, true);
                return makeWidgetBox<Widgets...>(labelName, box, nullptr);
            }

            static bool compareWidgets(Box::Ptr first, Box::Ptr second) {
                return first->GetChildren().front()->GetName() < second->GetChildren().front()->GetName() ? true : false;
            }
        };
    };
}

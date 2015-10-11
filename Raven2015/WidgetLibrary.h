#pragma once
#include "Common.h"
#include "SFGUI/Widgets.hpp"

// Shared pointer to a given type t
#define SPTR(t) std::shared_ptr<t>
// Shared pointer to an sfg::Widget
#define WSPTR SPTR(sfg::Widget)
// Statically casts an instance of std::shared_ptr<sfg::Widget> wptr to a shared pointer of the given Widget type t
// For example: WIDGET_CAST(vector<std::shared_ptr<sfg::Widget>>().front(), sfg::Box) provides a shared pointer to an sfg::Box
//     IF the first item in the vector is, in fact, an sfg::Box.
// Use only when you are absolutely sure of wptr's runtime type
#define WIDGET_CAST(wptr, t) SPTR(t)( (t*) wptr.get() )

// Type Macros for ButtonList
#define BUTTON_LIST_WTYPE sfg::Box
#define BUTTON_LIST_WTYPE_SPTR std::shared_ptr<BUTTON_LIST_WTYPE>

namespace Raven {

    // --------------------------------------------------------------------------------------------
    // To interface with any custom Widget class, you must treat it purely as a utility class.
    // This is because SFGUI manages its own memory regarding sfg::Widgets. Instead, you must
    // treat custom Widgets more like how the algorithms of <algorithm> work, taking a container
    // as a parameter rather than operating from a particular container instance.
    // 
    // Each class should have all static functions, no non-static member variables, a 
    // private constructor (so instances cannot be made), and a creation function:
    // static [top-level widget type here] Create() {...}
    // ...where [top-level widget type] is a type deriving from sfg::Widget
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
    class ButtonList {
    public:

        static void insertButton(BUTTON_LIST_WTYPE_SPTR type, size_t position, std::string labelName) {
            appendButton(type, labelName);
            type->ReorderChild(type->GetChildren().back(), position);
        }

        static void appendButton(BUTTON_LIST_WTYPE_SPTR type, std::string labelName) {
            type->PackEnd(makeButtonBox(labelName), true, true);
        }

        static void prependButton(BUTTON_LIST_WTYPE_SPTR type, std::string labelName) {
            type->PackStart(makeButtonBox(labelName), true, true);
        }

        static void removeButton(BUTTON_LIST_WTYPE_SPTR type, size_t position) {
            type->Remove(type->GetChildren()[position]);
        }

        static BUTTON_LIST_WTYPE_SPTR getButtonBox(BUTTON_LIST_WTYPE_SPTR type, size_t position) {
            return WIDGET_CAST(type->GetChildren()[position], BUTTON_LIST_WTYPE);
        }

        static size_t length(BUTTON_LIST_WTYPE_SPTR type) {
            return type->GetChildren().size();
        }

        static BUTTON_LIST_WTYPE_SPTR Create() {
            return BUTTON_LIST_WTYPE::Create(BUTTON_LIST_WTYPE::Orientation::VERTICAL);
        }

    private:
        ButtonList();

        static SPTR(sfg::Box) makeButtonBox(std::string labelName) {
            auto type = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
            auto button = sfg::Button::Create(labelName);
            type->Pack(button, true, false);
            return type;
        }
    };

}

#include "WidgetLibrary.h"


namespace Raven {

    RadioButtonMultiList::RadioButtonMultiList() : vbox(sfg::Box::Create(sfg::Box::Orientation::VERTICAL)) {

    }

    void RadioButtonMultiList::insertButton(unsigned int position, std::string labelName) {
        
    }

    void RadioButtonMultiList::appendButton(std::string labelName) {

    }

    void RadioButtonMultiList::prependButton(std::string labelName) {

    }

    unsigned int RadioButtonMultiList::length() {
        return 0;
    }

    std::shared_ptr<sfg::Box> RadioButtonMultiList::getTopLevelBox() {
        return vbox;
    }

    std::shared_ptr<sfg::Box> RadioButtonMultiList::makeButtonBox(std::string labelName) {
        auto box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
        auto button = sfg::RadioButton::Create(labelName);
        return box;
    }
}

#pragma once
#include "Common.h"
#include "SFGUI/Widgets.hpp"


namespace Raven {

    class RadioButtonMultiList {
    public:
        RadioButtonMultiList();

        void insertButton(unsigned int position, std::string labelName);
        void appendButton(std::string labelName);
        void prependButton(std::string labelName);
        void removeButton(unsigned int position);
        unsigned int length();
        std::shared_ptr<sfg::Box> getTopLevelBox();

    private:
        std::shared_ptr<sfg::Box> makeButtonBox(std::string labelName);

        std::shared_ptr<sfg::Box> vbox;

    };

}

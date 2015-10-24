#include "WidgetLibrary.h"

namespace Raven {

    template <typename... Widgets>
    unsigned int WidgetList<Widgets...>::counter = 0;
}
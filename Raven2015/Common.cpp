#include "Common.h"

namespace Raven {

    namespace Common {

        /*
        const std::string GUIWidgetNames::MASTER_WINDOW = "Editor";
        const std::string GUIWidgetNames::MASTER_TABLE = "Layout Table";
        const std::string GUIWidgetNames::SCENE_HIERARCHY = "Scene Hierarchy";
        const std::string GUIWidgetNames::COMPONENT_LIST = "Components";
        const std::string GUIWidgetNames::TEXTURE_LIST = "Textures";
        const std::string GUIWidgetNames::MUSIC_LIST = "Music";
        const std::string GUIWidgetNames::SOUND_LIST = "Sounds";
        const std::string GUIWidgetNames::FONT_LIST = "Fonts";
        const std::string GUIWidgetNames::CONTENT = "Content";
        const std::string GUIWidgetNames::TOOLBAR = "Toolbar";
        const std::string GUIWidgetNames::ENTITY_DESIGNER = "Entity Designer";
        const std::string GUIWidgetNames::PREFAB_LIST = "Prefab List";
        const std::string GUIWidgetNames::CANVAS = "Canvas";

        const std::string GUIWidgetTypes::WIDGET = "Widget";
        const std::string GUIWidgetTypes::WINDOW = "Window";
        const std::string GUIWidgetTypes::BOX = "Box";
        const std::string GUIWidgetTypes::TABLE = "Table";
        const std::string GUIWidgetTypes::CANVAS = "Canvas";
        const std::string GUIWidgetTypes::BUTTON = "Button";
		
        const std::string CollisionLayerSettings::SOLID = "Solid";
        const std::string CollisionLayerSettings::TRIGGER = "Trigger";
        */

        const std::set<std::string> CollisionInformation::settings({ COLLISION_LAYER_SETTINGS_SOLID, COLLISION_LAYER_SETTINGS_FIXED });
    }
}

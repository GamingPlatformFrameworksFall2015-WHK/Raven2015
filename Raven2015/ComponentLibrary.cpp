#include "ComponentLibrary.h"

namespace Raven {

#pragma region Data

    std::string Data::serialize(std::string tab) {
        return
            tab + "<Data>\r\n" +
            tab + "  <Name>" + this->name + "</Name>\r\n" +
            tab + "  <PrefabName>" + this->prefabName + "</PrefabName>\r\n" +
            tab + "  <Modified>" + std::to_string(this->modified) + "</Modified>\r\n" +
            tab + "</Data>\r\n";

    }

    void Data::deserialize(XMLNode* node) {
        name = node->FirstChildElement("Name")->GetText();
        prefabName = node->FirstChildElement("PrefabName")->GetText();
        node->FirstChildElement("Modified")->QueryBoolText(&this->modified);
    }

#pragma endregion

#pragma region Physics

    std::string Transform::serialize(std::string tab) {
        return
            tab + "<Transform>\r\n" +
            tab + "  <Transform>\r\n" +
            tab + "    <X>" + std::to_string(this->transform.x) + "</X>\r\n" +
            tab + "    <Y>" + std::to_string(this->transform.y) + "</Y>\r\n" +
            tab + "  </Transform>\r\n" +
            tab + "  <Rotation>" + std::to_string(this->rotation) + "</Rotation>\r\n" +
            tab + "</Transform>\r\n";
    }

    void Transform::deserialize(XMLNode* node) {
        XMLNode* t = node->FirstChild();
        t->FirstChildElement("X")->QueryFloatText(&(this->transform.x));
        t->FirstChildElement("Y")->QueryFloatText(&this->transform.y);
        node->FirstChildElement("Rotation")->QueryFloatText(&this->rotation);
    }

    std::string Rigidbody::serialize(std::string tab) {
        return
            tab + "<Rigidbody>\r\n" +
            tab + "  <Velocity>\r\n" +
            tab + "    <X>" + std::to_string(this->velocity.x) + "</X>\r\n" +
            tab + "    <Y>" + std::to_string(this->velocity.y) + "</Y>\r\n" +
            tab + "  </Velocity>\r\n" +
            tab + "  <Acceleration>\r\n" +
            tab + "    <X>" + std::to_string(this->acceleration.x) + "</X>\r\n" +
            tab + "    <Y>" + std::to_string(this->acceleration.y) + "</Y>\r\n" +
            tab + "  </Acceleration>\r\n" +
            tab + "  <RadialVelocity>" + std::to_string(this->radialVelocity) + "</RadialVelocity>\r\n" +
            tab + "</Rigidbody>\r\n";
    }

    void Rigidbody::deserialize(XMLNode* node) {
        XMLNode* t = node->FirstChildElement("Velocity");
        t->FirstChildElement("X")->QueryFloatText(&(this->velocity.x));
        t->FirstChildElement("Y")->QueryFloatText(&this->velocity.y);
        t = node->FirstChildElement("Acceleration");
        t->FirstChildElement("X")->QueryFloatText(&(this->acceleration.x));
        t->FirstChildElement("Y")->QueryFloatText(&this->acceleration.y);
        node->FirstChildElement("RadialVelocity")->QueryFloatText(&this->radialVelocity);
    }

    std::string BoxCollider::serialize(std::string tab) {
        std::string layersContent = "";
        for (std::pair<std::string, std::pair<bool, bool>> layer : layers) {
            layersContent +=
                tab + "    <Layer Name=\"" + layer.first + "\">\r\n" +
                tab + "      <Required>" + std::to_string(layer.second.first) + "</Required>\r\n" +
                tab + "      <Automatic>" + std::to_string(layer.second.second) + "</Automatic>\r\n" +
                tab + "    </Layer>\r\n";
        }
        bool solid = collisionSettings.find("Solid") != collisionSettings.end();
        bool trigger = collisionSettings.find("Trigger") != collisionSettings.end();

        return
            tab + "<BoxCollider>\r\n" +
            tab + "  <Width>" + std::to_string(this->width) + "</Width>\r\n" +
            tab + "  <Height>" + std::to_string(this->height) + "</Height>\r\n" +
            tab + "  <XOffset>" + std::to_string(this->originOffset.x) + "</XOffset>\r\n" +
            tab + "  <YOffset>" + std::to_string(this->originOffset.y) + "</YOffset>\r\n" +
            tab + "  <Layers>\r\n" +
            layersContent +
            tab + "  </Layers>\r\n" +
            tab + "  <Settings>\r\n" +
            tab + "    <Solid>" + std::to_string(solid) + "</Solid>\r\n" +
            tab + "    <Trigger>" + std::to_string(trigger) + "</Trigger>\r\n" +
            tab + "  </Settings>\r\n" +
            tab + "</BoxCollider>\r\n";
    }

    void BoxCollider::deserialize(XMLNode* node) {
        node->FirstChildElement("Width")->QueryFloatText(&this->width);
        node->FirstChildElement("Height")->QueryFloatText(&this->height);
        node->FirstChildElement("XOffset")->QueryFloatText(&this->originOffset.x);
        node->FirstChildElement("YOffset")->QueryFloatText(&this->originOffset.y);

        XMLElement* t = node->FirstChildElement("Layer");
        layers.clear();
        do {
            std::string layerName = t->Attribute("Name");
            bool required, automatic;
            t->FirstChildElement("Required")->QueryBoolText(&required);
            t->FirstChildElement("Automatic")->QueryBoolText(&automatic);
            layers.insert(std::make_pair(layerName, std::make_pair(required, automatic)));
        } while (t = t->NextSiblingElement("Layer"));

        t = node->FirstChildElement("Settings");
        collisionSettings.clear();
        for (std::string setting : cmn::CollisionInformation::settings) {
            bool val;
            t->FirstChildElement(setting.c_str())->QueryBoolText(&val);
            if (val) collisionSettings.insert(setting);
        }
    }

#pragma endregion

#pragma region Audio

    std::string SoundMaker::serialize(std::string tab) {
        return "";
    }

    void SoundMaker::deserialize(XMLNode* node) {

    }

    std::string MusicMaker::serialize(std::string tab) {
        return "";
    }

    void MusicMaker::deserialize(XMLNode* node) {

    }

#pragma endregion

#pragma region Rendering

    std::string Renderer::serialize(std::string tab) {
        return "";
    }

    void Renderer::deserialize(XMLNode* node) {

    }

#pragma endregion

#pragma region Timers

    std::string TimeTable::serialize(std::string tab) {
        return "";
    }

    void TimeTable::deserialize(XMLNode* node) {

    }

#pragma endregion

#pragma region Behaviors


    std::string CoreBehavior::serialize(std::string tab) {
        return "";
    }

    void CoreBehavior::deserialize(XMLNode* node) {

    }

#pragma endregion

#pragma region ActionListener

    std::string ActionListener::serialize(std::string tab) {
        return "";
    }

    void ActionListener::deserialize(XMLNode* node) {

    }


#pragma endregion

    const std::string ComponentLibrary::NULL_COMPONENT = "NULL_COMPONENT";
    const std::string ComponentLibrary::DATA = "Data";
    const std::string ComponentLibrary::TRANSFORM = "Transform";
    const std::string ComponentLibrary::RIGIDBODY = "Rigidbody";
    const std::string ComponentLibrary::BOX_COLLIDER = "BoxCollider";
    const std::string ComponentLibrary::SOUND_MAKER = "SoundMaker";
    const std::string ComponentLibrary::MUSIC_MAKER = "MusicMaker";
    const std::string ComponentLibrary::RENDERER = "Renderer";
    const std::string ComponentLibrary::TIME_TABLE = "TimeTable";
    const std::string ComponentLibrary::CORE_BEHAVIOR = "CoreBehavior";
    const std::string ComponentLibrary::ACTION_LISTENER = "ActionListener";

}
#include "ComponentLibrary.h"

namespace Raven {

#pragma region Data

    std::string Data::serialize(std::string tab, bool forPrefab) {
        return
            tab + "<Data>\r\n" +
            tab + "  <Name>" + this->name + "</Name>\r\n" +
            tab + "  <PrefabName>" + this->prefabName + "</PrefabName>\r\n" +
            tab + "  <Modified>" + std::to_string(this->modified) + "</Modified>\r\n" +
            tab + "</Data>\r\n";

    }

    void Data::deserialize(XMLNode* node, bool forPrefab) {
        name = node->FirstChildElement("Name")->GetText();
        prefabName = node->FirstChildElement("PrefabName")->GetText();
        node->FirstChildElement("Modified")->QueryBoolText(&this->modified);
    }

#pragma endregion

#pragma region Physics

    std::string Transform::serialize(std::string tab, bool forPrefab) {
        return
            tab + "<Transform>\r\n" +
            tab + "  <Transform>\r\n" +
            tab + "    <X>" + std::to_string(this->transform.x) + "</X>\r\n" +
            tab + "    <Y>" + std::to_string(this->transform.y) + "</Y>\r\n" +
            tab + "  </Transform>\r\n" +
            tab + "  <Rotation>" + std::to_string(this->rotation) + "</Rotation>\r\n" +
            tab + "</Transform>\r\n";
    }

    void Transform::deserialize(XMLNode* node, bool forPrefab) {
        XMLNode* t = node->FirstChild();
        t->FirstChildElement("X")->QueryFloatText(&(this->transform.x));
        t->FirstChildElement("Y")->QueryFloatText(&this->transform.y);
        node->FirstChildElement("Rotation")->QueryFloatText(&this->rotation);
    }

    std::string Rigidbody::serialize(std::string tab, bool forPrefab) {
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

    void Rigidbody::deserialize(XMLNode* node, bool forPrefab) {
        XMLNode* t = node->FirstChildElement("Velocity");
        t->FirstChildElement("X")->QueryFloatText(&(this->velocity.x));
        t->FirstChildElement("Y")->QueryFloatText(&this->velocity.y);
        t = node->FirstChildElement("Acceleration");
        t->FirstChildElement("X")->QueryFloatText(&(this->acceleration.x));
        t->FirstChildElement("Y")->QueryFloatText(&this->acceleration.y);
        node->FirstChildElement("RadialVelocity")->QueryFloatText(&this->radialVelocity);
    }

    std::string BoxCollider::serialize(std::string tab, bool forPrefab) {
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

    void BoxCollider::deserialize(XMLNode* node, bool forPrefab) {
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

    std::string SoundMaker::serialize(std::string tab, bool forPrefab) {
        std::string soundContent = "";
        for (auto soundFilePath_soundBuffer : soundMap) {
            soundContent +=
                tab + "  <SoundFilePath>" + soundFilePath_soundBuffer.first + "</SoundFilePath>\r\n";
        }
        return
            tab + "<SoundMaker>\r\n" +
            soundContent +
            tab + "</SoundMaker>\r\n";
    }

    void SoundMaker::deserialize(XMLNode* node, bool forPrefab) {

    }

    std::string MusicMaker::serialize(std::string tab, bool forPrefab) {
        std::string musicContent = "";
        for (auto musicFilePath_musicBuffer : musicMap) {
            musicContent +=
                tab + "  <MusicFilePath>" + musicFilePath_musicBuffer.first + "</MusicFilePath>\r\n";
        }
        return
            tab + "<MusicMaker>\r\n" +
            musicContent +
            tab + "</MusicMaker>\r\n";
    }

    void MusicMaker::deserialize(XMLNode* node, bool forPrefab) {

    }

#pragma endregion

#pragma region Rendering

    std::string Renderer::serialize(std::string tab, bool forPrefab) {
        std::string textContent = "";
        std::string rectangleContent = "";
        std::string circleContent = "";
        std::string spriteContent = "";
        for (auto assetName_renderable : texts) {
            textContent += tab + "    <TextName>" + assetName_renderable.first + "</TextName>\r\n";
        }
        for (auto assetName_renderable : rectangles) {
            rectangleContent += tab + "    <RectangleName>" + assetName_renderable.first + "</RectangleName>\r\n";
        }
        for (auto assetName_renderable : circles) {
            circleContent += tab + "    <CircleName>" + assetName_renderable.first + "</CircleName>\r\n";
        }
        for (auto assetName_renderable : sprites) {
            spriteContent += tab + "    <SpriteName>" + assetName_renderable.first + "</SpriteName>\r\n";
        }
        return
            tab + "<Renderer>\r\n" +
            tab + "  <Texts>\r\n" +
            textContent +
            tab + "  </Texts>\r\n" +
            tab + "  <Rectangles>\r\n" +
            rectangleContent +
            tab + "  </Rectangles>\r\n" +
            tab + "  <Circles>\r\n" +
            circleContent +
            tab + "  </Circles>\r\n" +
            tab + "  <Sprites>\r\n" +
            spriteContent +
            tab + "  </Sprites>\r\n" +
            tab + "</Renderables>\r\n";
    }

    void Renderer::deserialize(XMLNode* node, bool forPrefab) {

    }

#pragma endregion

#pragma region Behaviors

    std::bitset<4> Pawn::ids;

    std::string Pawn::serialize(std::string tab, bool forPrefab) {
        return 
            tab + "<Pawn>\r\n" +
            tab + "  <PlayerID>" + std::to_string(playerId) + "</PlayerID>\r\n" +
            tab + "</Pawn>\r\n";
    }

    void Pawn::deserialize(XMLNode* node, bool forPrefab) {
        XMLElement* e = node->FirstChildElement("PlayerID");
        ids.reset(playerId);
        e->QueryIntText(&playerId);
        if (ids.test(playerId)) {
            cerr << "Warning: Deserialized Pawn using ID already assigned in Pawn::ids" << endl;
        }
        ids.set(playerId);
    }

    std::string Villain::serialize(std::string tab, bool forPrefab) {
        return 
            tab + "<Villain>\r\n" + 
            tab + "</Villain>\r\n";
    }

    void Villain::deserialize(XMLNode* node, bool forPrefab) {

    }

    std::string Tracker::serialize(std::string tab, bool forPrefab) {
        return
            tab + "<Tracker>\r\n" +
            tab + "  <Target>" + std::to_string(target) + "</Target>\r\n" +
            tab + "</Tracker>\r\n";
    }

    void Tracker::deserialize(XMLNode* node, bool forPrefab) {
        int i;
        node->FirstChildElement("Target")->QueryIntText(&i);
        target = (ComponentType) i;
    }

    std::string Pacer::serialize(std::string tab, bool forPrefab) {
        return
            tab + "<Pacer>\r\n" +
            tab + "  <Direction>" + std::to_string(direction) + "</Direction>\r\n" +
            tab + "  <Velocity>\r\n" +
            tab + "    <X>" + std::to_string(velocity.x) + "</X>\r\n" +
            tab + "    <Y>" + std::to_string(velocity.y) + "</Y>\r\n" +
            tab + "  </Velocity>\r\n" +
            tab + "  <Origin>\r\n" +
            tab + "    <X>" + std::to_string(velocity.x) + "</X>\r\n" +
            tab + "    <Y>" + std::to_string(velocity.y) + "</Y>\r\n" +
            tab + "  </Origin>\r\n" +
            tab + "  <Radius>" + std::to_string(radius) + "</Radius>\r\n" +
            tab + "</Pacer>\r\n";
    }

    void Pacer::deserialize(XMLNode* node, bool forPrefab) {
        int i;
        node->FirstChildElement("Direction")->QueryIntText(&i);
        direction = (Direction) i;
        XMLElement* vector = node->FirstChildElement("Velocity");
        vector->FirstChildElement("X")->QueryFloatText(&velocity.x);
        vector->FirstChildElement("Y")->QueryFloatText(&velocity.y);
        vector = node->FirstChildElement("Origin");
        vector->FirstChildElement("X")->QueryFloatText(&velocity.x);
        vector->FirstChildElement("Y")->QueryFloatText(&velocity.y);
        node->FirstChildElement("Radius")->QueryFloatText(&radius);
    }

#pragma endregion

}
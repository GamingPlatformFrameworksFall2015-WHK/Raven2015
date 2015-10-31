#include "ComponentLibrary.h"
#include "EventLibrary.h"
#include "Game.h"
#include "XMLSystem.h"

namespace Raven {

#pragma region Data

    std::string Data::serialize(std::string tab) {
        return
            tab + "<Data>\r\n" +
            tab + "  <Name>" + this->name + "</Name>\r\n" +
            tab + "  <refabName>" + this->prefabName + "</refabName>\r\n" +
            tab + "  <Modified>" + std::to_string(this->modified) + "</Modified>\r\n" +
            tab + "  <ersistent>" + std::to_string(this->persistent) + "</ersistent>\r\n" +
            tab + "</Data>\r\n";
    }

    void Data::deserialize(XMLNode* node) {
        XMLElement* nameElem = node->FirstChildElement("Name");
        name = nameElem->GetText();
        prefabName = node->FirstChildElement("PrefabName")->GetText();
        node->FirstChildElement("Modified")->QueryBoolText(&this->modified);
        node->FirstChildElement("Persistent")->QueryBoolText(&this->persistent);
    }

#pragma endregion

#pragma region Physics

    std::string Transform::serialize(std::string tab) {
        return
            tab + "<Transform>\r\n" +
            tab + "  <SubTransform>\r\n" +
            tab + "    <TransformX>" + std::to_string(this->transform.x) + "</TransformX>\r\n" +
            tab + "    <TransformY>" + std::to_string(this->transform.y) + "</TransformY>\r\n" +
            tab + "  </SubTransform>\r\n" +
            tab + "  <Rotation>" + std::to_string(this->rotation) + "</Rotation>\r\n" +
            tab + "</Transform>\r\n";
    }

    void Transform::deserialize(XMLNode* node) {
        XMLNode* t = node->FirstChild();
        t->FirstChildElement("TransformX")->QueryFloatText(&(this->transform.x));
        t->FirstChildElement("TransformY")->QueryFloatText(&this->transform.y);
        node->FirstChildElement("Rotation")->QueryFloatText(&this->rotation);
    }

    std::string Rigidbody::serialize(std::string tab) {
        return
            tab + "<Rigidbody>\r\n" +
            tab + "  <Velocity>\r\n" +
            tab + "    <VelocityX>" + std::to_string(this->velocity.x) + "</VelocityX>\r\n" +
            tab + "    <VelocityY>" + std::to_string(this->velocity.y) + "</VelocityY>\r\n" +
            tab + "  </Velocity>\r\n" +
            tab + "  <Acceleration>\r\n" +
            tab + "    <AccelerationX>" + std::to_string(this->acceleration.x) + "</AccelerationX>\r\n" +
            tab + "    <AccelerationY>" + std::to_string(this->acceleration.y) + "</AccelerationY>\r\n" +
            tab + "  </Acceleration>\r\n" +
            tab + "  <RadialVelocity>" + std::to_string(this->radialVelocity) + "</RadialVelocity>\r\n" +
            tab + "</Rigidbody>\r\n";
    }

    void Rigidbody::deserialize(XMLNode* node) {
        XMLNode* t = node->FirstChildElement("Velocity");
        t->FirstChildElement("VelocityX")->QueryFloatText(&(this->velocity.x));
        t->FirstChildElement("VelocityY")->QueryFloatText(&this->velocity.y);
        t = node->FirstChildElement("Acceleration");
        t->FirstChildElement("AccelerationX")->QueryFloatText(&(this->acceleration.x));
        t->FirstChildElement("AccelerationY")->QueryFloatText(&this->acceleration.y);
        node->FirstChildElement("RadialVelocity")->QueryFloatText(&this->radialVelocity);
    }

    std::string BoxCollider::serialize(std::string tab) {
        std::string layersContent = "";
        for (std::string layer : layers) {
            layersContent +=
                tab + "    <ayer>" + layer + "</ayer>\r\n";
        }
        bool solid = collisionSettings.find("Solid") != collisionSettings.end();
        bool fixed = collisionSettings.find("Fixed") != collisionSettings.end();

        return
            tab + "<BoxCollider>\r\n" +
            tab + "  <Width>" + std::to_string(this->width) + "</Width>\r\n" +
            tab + "  <Height>" + std::to_string(this->height) + "</Height>\r\n" +
            tab + "  <XOffset>" + std::to_string(this->originOffset.x) + "</XOffset>\r\n" +
            tab + "  <YOffset>" + std::to_string(this->originOffset.y) + "</YOffset>\r\n" +
            tab + "  <ayers>\r\n" +
            layersContent +
            tab + "  </ayers>\r\n" +
            tab + "  <Settings>\r\n" +
            tab + "    <Solid>" + std::to_string(solid) + "</Solid>\r\n" +
            tab + "    <Fixed>" + std::to_string(fixed) + "</Fixed>\r\n" +
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
            layers.insert(t->GetText());
        } while (t = t->NextSiblingElement("Layer"));

        t = node->FirstChildElement("Settings");
        collisionSettings.clear();
        for (std::string setting : cmn::CollisionInformation::settings) {
            bool val;
            t->FirstChildElement(setting.c_str())->QueryBoolText(&val);
            if (val) collisionSettings.insert("" + setting);
        }
    }

#pragma endregion

#pragma region Audio

    std::string SoundMaker::serialize(std::string tab) {
        std::string soundContent = "";
        for (auto soundFilePath_soundBuffer : soundMap) {
            soundContent +=
                tab + "  <SoundMakerSoundFilePath>" + soundFilePath_soundBuffer.first + "</SoundMakerSoundFilePath>\r\n";
        }
        return
            tab + "<SoundMaker>\r\n" +
            soundContent +
            tab + "</SoundMaker>\r\n";
    }

    void SoundMaker::deserialize(XMLNode* node) {
        XMLElement* e = node->FirstChildElement("SoundMakerSoundFilePath");
        do {
            soundMap.insert(std::make_pair(e->GetText(), std::shared_ptr<sf::SoundBuffer>(new sf::SoundBuffer())));
            cmn::game->events.emit<AudioEvent>(e->GetText(), this, 
                cmn::EAudioType::SOUND, cmn::EAudioOperation::AUDIO_LOAD, cmn::EAudioLoop::LOOP_FALSE);
        } while (e = e->NextSiblingElement("SoundMakerSoundFilePath"));
    }

    std::string MusicMaker::serialize(std::string tab) {
        std::string musicContent = "";
        for (auto musicFilePath_music : musicMap) {
            musicContent +=
                tab + "  <MusicMakerMusicFilePath>" + musicFilePath_music.first + "</MusicMakerMusicFilePath>\r\n";
        }
        return
            tab + "<MusicMaker>\r\n" +
            musicContent +
            tab + "</MusicMaker>\r\n";
    }

    void MusicMaker::deserialize(XMLNode* node) {
        XMLElement* e = node->FirstChildElement("MusicMakerMusicFilePath");
        do {
            musicMap.insert(std::make_pair(e->GetText(), std::shared_ptr<sf::Music>(new sf::Music())));
            cmn::game->events.emit<AudioEvent>(e->GetText(), this, 
                cmn::EAudioType::MUSIC, cmn::EAudioOperation::AUDIO_LOAD, cmn::EAudioLoop::LOOP_FALSE);
        } while (e = e->NextSiblingElement("MusicMakerMusicFilePath"));
    }

#pragma endregion

#pragma region Rendering

    std::string Renderer::serialize(std::string tab) {
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

    void Renderer::deserialize(XMLNode* node) {
        XMLElement* e = node->FirstChildElement("Texts");
        XMLElement* t = e->FirstChildElement("TextName");
        if (t) {
            do {
                texts.insert(std::make_pair(t->GetText(), cmn::game->systems.system<XMLSystem>()->renderableTextMap[t->GetText()]));
                //cmn::game->events.emit<XMLDeserializeRendererAsset<RenderableText>>(t->GetText(), this);
            } while (t = t->NextSiblingElement("TextName"));
        }
        e = node->FirstChildElement("Rectangles");
        t = e->FirstChildElement("RectangleName");
        if (t) {
            do {
                rectangles.insert(std::make_pair(t->GetText(), cmn::game->systems.system<XMLSystem>()->renderableRectangleMap[t->GetText()]));
                //cmn::game->events.emit<XMLDeserializeRendererAsset<RenderableRectangle>>(t->GetText(), this);
            } while (t = t->NextSiblingElement("RectangleName"));
        }
        e = node->FirstChildElement("Circles");
        t = e->FirstChildElement("CircleName");
        if (t) {
            do {
                circles.insert(std::make_pair(t->GetText(), cmn::game->systems.system<XMLSystem>()->renderableCircleMap[t->GetText()]));
                //cmn::game->events.emit<XMLDeserializeRendererAsset<RenderableCircle>>(t->GetText(), this);
            } while (t = t->NextSiblingElement("CircleName"));
        }
        e = node->FirstChildElement("Sprites");
        t = e->FirstChildElement("SpriteName");
        if (t) {
            do {
                sprites.insert(std::make_pair(t->GetText(), cmn::game->systems.system<XMLSystem>()->renderableSpriteMap[t->GetText()]));
                //cmn::game->events.emit<XMLDeserializeRendererAsset<RenderableSprite>>(t->GetText(), this);
            } while (t = t->NextSiblingElement("SpriteName"));
        }
    }

#pragma endregion

#pragma region Behaviors

    std::bitset<4> Pawn::ids;

    std::string Pawn::serialize(std::string tab) {
        return
            tab + "<Pawn>\r\n" +
            tab + "  <PlayerID>" + std::to_string(playerId) + "</PlayerID>\r\n" +
            tab + "</Pawn>\r\n";
    }

    void Pawn::deserialize(XMLNode* node) {
        XMLElement* e = nullptr;
        e = node->FirstChildElement("PlayerID");
        ids.reset(playerId);
        e->QueryIntText(&playerId);
        if (ids.test(playerId)) {
            cerr << "Warning: Deserialized Pawn using ID already assigned in Pawn::ids" << endl;
        }
        ids.set(playerId);
    }

    std::string Villain::serialize(std::string tab) {
        return
            tab + "<Villain>\r\n" +
            tab + "</Villain>\r\n";
    }

    void Villain::deserialize(XMLNode* node) {

    }

    std::string Tracker::serialize(std::string tab) {
        return
            tab + "<Tracker>\r\n" +
            tab + "  <Target>" + std::to_string(target) + "</Target>\r\n" +
            tab + "</Tracker>\r\n";
    }

    void Tracker::deserialize(XMLNode* node) {
        int i;
        node->FirstChildElement("Target")->QueryIntText(&i);
        target = (ComponentType) i;
    }

    std::string Pacer::serialize(std::string tab) {
        return
            tab + "<Pacer>\r\n" +
            tab + "  <PacerDirection>" + std::to_string(direction) + "</PacerDirection>\r\n" +
            tab + "  <PacerVelocity>\r\n" +
            tab + "    <PacerVelocityX>" + std::to_string(velocity.x) + "</PacerVelocityX>\r\n" +
            tab + "    <PacerVelocityY>" + std::to_string(velocity.y) + "</PacerVelocityY>\r\n" +
            tab + "  </PacerVelocity>\r\n" +
            tab + "  <PacerOrigin>\r\n" +
            tab + "    <PacerOriginX>" + std::to_string(velocity.x) + "</PacerOriginX>\r\n" +
            tab + "    <PacerOriginY>" + std::to_string(velocity.y) + "</PacerOriginY>\r\n" +
            tab + "  </PacerOrigin>\r\n" +
            tab + "  <PacerRadius>" + std::to_string(radius) + "</PacerRadius>\r\n" +
            tab + "</Pacer>\r\n";
    }

    void Pacer::deserialize(XMLNode* node) {
        int i;
        node->FirstChildElement("PacerDirection")->QueryIntText(&i);
        direction = (Direction)i;
        XMLElement* vector = node->FirstChildElement("PacerVelocity");
        vector->FirstChildElement("PacerVelocityX")->QueryFloatText(&velocity.x);
        vector->FirstChildElement("PacerVelocityY")->QueryFloatText(&velocity.y);
        vector = node->FirstChildElement("PacerOrigin");
        vector->FirstChildElement("PacerOriginX")->QueryFloatText(&velocity.x);
        vector->FirstChildElement("PacerOriginY")->QueryFloatText(&velocity.y);
        node->FirstChildElement("PacerRadius")->QueryFloatText(&radius);
    }

#pragma endregion

}
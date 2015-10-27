#include "ComponentLibrary.h"
#include "EventLibrary.h"

namespace Raven {

#pragma region Data

    std::string Data::serialize(std::string tab, bool forPrefab) {
        if (forPrefab) {
            return
                tab + "<PData>\r\n" +
                tab + "  <PName>" + this->name + "</PName>\r\n" +
                tab + "  <PPrefabName>" + this->prefabName + "</PPrefabName>\r\n" +
                tab + "  <PModified>" + std::to_string(this->modified) + "</PModified>\r\n" +
                tab + "  <PPersistent>" + std::to_string(this->persistent) + "</PPersistent>\r\n" +
                tab + "</PData>\r\n";
        }
        else {
            return
                tab + "<LData>\r\n" +
                tab + "  <LName>" + this->name + "</LName>\r\n" +
                tab + "  <LPrefabName>" + this->prefabName + "</LPrefabName>\r\n" +
                tab + "  <LModified>" + std::to_string(this->modified) + "</LModified>\r\n" +
                tab + "  <LPersistent>" + std::to_string(this->persistent) + "</LPersistent>\r\n" +
                tab + "</LData>\r\n";
        }

    }

    void Data::deserialize(XMLNode* node, bool forPrefab) {
        if (forPrefab) {
            XMLElement* nameElem = node->FirstChildElement("PName");
            name = nameElem->GetText();
            prefabName = node->FirstChildElement("PPrefabName")->GetText();
            node->FirstChildElement("PModified")->QueryBoolText(&this->modified);
            node->FirstChildElement("PPersistent")->QueryBoolText(&this->persistent);
        }
        else {
            name = node->FirstChildElement("LName")->GetText();
            prefabName = node->FirstChildElement("LPrefabName")->GetText();
            node->FirstChildElement("LModified")->QueryBoolText(&this->modified);
            node->FirstChildElement("LPersistent")->QueryBoolText(&this->persistent);
        }
    }

#pragma endregion

#pragma region Physics

    std::string Transform::serialize(std::string tab, bool forPrefab) {
        if (forPrefab) {
            return
                tab + "<PTransform>\r\n" +
                tab + "  <PSubTransform>\r\n" +
                tab + "    <PTransformX>" + std::to_string(this->transform.x) + "</PTransformX>\r\n" +
                tab + "    <PTransformY>" + std::to_string(this->transform.y) + "</PTransformY>\r\n" +
                tab + "  </PSubTransform>\r\n" +
                tab + "  <PRotation>" + std::to_string(this->rotation) + "</PRotation>\r\n" +
                tab + "</PTransform>\r\n";
        }
        else {
            return
                tab + "<LTransform>\r\n" +
                tab + "  <LSubTransform>\r\n" +
                tab + "    <LTransformX>" + std::to_string(this->transform.x) + "</LTransformX>\r\n" +
                tab + "    <LTransformY>" + std::to_string(this->transform.y) + "</LTransformY>\r\n" +
                tab + "  </LSubTransform>\r\n" +
                tab + "  <LRotation>" + std::to_string(this->rotation) + "</LRotation>\r\n" +
                tab + "</LTransform>\r\n";
        }
    }

    void Transform::deserialize(XMLNode* node, bool forPrefab) {
        XMLNode* t = node->FirstChild();
        if (forPrefab) {
            t->FirstChildElement("PTransformX")->QueryFloatText(&(this->transform.x));
            t->FirstChildElement("PTransformY")->QueryFloatText(&this->transform.y);
            node->FirstChildElement("PRotation")->QueryFloatText(&this->rotation);
        }
        else {
            t->FirstChildElement("LTransformX")->QueryFloatText(&(this->transform.x));
            t->FirstChildElement("LTransformY")->QueryFloatText(&this->transform.y);
            node->FirstChildElement("LRotation")->QueryFloatText(&this->rotation);
        }
    }

    std::string Rigidbody::serialize(std::string tab, bool forPrefab) {
        if (forPrefab) {
            return
                tab + "<PRigidbody>\r\n" +
                tab + "  <PVelocity>\r\n" +
                tab + "    <PVelocityX>" + std::to_string(this->velocity.x) + "</PVelocityX>\r\n" +
                tab + "    <PVelocityY>" + std::to_string(this->velocity.y) + "</PVelocityY>\r\n" +
                tab + "  </PVelocity>\r\n" +
                tab + "  <PAcceleration>\r\n" +
                tab + "    <PAccelerationX>" + std::to_string(this->acceleration.x) + "</PAccelerationX>\r\n" +
                tab + "    <PAccelerationY>" + std::to_string(this->acceleration.y) + "</PAccelerationY>\r\n" +
                tab + "  </PAcceleration>\r\n" +
                tab + "  <PRadialVelocity>" + std::to_string(this->radialVelocity) + "</PRadialVelocity>\r\n" +
                tab + "</PRigidbody>\r\n";
        }
        else {
            return
                tab + "<LRigidbody>\r\n" +
                tab + "  <LVelocity>\r\n" +
                tab + "    <LVelocityX>" + std::to_string(this->velocity.x) + "</LVelocityX>\r\n" +
                tab + "    <LVelocityY>" + std::to_string(this->velocity.y) + "</LVelocityY>\r\n" +
                tab + "  </LVelocity>\r\n" +
                tab + "  <LAcceleration>\r\n" +
                tab + "    <LAccelerationX>" + std::to_string(this->acceleration.x) + "</LAccelerationX>\r\n" +
                tab + "    <LAccelerationY>" + std::to_string(this->acceleration.y) + "</LAccelerationY>\r\n" +
                tab + "  </LAcceleration>\r\n" +
                tab + "  <LRadialVelocity>" + std::to_string(this->radialVelocity) + "</LRadialVelocity>\r\n" +
                tab + "</LRigidbody>\r\n";
        }
    }

    void Rigidbody::deserialize(XMLNode* node, bool forPrefab) {
        if (forPrefab) {
            XMLNode* t = node->FirstChildElement("PVelocity");
            t->FirstChildElement("PVelocityX")->QueryFloatText(&(this->velocity.x));
            t->FirstChildElement("PVelocityY")->QueryFloatText(&this->velocity.y);
            t = node->FirstChildElement("PAcceleration");
            t->FirstChildElement("PAccelerationX")->QueryFloatText(&(this->acceleration.x));
            t->FirstChildElement("PAccelerationY")->QueryFloatText(&this->acceleration.y);
            node->FirstChildElement("PRadialVelocity")->QueryFloatText(&this->radialVelocity);
        }
        else {
            XMLNode* t = node->FirstChildElement("LVelocity");
            t->FirstChildElement("LVelocityX")->QueryFloatText(&(this->velocity.x));
            t->FirstChildElement("LVelocityY")->QueryFloatText(&this->velocity.y);
            t = node->FirstChildElement("LAcceleration");
            t->FirstChildElement("LAccelerationX")->QueryFloatText(&(this->acceleration.x));
            t->FirstChildElement("LAccelerationY")->QueryFloatText(&this->acceleration.y);
            node->FirstChildElement("LRadialVelocity")->QueryFloatText(&this->radialVelocity);
        }
    }

    std::string BoxCollider::serialize(std::string tab, bool forPrefab) {
        std::string layersContent = "";

        if (forPrefab) {
            for (std::string layer : layers) {
                layersContent +=
                    tab + "    <PLayer>" + layer + "</PLayer>\r\n";
            }
            bool solid = collisionSettings.find("PSolid") != collisionSettings.end();
            bool fixed = collisionSettings.find("PFixed") != collisionSettings.end();

            return
                tab + "<PBoxCollider>\r\n" +
                tab + "  <PWidth>" + std::to_string(this->width) + "</PWidth>\r\n" +
                tab + "  <PHeight>" + std::to_string(this->height) + "</PHeight>\r\n" +
                tab + "  <PXOffset>" + std::to_string(this->originOffset.x) + "</PXOffset>\r\n" +
                tab + "  <PYOffset>" + std::to_string(this->originOffset.y) + "</PYOffset>\r\n" +
                tab + "  <PLayers>\r\n" +
                layersContent +
                tab + "  </PLayers>\r\n" +
                tab + "  <PSettings>\r\n" +
                tab + "    <PSolid>" + std::to_string(solid) + "</PSolid>\r\n" +
                tab + "    <PFixed>" + std::to_string(fixed) + "</PFixed>\r\n" +
                tab + "  </PSettings>\r\n" +
                tab + "</PBoxCollider>\r\n";
        }
        else {
            for (std::string layer : layers) {
                layersContent +=
                    tab + "    <LLayer>" + layer + "</LLayer>\r\n";
            }
            bool solid = collisionSettings.find("LSolid") != collisionSettings.end();
            bool fixed = collisionSettings.find("LFixed") != collisionSettings.end();

            return
                tab + "<LBoxCollider>\r\n" +
                tab + "  <LWidth>" + std::to_string(this->width) + "</LWidth>\r\n" +
                tab + "  <LHeight>" + std::to_string(this->height) + "</LHeight>\r\n" +
                tab + "  <LXOffset>" + std::to_string(this->originOffset.x) + "</LXOffset>\r\n" +
                tab + "  <LYOffset>" + std::to_string(this->originOffset.y) + "</LYOffset>\r\n" +
                tab + "  <LLayers>\r\n" +
                layersContent +
                tab + "  </LLayers>\r\n" +
                tab + "  <LSettings>\r\n" +
                tab + "    <LSolid>" + std::to_string(solid) + "</LSolid>\r\n" +
                tab + "    <LFixed>" + std::to_string(fixed) + "</LFixed>\r\n" +
                tab + "  </LSettings>\r\n" +
                tab + "</LBoxCollider>\r\n";
        }
    }

    void BoxCollider::deserialize(XMLNode* node, bool forPrefab) {
        if (forPrefab) {
            node->FirstChildElement("PWidth")->QueryFloatText(&this->width);
            node->FirstChildElement("PHeight")->QueryFloatText(&this->height);
            node->FirstChildElement("PXOffset")->QueryFloatText(&this->originOffset.x);
            node->FirstChildElement("PYOffset")->QueryFloatText(&this->originOffset.y);

            XMLElement* t = node->FirstChildElement("PLayer");
            layers.clear();
            do {
                layers.insert(t->GetText());
            } while (t = t->NextSiblingElement("PLayer"));

            t = node->FirstChildElement("PSettings");
            collisionSettings.clear();
            for (std::string setting : cmn::CollisionInformation::settings) {
                bool val;
                t->FirstChildElement(setting.c_str())->QueryBoolText(&val);
                if (val) collisionSettings.insert("P" + setting);
            }
        }
        else {
            node->FirstChildElement("LWidth")->QueryFloatText(&this->width);
            node->FirstChildElement("LHeight")->QueryFloatText(&this->height);
            node->FirstChildElement("LXOffset")->QueryFloatText(&this->originOffset.x);
            node->FirstChildElement("LYOffset")->QueryFloatText(&this->originOffset.y);

            XMLElement* t = node->FirstChildElement("LLayer");
            layers.clear();
            do {
                layers.insert(t->GetText());
            } while (t = t->NextSiblingElement("LLayer"));

            t = node->FirstChildElement("LSettings");
            collisionSettings.clear();
            for (std::string setting : cmn::CollisionInformation::settings) {
                bool val;
                t->FirstChildElement(setting.c_str())->QueryBoolText(&val);
                if (val) collisionSettings.insert("L" + setting);
            }
        }
    }

#pragma endregion

#pragma region Audio

    std::string SoundMaker::serialize(std::string tab, bool forPrefab) {
        std::string soundContent = "";
        if (forPrefab) {
            for (auto soundFilePath_soundBuffer : soundMap) {
                soundContent +=
                    tab + "  <PSoundMakerSoundFilePath>" + soundFilePath_soundBuffer.first + "</PSoundMakerSoundFilePath>\r\n";
            }
            return
                tab + "<PSoundMaker>\r\n" +
                soundContent +
                tab + "</PSoundMaker>\r\n";
        }
        else {
            for (auto soundFilePath_soundBuffer : soundMap) {
                soundContent +=
                    tab + "  <LSoundMakerSoundFilePath>" + soundFilePath_soundBuffer.first + "</LSoundMakerSoundFilePath>\r\n";
            }
            return
                tab + "<LSoundMaker>\r\n" +
                soundContent +
                tab + "</LSoundMaker>\r\n";
        }
    }

    void SoundMaker::deserialize(XMLNode* node, bool forPrefab) {
        if (forPrefab) {
            XMLElement* e = node->FirstChildElement("PSoundMakerSoundFilePath");
            do {
                soundMap.insert(std::make_pair(e->GetText(), std::shared_ptr<sf::SoundBuffer>(new sf::SoundBuffer())));
            } while (e = e->NextSiblingElement("PSoundMakerSoundFilePath"));
        }
        else {
            XMLElement* e = node->FirstChildElement("LSoundMakerSoundFilePath");
            do {
                soundMap.insert(std::make_pair(e->GetText(), std::shared_ptr<sf::SoundBuffer>(new sf::SoundBuffer())));
            } while (e = e->NextSiblingElement("LSoundMakerSoundFilePath"));
        }
    }

    std::string MusicMaker::serialize(std::string tab, bool forPrefab) {
        std::string musicContent = "";
        if (forPrefab) {
            for (auto musicFilePath_music : musicMap) {
                musicContent +=
                    tab + "  <PMusicMakerMusicFilePath>" + musicFilePath_music.first + "</PMusicMakerMusicFilePath>\r\n";
            }
            return
                tab + "<PMusicMaker>\r\n" +
                musicContent +
                tab + "</PMusicMaker>\r\n";
        }
        else {
            for (auto musicFilePath_music : musicMap) {
                musicContent +=
                    tab + "  <LMusicMakerMusicFilePath>" + musicFilePath_music.first + "</LMusicMakerMusicFilePath>\r\n";
            }
            return
                tab + "<LMusicMaker>\r\n" +
                musicContent +
                tab + "</LMusicMaker>\r\n";
        }
    }

    void MusicMaker::deserialize(XMLNode* node, bool forPrefab) {
        if (forPrefab) {
            XMLElement* e = node->FirstChildElement("PMusicMakerMusicFilePath");
            do {
                musicMap.insert(std::make_pair(e->GetText(), std::shared_ptr<sf::Music>(new sf::Music())));
            } while (e = e->NextSiblingElement("PMusicMakerMusicFilePath"));
        }
        else {
            XMLElement* e = node->FirstChildElement("LMusicMakerMusicFilePath");
            do {
                musicMap.insert(std::make_pair(e->GetText(), std::shared_ptr<sf::Music>(new sf::Music())));
            } while (e = e->NextSiblingElement("LMusicMakerMusicFilePath"));
        }
    }

#pragma endregion

#pragma region Rendering

    std::string Renderer::serialize(std::string tab, bool forPrefab) {
        std::string textContent = "";
        std::string rectangleContent = "";
        std::string circleContent = "";
        std::string spriteContent = "";
        if (forPrefab) {
            for (auto assetName_renderable : texts) {
                textContent += tab + "    <PTextName>" + assetName_renderable.first + "</PTextName>\r\n";
            }
            for (auto assetName_renderable : rectangles) {
                rectangleContent += tab + "    <PRectangleName>" + assetName_renderable.first + "</PRectangleName>\r\n";
            }
            for (auto assetName_renderable : circles) {
                circleContent += tab + "    <PCircleName>" + assetName_renderable.first + "</PCircleName>\r\n";
            }
            for (auto assetName_renderable : sprites) {
                spriteContent += tab + "    <PSpriteName>" + assetName_renderable.first + "</PSpriteName>\r\n";
            }
            return
                tab + "<PRenderer>\r\n" +
                tab + "  <PTexts>\r\n" +
                textContent +
                tab + "  </PTexts>\r\n" +
                tab + "  <PRectangles>\r\n" +
                rectangleContent +
                tab + "  </PRectangles>\r\n" +
                tab + "  <PCircles>\r\n" +
                circleContent +
                tab + "  </PCircles>\r\n" +
                tab + "  <PSprites>\r\n" +
                spriteContent +
                tab + "  </PSprites>\r\n" +
                tab + "</PRenderables>\r\n";
        }
        else {
            for (auto assetName_renderable : texts) {
                textContent += tab + "    <LTextName>" + assetName_renderable.first + "</LTextName>\r\n";
            }
            for (auto assetName_renderable : rectangles) {
                rectangleContent += tab + "    <LRectangleName>" + assetName_renderable.first + "</LRectangleName>\r\n";
            }
            for (auto assetName_renderable : circles) {
                circleContent += tab + "    <LCircleName>" + assetName_renderable.first + "</LCircleName>\r\n";
            }
            for (auto assetName_renderable : sprites) {
                spriteContent += tab + "    <LSpriteName>" + assetName_renderable.first + "</LSpriteName>\r\n";
            }
            return
                tab + "<LRenderer>\r\n" +
                tab + "  <LTexts>\r\n" +
                textContent +
                tab + "  </LTexts>\r\n" +
                tab + "  <LRectangles>\r\n" +
                rectangleContent +
                tab + "  </LRectangles>\r\n" +
                tab + "  <LCircles>\r\n" +
                circleContent +
                tab + "  </LCircles>\r\n" +
                tab + "  <LSprites>\r\n" +
                spriteContent +
                tab + "  </LSprites>\r\n" +
                tab + "</LRenderables>\r\n";
        }
    }

    void Renderer::deserialize(XMLNode* node, bool forPrefab) {
        if (forPrefab) {
            XMLElement* e = node->FirstChildElement("PTexts");
            XMLElement* t = e->FirstChildElement("PTextName");
            if (t) {
                do {
                    cmn::events->emit<XMLDeserializeRendererAsset<RenderableText>>(t->GetText(), texts, forPrefab);
                } while (t = t->NextSiblingElement("PTextName"));
            }
            e = node->FirstChildElement("PRectangles");
            t = e->FirstChildElement("PRectangleName");
            if (t) {
                do {
                    cmn::events->emit<XMLDeserializeRendererAsset<RenderableText>>(t->GetText(), texts, forPrefab);
                } while (t = t->NextSiblingElement("PRectangleName"));
            }
            e = node->FirstChildElement("PCircles");
            t = e->FirstChildElement("PCircleName");
            if (t) {
                do {
                    cmn::events->emit<XMLDeserializeRendererAsset<RenderableText>>(t->GetText(), texts, forPrefab);
                } while (t = t->NextSiblingElement("PCircleName"));
            }
            e = node->FirstChildElement("PSprites");
            t = e->FirstChildElement("PSpriteName");
            if (t) {
                do {
                    cmn::events->emit<XMLDeserializeRendererAsset<RenderableText>>(t->GetText(), texts, forPrefab);
                } while (t = t->NextSiblingElement("PSpriteName"));
            }
        }
        else {
            XMLElement* e = node->FirstChildElement("LTexts");
            XMLElement* t = e->FirstChildElement("LTextName");
            if (t) {
                do {
                    cmn::events->emit<XMLDeserializeRendererAsset<RenderableText>>(t->GetText(), texts, forPrefab);
                } while (t = t->NextSiblingElement("LTextName"));
            }
            e = node->FirstChildElement("LRectangles");
            t = e->FirstChildElement("LRectangleName");
            if (t) {
                do {
                    cmn::events->emit<XMLDeserializeRendererAsset<RenderableText>>(t->GetText(), texts, forPrefab);
                } while (t = t->NextSiblingElement("LRectangleName"));
            }
            e = node->FirstChildElement("LCircles");
            t = e->FirstChildElement("LCircleName");
            if (t) {
                do {
                    cmn::events->emit<XMLDeserializeRendererAsset<RenderableText>>(t->GetText(), texts, forPrefab);
                } while (t = t->NextSiblingElement("LCircleName"));
            }
            e = node->FirstChildElement("LSprites");
            t = e->FirstChildElement("LSpriteName");
            if (t) {
                do {
                    cmn::events->emit<XMLDeserializeRendererAsset<RenderableText>>(t->GetText(), texts, forPrefab);
                } while (t = t->NextSiblingElement("LSpriteName"));
            }
        }
    }

#pragma endregion

#pragma region Behaviors

    std::bitset<4> Pawn::ids;

    std::string Pawn::serialize(std::string tab, bool forPrefab) {
        if (forPrefab) {
            return
                tab + "<PPawn>\r\n" +
                tab + "  <PPlayerID>" + std::to_string(playerId) + "</PPlayerID>\r\n" +
                tab + "</PPawn>\r\n";
        }
        else {
            return
                tab + "<LPawn>\r\n" +
                tab + "  <LPlayerID>" + std::to_string(playerId) + "</LPlayerID>\r\n" +
                tab + "</LPawn>\r\n";

        }
    }

    void Pawn::deserialize(XMLNode* node, bool forPrefab) {
        XMLElement* e = nullptr;
        if (forPrefab) {
            e = node->FirstChildElement("PPlayerID");
        }
        else {
            e = node->FirstChildElement("LPlayerID");
        }
        ids.reset(playerId);
        e->QueryIntText(&playerId);
        if (ids.test(playerId)) {
            cerr << "Warning: Deserialized Pawn using ID already assigned in Pawn::ids" << endl;
        }
        ids.set(playerId);
    }

    std::string Villain::serialize(std::string tab, bool forPrefab) {
        if (forPrefab) {
            return
                tab + "<PVillain>\r\n" +
                tab + "</PVillain>\r\n";
        }
        else {
            return
                tab + "<LVillain>\r\n" +
                tab + "</LVillain>\r\n";
        }
    }

    void Villain::deserialize(XMLNode* node, bool forPrefab) {

    }

    std::string Tracker::serialize(std::string tab, bool forPrefab) {
        if (forPrefab) {
            return
                tab + "<PTracker>\r\n" +
                tab + "  <PTarget>" + std::to_string(target) + "</PTarget>\r\n" +
                tab + "</PTracker>\r\n";
        }
        else {
            return
                tab + "<LTracker>\r\n" +
                tab + "  <LTarget>" + std::to_string(target) + "</LTarget>\r\n" +
                tab + "</LTracker>\r\n";
        }
    }

    void Tracker::deserialize(XMLNode* node, bool forPrefab) {
        int i;
        if (forPrefab) {
            node->FirstChildElement("PTarget")->QueryIntText(&i);
        }
        else {
            node->FirstChildElement("LTarget")->QueryIntText(&i);
        }
        target = (ComponentType) i;
    }

    std::string Pacer::serialize(std::string tab, bool forPrefab) {
        if (forPrefab) {
            return
                tab + "<PPacer>\r\n" +
                tab + "  <PPacerDirection>" + std::to_string(direction) + "</PPacerDirection>\r\n" +
                tab + "  <PPacerVelocity>\r\n" +
                tab + "    <PPacerVelocityX>" + std::to_string(velocity.x) + "</PPacerVelocityX>\r\n" +
                tab + "    <PPacerVelocityY>" + std::to_string(velocity.y) + "</PPacerVelocityY>\r\n" +
                tab + "  </PPacerVelocity>\r\n" +
                tab + "  <PPacerOrigin>\r\n" +
                tab + "    <PPacerOriginX>" + std::to_string(velocity.x) + "</PPacerOriginX>\r\n" +
                tab + "    <PPacerOriginY>" + std::to_string(velocity.y) + "</PPacerOriginY>\r\n" +
                tab + "  </PPacerOrigin>\r\n" +
                tab + "  <PPacerRadius>" + std::to_string(radius) + "</PPacerRadius>\r\n" +
                tab + "</PPacer>\r\n";
        }
        else {
            return
                tab + "<LPacer>\r\n" +
                tab + "  <LPacerDirection>" + std::to_string(direction) + "</LPacerDirection>\r\n" +
                tab + "  <LPacerVelocity>\r\n" +
                tab + "    <LPacerVelocityX>" + std::to_string(velocity.x) + "</LPacerVelocityX>\r\n" +
                tab + "    <LPacerVelocityY>" + std::to_string(velocity.y) + "</LPacerVelocityY>\r\n" +
                tab + "  </LPacerVelocity>\r\n" +
                tab + "  <LPacerOrigin>\r\n" +
                tab + "    <LPacerOriginX>" + std::to_string(velocity.x) + "</LPacerOriginX>\r\n" +
                tab + "    <LPacerOriginY>" + std::to_string(velocity.y) + "</LPacerOriginY>\r\n" +
                tab + "  </LPacerOrigin>\r\n" +
                tab + "  <LPacerRadius>" + std::to_string(radius) + "</LPacerRadius>\r\n" +
                tab + "</LPacer>\r\n";
        }
    }

    void Pacer::deserialize(XMLNode* node, bool forPrefab) {
        int i;
        if (forPrefab) {
            node->FirstChildElement("PPacerDirection")->QueryIntText(&i);
            direction = (Direction)i;
            XMLElement* vector = node->FirstChildElement("PPacerVelocity");
            vector->FirstChildElement("PPacerVelocityX")->QueryFloatText(&velocity.x);
            vector->FirstChildElement("PPacerVelocityY")->QueryFloatText(&velocity.y);
            vector = node->FirstChildElement("PPacerOrigin");
            vector->FirstChildElement("PPacerOriginX")->QueryFloatText(&velocity.x);
            vector->FirstChildElement("PPacerOriginY")->QueryFloatText(&velocity.y);
            node->FirstChildElement("PPacerRadius")->QueryFloatText(&radius);
        }
        else {
            node->FirstChildElement("LPacerDirection")->QueryIntText(&i);
            direction = (Direction)i;
            XMLElement* vector = node->FirstChildElement("LPacerVelocity");
            vector->FirstChildElement("LPacerVelocityX")->QueryFloatText(&velocity.x);
            vector->FirstChildElement("LPacerVelocityY")->QueryFloatText(&velocity.y);
            vector = node->FirstChildElement("LPacerOrigin");
            vector->FirstChildElement("LPacerOriginX")->QueryFloatText(&velocity.x);
            vector->FirstChildElement("LPacerOriginY")->QueryFloatText(&velocity.y);
            node->FirstChildElement("LPacerRadius")->QueryFloatText(&radius);
        }
    }

#pragma endregion

}
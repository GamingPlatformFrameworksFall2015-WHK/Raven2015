#include "ComponentLibrary.h"
#include "EventLibrary.h"
#include "Game.h"
#include "XMLSystem.h"
#include "WidgetLibrary.h"

#define COMBO_TEXT 0
#define COMBO_RECTANGLE 1
#define COMBO_CIRCLE 2
#define COMBO_SPRITE 3

namespace Raven {

    template <typename C>
    ComboBox::Ptr fillComboBoxWithComponents(ComboBox::Ptr box, C* c) {
        box->AppendItem(c->getElementName().c_str());
        return box;
    }

    template <typename C, typename... Components>
    ComboBox::Ptr fillComboBoxWithComponents(ComboBox::Ptr box, C* c, Components*... components) {
        box->AppendItem(c->getElementName().c_str());
        return fillComboBoxWithComponents<Components...>(box, components...);
    }

    void(*componentFormatter)(Box::Ptr) = [](Box::Ptr box) {
        Label* varName = (Label*)box->GetChildren()[0].get();
        varName->SetText("");
        Entry* editableValue = (Entry*) box->GetChildren()[1].get();
        editableValue->SetRequisition(sf::Vector2f(160.f, 20.f));
        editableValue->SetText("");
        Label* currentValue = (Label*)box->GetChildren()[2].get();
        currentValue->SetText("");
        Label* hiddenData = (Label*)box->GetChildren()[3].get();
        hiddenData->SetText("");
        hiddenData->Show(false);
        ComboBox* comboData = (ComboBox*)box->GetChildren()[4].get();
        comboData->Show(false);
    };

    ComboBox* (*rendererFormatter)(Box::Ptr) = [](Box::Ptr box) {
        componentFormatter(box);
        ComboBox* comboData = (ComboBox*)box->GetChildren()[4].get();
        comboData->Show(true);
        comboData->AppendItem("Text");
        comboData->AppendItem("Rectangle");
        comboData->AppendItem("Circle");
        comboData->AppendItem("Sprite");
        return comboData;
    };

    void(*rendererFormatterText)(Box::Ptr) = [](Box::Ptr box) {
        componentFormatter(box);
        ComboBox* comboData = rendererFormatter(box);
        comboData->SelectItem(COMBO_TEXT);
    };

    void(*rendererFormatterRectangle)(Box::Ptr) = [](Box::Ptr box) {
        componentFormatter(box);
        ComboBox* comboData = rendererFormatter(box);
        comboData->SelectItem(COMBO_RECTANGLE);
    };

    void(*rendererFormatterCircle)(Box::Ptr) = [](Box::Ptr box) {
        componentFormatter(box);
        ComboBox* comboData = rendererFormatter(box);
        comboData->SelectItem(COMBO_CIRCLE);
    };
    
    void(*rendererFormatterSprite)(Box::Ptr) = [](Box::Ptr box) {
        componentFormatter(box);
        ComboBox* comboData = rendererFormatter(box);
        comboData->SelectItem(COMBO_SPRITE);
    };

    void(*trackerFormatter)(Box::Ptr) = [](Box::Ptr box) {
        componentFormatter(box);
        Entry* e = (Entry*)box->GetChildren()[1].get();
        e->Show(false);
        ComboBox* comboData = (ComboBox*)box->GetChildren()[4].get();
        comboData->Show(true);
    };

    void(*pacerDirectionFormatter)(Box::Ptr) = [](Box::Ptr box) {
        componentFormatter(box);
        Entry* e = (Entry*)box->GetChildren()[1].get();
        e->Show(false);
        ComboBox* comboData = (ComboBox*)box->GetChildren()[4].get();
        comboData->AppendItem("Vertical");
        comboData->AppendItem("Horizontal");
        comboData->AppendItem("Diagonal");
        comboData->Show(true);
    };

    void initEditableAssetListItem(Box::Ptr box, std::string value) {
        ((Label*)box->GetChildren()[0].get())->SetText(box->GetName()); // varName
        ((Entry*)box->GetChildren()[1].get())->SetText(value.c_str());  // edited value
        ((Label*)box->GetChildren()[2].get())->SetText(value.c_str());  // current value
    }

    std::string getHiddenData(Box::Ptr box, size_t position) {
        return ((Label*)((Box*)box->GetChildren()[position].get())->GetChildren()[3].get())->GetText();
    }

    int getDropDownValue(Box::Ptr box, size_t position) {
        return ((ComboBox*)((Box*)box->GetChildren()[position].get())->GetChildren()[4].get())->GetSelectedItem();
    }

    std::string getEntryValue(Box::Ptr box, size_t position) {
        return ((Entry*)((Box*)box->GetChildren()[position].get())->GetChildren()[1].get())->GetText();
    }

    template <typename C>
    std::string getNameByTypeHelper(ComponentType type, std::string name, C* c) {
        if (c->getType() == type) {
            name = c->getElementName();
        }
        return name;
    }

    template <typename C, typename... Components>
    std::string getNameByTypeHelper(ComponentType type, std::string name, C* c, Components*... components) {
        if (c->getType() == type) {
            name = c->getElementName();
        }
        return getNameByTypeHelper<Components...>(type, name, components...);
    }

    std::string getNameByType(ComponentType type) {
        return getNameByTypeHelper<COMPONENT_TYPE_LIST>(type, "", COMPONENT_TYPES(::getNullPtrToType()));
    }

    struct Pacer;

    std::string getTextFromDirection(Pacer::Direction dir) {
        switch (dir) {
        case Pacer::Direction::HORIZONTAL: return "Horizontal";
        case Pacer::Direction::VERTICAL:   return "Vertical";
        case Pacer::Direction::DIAGONAL:   return "Diagonal";
        default: return "";
        }
    }

#pragma region Data

    std::string Data::serialize(std::string tab) {
        return
            tab + "<Data>\r\n" +
            tab + "  <Name>" + this->name + "</Name>\r\n" +
            tab + "  <PrefabName>" + this->prefabName + "</PrefabName>\r\n" +
            tab + "  <Modified>" + std::to_string(this->modified) + "</Modified>\r\n" +
            tab + "  <Persistent>" + std::to_string(this->persistent) + "</Persistent>\r\n" +
            tab + "</Data>\r\n";
    }

    void Data::deserialize(XMLNode* node) {
        name = node->FirstChildElement("Name")->GetText();
        prefabName = node->FirstChildElement("PrefabName")->GetText();
        node->FirstChildElement("Modified")->QueryBoolText(&this->modified);
        node->FirstChildElement("Persistent")->QueryBoolText(&this->persistent);
    }

    Box::Ptr Data::createWidget() {
        Box::Ptr box = ED_ASSET_WIDGET_LIST::Create();

        Box::Ptr nameBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "name", componentFormatter);
        initEditableAssetListItem(nameBox, name.c_str());
        Box::Ptr prefabNameBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "prefabName", componentFormatter);
        initEditableAssetListItem(prefabNameBox, prefabName.c_str());
        Box::Ptr modifiedBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "modified", componentFormatter);
        initEditableAssetListItem(modifiedBox, std::to_string(modified).c_str());
        Box::Ptr persistentBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "persistent", componentFormatter);
        initEditableAssetListItem(persistentBox, std::to_string(persistent).c_str());

        return box;
    }

    bool Data::deserializeWidget(Box::Ptr box) {
        std::string s;
        bool b = true;
        b &= (name = getEntryValue(box, 0)).size() ? true : false;
        b &= (prefabName = getEntryValue(box, 1)).size() ? true : false;
        b &= (s = getEntryValue(box, 2)).size() ? true : false;
        if (b) modified = (std::stoi(s) ? true : false);
        b &= (s = getEntryValue(box, 3)).size() ? true : false;
        if (b) persistent = (std::stoi(s) ? true : false);
        return b;
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

    Box::Ptr Transform::createWidget() {
        Box::Ptr box = ED_ASSET_WIDGET_LIST::Create();

        Box::Ptr transformXBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "Transform X", componentFormatter);
        initEditableAssetListItem(transformXBox, std::to_string(transform.x).c_str());
        Box::Ptr transformYBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "Transform Y", componentFormatter);
        initEditableAssetListItem(transformYBox, std::to_string(transform.y).c_str());
        Box::Ptr rotationBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "Rotation", componentFormatter);
        initEditableAssetListItem(rotationBox, std::to_string(rotation).c_str());

        return box;
    }

    bool Transform::deserializeWidget(Box::Ptr box) {
        std::string s;
        bool b = true;
        b &= (s = getEntryValue(box, 0)).size() ? true : false;
        if (b) transform.x = std::stof(s);
        b &= (s = getEntryValue(box, 1)).size() ? true : false;
        if (b) transform.y = std::stof(s);
        b &= (s = getEntryValue(box, 2)).size() ? true : false;
        if (b) rotation = std::stof(s);
        return b;
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

    Box::Ptr Rigidbody::createWidget() {
        Box::Ptr box = ED_ASSET_WIDGET_LIST::Create();

        Box::Ptr velocityXBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "Velocity X", componentFormatter);
        initEditableAssetListItem(velocityXBox, std::to_string(velocity.x).c_str());
        Box::Ptr velocityYBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "Velocity Y", componentFormatter);
        initEditableAssetListItem(velocityYBox, std::to_string(velocity.y).c_str());
        Box::Ptr accelerationXBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "Acceleration X", componentFormatter);
        initEditableAssetListItem(accelerationXBox, std::to_string(acceleration.x).c_str());
        Box::Ptr accelerationYBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "Acceleration Y", componentFormatter);
        initEditableAssetListItem(accelerationYBox, std::to_string(acceleration.y).c_str());
        Box::Ptr radialVelocityBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "RadialVelocity", componentFormatter);
        initEditableAssetListItem(radialVelocityBox, std::to_string(radialVelocity).c_str());

        return box;
    }

    bool Rigidbody::deserializeWidget(Box::Ptr box) {
        std::string s;
        bool b = true;
        b &= (s = getEntryValue(box, 0)).size() ? true : false;
        if (b) velocity.x = std::stof(s);
        b &= (s = getEntryValue(box, 1)).size() ? true : false;
        if (b) velocity.y = std::stof(s);
        b &= (s = getEntryValue(box, 2)).size() ? true : false;
        if (b) acceleration.x = std::stof(s);
        b &= (s = getEntryValue(box, 3)).size() ? true : false;
        if (b) acceleration.y = std::stof(s);
        b &= (s = getEntryValue(box, 4)).size() ? true : false;
        if (b) radialVelocity = std::stof(s);
        return b;
    }

    std::string BoxCollider::serialize(std::string tab) {
        std::string layersContent = "";
        for (std::string layer : layers) {
            layersContent +=
                tab + "    <Layer>" + layer + "</Layer>\r\n";
        }
        bool solid = collisionSettings.find("Solid") != collisionSettings.end();
        bool fixed = collisionSettings.find("Fixed") != collisionSettings.end();

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
        while (t) {
            layers.insert(t->GetText());
            t = t->NextSiblingElement("Layer");
        } 

        t = node->FirstChildElement("Settings");
        collisionSettings.clear();
        for (std::string setting : cmn::CollisionInformation::settings) {
            bool val;
            t->FirstChildElement(setting.c_str())->QueryBoolText(&val);
            if (val) collisionSettings.insert("" + setting);
        }
    }

    Box::Ptr BoxCollider::createWidget() {
        Box::Ptr box = ED_ASSET_WIDGET_LIST::Create();

        Box::Ptr widthBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "Width", componentFormatter);
        initEditableAssetListItem(widthBox, std::to_string(width).c_str());
        Box::Ptr heightBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "Height", componentFormatter);
        initEditableAssetListItem(heightBox, std::to_string(height).c_str());
        for (auto layer : layers) {
            Box::Ptr layerBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "Collision Layer", componentFormatter);
            initEditableAssetListItem(layerBox, layer.c_str());
        }
        Box::Ptr solidBox = ED_ASSET_WIDGET_LIST::appendWidget(box, COLLISION_LAYER_SETTINGS_SOLID, componentFormatter);
        initEditableAssetListItem(solidBox, COLLISION_LAYER_SETTINGS_SOLID);
        Box::Ptr fixedBox = ED_ASSET_WIDGET_LIST::appendWidget(box, COLLISION_LAYER_SETTINGS_FIXED, componentFormatter);
        initEditableAssetListItem(fixedBox, COLLISION_LAYER_SETTINGS_FIXED);

        return box;
    }

    bool BoxCollider::deserializeWidget(Box::Ptr box) {
        std::string s;
        bool b = true;
        b &= (s = getEntryValue(box, 0)).size() ? true : false;
        if (b) width = std::stof(s);
        b &= (s = getEntryValue(box, 1)).size() ? true : false;
        if (b) height = std::stof(s);
        b &= (s = getHiddenData(box, 2)).size() ? true : false;
        int numLayers = b ? stoi(s) : b;
        for (int i = 0; i < numLayers; ++i) {
            b &= (s = getEntryValue(box, 2 + i)).size() ? true : false;
        }
        b &= (s = getEntryValue(box, 2 + numLayers)).size() ? true : false;
        if (b && stoi(s)) {
            collisionSettings.insert(COLLISION_LAYER_SETTINGS_SOLID);
        }
        b &= (s = getEntryValue(box, 3 + numLayers)).size() ? true : false;
        if (b && stoi(s)) {
            collisionSettings.insert(COLLISION_LAYER_SETTINGS_FIXED);
        }
        return b;
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
        while (e) {
            soundMap.insert(std::make_pair(e->GetText(), std::shared_ptr<sf::SoundBuffer>(new sf::SoundBuffer())));
            cmn::game->events.emit<AudioEvent>(e->GetText(), this,
                cmn::EAudioType::SOUND, cmn::EAudioOperation::AUDIO_LOAD, cmn::EAudioLoop::LOOP_FALSE);
            e = e->NextSiblingElement("SoundMakerSoundFilePath");
        }
    }

    Box::Ptr SoundMaker::createWidget() {
        Box::Ptr box = ED_ASSET_WIDGET_LIST::Create();

        for (auto name_soundBuffer : soundMap) {
            Box::Ptr soundBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "Sound", componentFormatter);
            initEditableAssetListItem(soundBox, name_soundBuffer.first.c_str());
        }

        return box;
    }

    bool SoundMaker::deserializeWidget(Box::Ptr box) {
        std::string s;
        bool b = true;
        b &= (s = getHiddenData(box, 0)).size() ? true : false;
        int numSounds = b ? stoi(s) : b;
        soundMap.clear();
        for (int i = 0; i < numSounds; ++i) {
            b &= (s = getEntryValue(box, i)).size() ? true : false;
            soundMap.insert(std::make_pair(s, std::shared_ptr<sf::SoundBuffer>(new sf::SoundBuffer())));
            if (!soundMap.at(s)->loadFromFile(s)) {
                cerr << "Warning: Widget-Deserialization: SoundBuffer failed to load sound at path: " + s << endl;
            }
        }
        sound.setLoop(false);
        return b;
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
        while (e) {
            musicMap.insert(std::make_pair(e->GetText(), std::shared_ptr<sf::Music>(new sf::Music())));
            cmn::game->events.emit<AudioEvent>(e->GetText(), this, 
                cmn::EAudioType::MUSIC, cmn::EAudioOperation::AUDIO_LOAD, cmn::EAudioLoop::LOOP_FALSE);

            e = e->NextSiblingElement("MusicMakerMusicFilePath");
        } 
    }

    Box::Ptr MusicMaker::createWidget() {
        Box::Ptr box = ED_ASSET_WIDGET_LIST::Create();

        for (auto name_music : musicMap) {
            Box::Ptr musicBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "Music", componentFormatter);
            initEditableAssetListItem(musicBox, name_music.first.c_str());
        }

        return box;
    }

    bool MusicMaker::deserializeWidget(Box::Ptr box) {
        std::string s;
        bool b = true;
        b &= (s = getHiddenData(box, 0)).size() ? true : false;
        int numMusic = b ? stoi(s) : b;
        musicMap.clear();
        for (int i = 0; i < numMusic; ++i) {
            b &= (s = getEntryValue(box, i)).size() ? true : false;
            musicMap.insert(std::make_pair(s, std::shared_ptr<sf::Music>(new sf::Music())));
            if (!musicMap.at(s)->openFromFile(s)) {
                cerr << "Warning: Widget-Deserialization: SoundBuffer failed to open music at path: " + s << endl;
            }
            musicMap[s]->setLoop(true);
        }
        return b;
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
            tab + "</Renderer>\r\n";
    }

    void Renderer::deserialize(XMLNode* node) {
        XMLElement* e = node->FirstChildElement("Texts");
        XMLElement* t = e->FirstChildElement("TextName");
        XMLSystem* xml = cmn::game->systems.system<XMLSystem>().get();
        while (t) {
            texts.insert(std::make_pair(t->GetText(), std::shared_ptr<RenderableText>(new RenderableText(*xml->renderableTextMap[t->GetText()].get()))));
            t = t->NextSiblingElement("TextName");
        } 
        e = node->FirstChildElement("Rectangles");
        t = e->FirstChildElement("RectangleName");
        while (t) {
            rectangles.insert(std::make_pair(t->GetText(), std::shared_ptr<RenderableRectangle>(new RenderableRectangle(*xml->renderableRectangleMap[t->GetText()].get()))));
            t = t->NextSiblingElement("RectangleName");
        } 
        e = node->FirstChildElement("Circles");
        t = e->FirstChildElement("CircleName");
        while (t) {
            circles.insert(std::make_pair(t->GetText(), std::shared_ptr<RenderableCircle>(new RenderableCircle(*xml->renderableCircleMap[t->GetText()].get()))));
            t = t->NextSiblingElement("CircleName");
        } 
        e = node->FirstChildElement("Sprites");
        t = e->FirstChildElement("SpriteName");
        while (t) {
            sprites.insert(std::make_pair(t->GetText(), std::shared_ptr<RenderableSprite>(new RenderableSprite(*xml->renderableSpriteMap[t->GetText()].get()))));
            t = t->NextSiblingElement("SpriteName");
        } 
    }

    Box::Ptr Renderer::createWidget() {
        Box::Ptr box = ED_ASSET_WIDGET_LIST::Create();

        for (auto name_renderable : texts) {
            Box::Ptr textBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "Text", rendererFormatterText);
            initEditableAssetListItem(textBox, name_renderable.first.c_str());
        }
        for (auto name_renderable : rectangles) {
            Box::Ptr rectangleBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "Rectangle", rendererFormatterRectangle);
            initEditableAssetListItem(rectangleBox, name_renderable.first.c_str());
        }
        for (auto name_renderable : circles) {
            Box::Ptr circleBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "Circle", rendererFormatterCircle);
            initEditableAssetListItem(circleBox, name_renderable.first.c_str());
        }
        for (auto name_renderable : sprites) {
            Box::Ptr spriteBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "Sprite", rendererFormatterSprite);
            initEditableAssetListItem(spriteBox, name_renderable.first.c_str());
        }

        return box;
    }

    bool Renderer::deserializeWidget(Box::Ptr box) {
        std::string s;
        bool b = true;
        size_t numItems = box->GetChildren().size();
        auto xml = cmn::game->systems.system<XMLSystem>();
        for (int i = 0; i < numItems; ++i) {
            int type = getDropDownValue(box, i);
            b &= (s = getEntryValue(box, i)).size() ? true : false;
            if (!b) {
                cerr << "Warning: Renderer widget attempting to search for empty string in an asset map. Asset Code: " 
                    + std::to_string(type) << endl;
                continue;
            }
            switch (type) {
            case COMBO_TEXT: texts[s] = xml->renderableTextMap[s]; break;
            case COMBO_RECTANGLE: rectangles[s] = xml->renderableRectangleMap[s]; break;
            case COMBO_CIRCLE: circles[s] = xml->renderableCircleMap[s]; break;
            case COMBO_SPRITE: sprites[s] = xml->renderableSpriteMap[s]; break;
            }
        }
        return b;
    }

 
#pragma endregion

#pragma region Behaviors

    std::string Pawn::serialize(std::string tab) {
        return
            tab + "<Pawn>\r\n" +
            tab + "</Pawn>\r\n";
    }

    void Pawn::deserialize(XMLNode* node) {
    }

    Box::Ptr Pawn::createWidget() {
        Box::Ptr box = Box::Create();

        return box;
    }

    bool Pawn::deserializeWidget(Box::Ptr box) {
        return true;
    }

    std::string Villain::serialize(std::string tab) {
        return
            tab + "<Villain>\r\n" +
            tab + "</Villain>\r\n";
    }

    void Villain::deserialize(XMLNode* node) {

    }

    Box::Ptr Villain::createWidget() {
        Box::Ptr box = Box::Create();

        return box;
    }

    bool Villain::deserializeWidget(Box::Ptr box) {

        return true;
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

    Box::Ptr Tracker::createWidget() {
        Box::Ptr box = ED_ASSET_WIDGET_LIST::Create();

        Box::Ptr targetBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "Target", trackerFormatter);
        ((Label*)box->GetChildren()[0].get())->SetText(box->GetName()); // varName
        ((Label*)box->GetChildren()[2].get())->SetText(getNameByType(target).c_str());  // current value

        return box;
    }

    bool Tracker::deserializeWidget(Box::Ptr box) {
        target = (ComponentType)getDropDownValue(box, 0);
        return true;
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

    Box::Ptr Pacer::createWidget() {
        Box::Ptr box = ED_ASSET_WIDGET_LIST::Create();

        Box::Ptr directionBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "Direction", pacerDirectionFormatter);
        ((Label*)box->GetChildren()[0].get())->SetText(box->GetName()); // varName
        ((Label*)box->GetChildren()[2].get())->SetText(getTextFromDirection(direction).c_str());  // current value
        ((ComboBox*)box->GetChildren()[4].get())->SelectItem(direction);

        Box::Ptr velocityXBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "Velocity X", componentFormatter);
        initEditableAssetListItem(velocityXBox, std::to_string(velocity.x).c_str());
        Box::Ptr velocityYBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "Velocity Y", componentFormatter);
        initEditableAssetListItem(velocityYBox, std::to_string(velocity.y).c_str());
        Box::Ptr originXBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "Origin X", componentFormatter);
        initEditableAssetListItem(originXBox, std::to_string(origin.x).c_str());
        Box::Ptr originYBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "Origin Y", componentFormatter);
        initEditableAssetListItem(originYBox, std::to_string(origin.y).c_str());
        Box::Ptr radiusBox = ED_ASSET_WIDGET_LIST::appendWidget(box, "Radius", componentFormatter);
        initEditableAssetListItem(radiusBox, std::to_string(radius).c_str());

        return box;
    }

    bool Pacer::deserializeWidget(Box::Ptr box) {
        direction = (Direction)getDropDownValue(box, 0);
        std::string s;
        bool b = true;
        b &= (s = getEntryValue(box, 1)).size() ? true : false;
        if (b) velocity.x = std::stof(s);
        b &= (s = getEntryValue(box, 2)).size() ? true : false;
        if (b) velocity.y = std::stof(s);
        b &= (s = getEntryValue(box, 3)).size() ? true : false;
        if (b) origin.x = std::stof(s);
        b &= (s = getEntryValue(box, 4)).size() ? true : false;
        if (b) origin.y = std::stof(s);
        b &= (s = getEntryValue(box, 5)).size() ? true : false;
        if (b) radius = std::stof(s);
        return b;
    }

#pragma endregion

}
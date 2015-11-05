#include "DataAssetLibrary.h"
#include "ComponentLibrary.h"

namespace Raven {

    void(*dataAssetFormatter)(Box::Ptr) = [](Box::Ptr box) {
        //Label* varName = (Label*)box->GetChildren()[0].get();
        //varName->SetText("");
        Entry* editableValue = (Entry*) box->GetChildren()[1].get();
        editableValue->SetRequisition(sf::Vector2f(160.f, 20.f));
        //editableValue->SetText("");
        //Label* currentValue = (Label*)box->GetChildren()[2].get();
        //currentValue->SetText("");
        Label* hiddenData = (Label*)box->GetChildren()[3].get();
        hiddenData->SetText("");
        hiddenData->Show(false);
        ComboBox* comboData = (ComboBox*)box->GetChildren()[4].get();
        comboData->Show(false);
    };

    std::string translateRenderingLayer(cmn::ERenderingLayer layer) {
        switch (layer) {
        case cmn::ERenderingLayer::Foreground: return "Foreground";
        case cmn::ERenderingLayer::Background: return "Background";
        case cmn::ERenderingLayer::HUD: return "HUD";
        case cmn::ERenderingLayer::GUI: return "GUI";
        }
        return "";
    }

    cmn::ERenderingLayer translateRenderingLayer(std::string layer) {
        switch (layer.size()) {
        case 10: 
            switch (layer.at(0)) {
            case 'F': return cmn::ERenderingLayer::Foreground;
            case 'B': return cmn::ERenderingLayer::Background;
            }
            break;
        case 3:
            switch (layer.at(0)) {
            case 'H': return cmn::ERenderingLayer::HUD;
            case 'G': return cmn::ERenderingLayer::GUI;
            }
            break;
        }
        return cmn::ERenderingLayer::NO_LAYER;
    }

    void setAssetWidgetValue(Box::Ptr hbox, std::string value) {
        Entry* editableValue = (Entry*)hbox->GetChildren()[1].get();
        editableValue->SetText(value.c_str());
        Label* currentValue = (Label*)hbox->GetChildren()[2].get();
        editableValue->SetText(value.c_str());
    }

#define ADD_ASSET_ITEM_TO_WIDGET(name, value) \
        ED_ASSET_WIDGET_LIST::appendWidget(hbox, name, dataAssetFormatter); \
        setAssetWidgetValue(hbox, value);

#define GET_ENTRY_AND_TAKE_ACTION(action) \
        b &= (s = getEntryValue(box, index++)).size() ? true : false; \
        if (b) action;

    void initRenderableWidget(Box::Ptr hbox, Renderable& r) {
        ADD_ASSET_ITEM_TO_WIDGET("Origin Offset X", std::to_string(r.offsetX))
        ADD_ASSET_ITEM_TO_WIDGET("Origin Offset Y", std::to_string(r.offsetY))
        ADD_ASSET_ITEM_TO_WIDGET("Render Layer", translateRenderingLayer(r.renderLayer))
        ADD_ASSET_ITEM_TO_WIDGET("Render Priority", std::to_string(r.renderPriority))
    }

    void initTransformableWidget(Box::Ptr hbox, sf::Transformable* t, std::string renderableType) {
        ED_ASSET_WIDGET_LIST::appendWidget(hbox, renderableType + " Position X", dataAssetFormatter);
        setAssetWidgetValue(hbox, std::to_string(t->getPosition().x));
        ED_ASSET_WIDGET_LIST::appendWidget(hbox, renderableType + " Position Y", dataAssetFormatter);
        setAssetWidgetValue(hbox, std::to_string(t->getPosition().y));
        ED_ASSET_WIDGET_LIST::appendWidget(hbox, renderableType + " Rotation", dataAssetFormatter);
        setAssetWidgetValue(hbox, std::to_string(t->getRotation()));
    }

    Box::Ptr RenderableText::createWidget() {
        Box::Ptr box = ED_ASSET_WIDGET_LIST::Create();

        initRenderableWidget(box, *this);
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Font File Path", dataAssetFormatter);
        setAssetWidgetValue(box, fontFilePath);
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Text String", dataAssetFormatter);
        setAssetWidgetValue(box, text.getString());
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Text Size", dataAssetFormatter);
        setAssetWidgetValue(box, std::to_string(text.getCharacterSize()));
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Text Color R", dataAssetFormatter);
        setAssetWidgetValue(box, std::to_string(text.getColor().r));
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Text Color G", dataAssetFormatter);
        setAssetWidgetValue(box, std::to_string(text.getColor().g));
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Text Color B", dataAssetFormatter);
        setAssetWidgetValue(box, std::to_string(text.getColor().b));
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Text Color A", dataAssetFormatter);
        setAssetWidgetValue(box, std::to_string(text.getColor().a));
        initTransformableWidget(box, (sf::Transformable*) this, "Text");

        return box;
    }

    bool RenderableText::deserializeWidget(Box::Ptr box) {
        std::string s;
        bool b = true;
        int index = 0;
        GET_ENTRY_AND_TAKE_ACTION(offsetX = stof(s));
        GET_ENTRY_AND_TAKE_ACTION(offsetY = stof(s));
        GET_ENTRY_AND_TAKE_ACTION(renderLayer = translateRenderingLayer(s));
        GET_ENTRY_AND_TAKE_ACTION(renderPriority = stoi(s));
        GET_ENTRY_AND_TAKE_ACTION(
            fontFilePath = s;
            if (!font.loadFromFile(fontFilePath)) {
                cerr << "Warning: Font failed to load from path: " + fontFilePath << endl;
            }
        );
        sf::Uint8 red, green, blue, alpha;
        GET_ENTRY_AND_TAKE_ACTION(text.setString(s));
        GET_ENTRY_AND_TAKE_ACTION(text.setCharacterSize(stoi(s)));
        GET_ENTRY_AND_TAKE_ACTION(red = text.getColor().r);
        GET_ENTRY_AND_TAKE_ACTION(green = text.getColor().g);
        GET_ENTRY_AND_TAKE_ACTION(blue = text.getColor().b);
        GET_ENTRY_AND_TAKE_ACTION(alpha = text.getColor().a);
        text.setColor(sf::Color(red, green, blue, alpha));
        float x, y, rotation;
        GET_ENTRY_AND_TAKE_ACTION(x = stof(s));
        GET_ENTRY_AND_TAKE_ACTION(y = stof(s));
        text.setPosition(sf::Vector2f(x, y));
        GET_ENTRY_AND_TAKE_ACTION(rotation = stof(s));
        text.setRotation(rotation);

        return b;
    }

    Box::Ptr RenderableRectangle::createWidget() {
        Box::Ptr box = ED_ASSET_WIDGET_LIST::Create();

        initRenderableWidget(box, *this);
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Rectangle Color R", dataAssetFormatter);
        setAssetWidgetValue(box, std::to_string(rectangle.getFillColor().r));
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Rectangle Color G", dataAssetFormatter);
        setAssetWidgetValue(box, std::to_string(rectangle.getFillColor().g));
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Rectangle Color B", dataAssetFormatter);
        setAssetWidgetValue(box, std::to_string(rectangle.getFillColor().b));
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Rectangle Color A", dataAssetFormatter);
        setAssetWidgetValue(box, std::to_string(rectangle.getFillColor().a));
        initTransformableWidget(box, (sf::Transformable*) this, "Rectangle");

        return box;
    }

    bool RenderableRectangle::deserializeWidget(Box::Ptr box) {
        std::string s;
        bool b = true;
        int index = 0;
        GET_ENTRY_AND_TAKE_ACTION(offsetX = stof(s));
        GET_ENTRY_AND_TAKE_ACTION(offsetY = stof(s));
        GET_ENTRY_AND_TAKE_ACTION(renderLayer = translateRenderingLayer(s));
        GET_ENTRY_AND_TAKE_ACTION(renderPriority = stoi(s));
        sf::Uint8 red, green, blue, alpha;
        GET_ENTRY_AND_TAKE_ACTION(red = rectangle.getFillColor().r);
        GET_ENTRY_AND_TAKE_ACTION(green = rectangle.getFillColor().g);
        GET_ENTRY_AND_TAKE_ACTION(blue = rectangle.getFillColor().b);
        GET_ENTRY_AND_TAKE_ACTION(alpha = rectangle.getFillColor().a);
        rectangle.setFillColor(sf::Color(red, green, blue, alpha));
        float x, y, rotation;
        GET_ENTRY_AND_TAKE_ACTION(x = stof(s));
        GET_ENTRY_AND_TAKE_ACTION(y = stof(s));
        rectangle.setPosition(sf::Vector2f(x, y));
        GET_ENTRY_AND_TAKE_ACTION(rotation = stof(s));
        rectangle.setRotation(rotation);
        return b;
    }

    Box::Ptr RenderableCircle::createWidget() {
        Box::Ptr box = ED_ASSET_WIDGET_LIST::Create();
        
        initRenderableWidget(box, *this);
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Rectangle Color R", dataAssetFormatter);
        setAssetWidgetValue(box, std::to_string(circle.getFillColor().r));
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Rectangle Color G", dataAssetFormatter);
        setAssetWidgetValue(box, std::to_string(circle.getFillColor().g));
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Rectangle Color B", dataAssetFormatter);
        setAssetWidgetValue(box, std::to_string(circle.getFillColor().b));
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Rectangle Color A", dataAssetFormatter);
        setAssetWidgetValue(box, std::to_string(circle.getFillColor().a));
        initTransformableWidget(box, (sf::Transformable*) this, "Rectangle");

        return box;
    }

    bool RenderableCircle::deserializeWidget(Box::Ptr box) {
        std::string s;
        bool b = true;
        int index = 0;
        GET_ENTRY_AND_TAKE_ACTION(offsetX = stof(s));
        GET_ENTRY_AND_TAKE_ACTION(offsetY = stof(s));
        GET_ENTRY_AND_TAKE_ACTION(renderLayer = translateRenderingLayer(s));
        GET_ENTRY_AND_TAKE_ACTION(renderPriority = stoi(s));
        sf::Uint8 red, green, blue, alpha;
        GET_ENTRY_AND_TAKE_ACTION(red = circle.getFillColor().r);
        GET_ENTRY_AND_TAKE_ACTION(green = circle.getFillColor().g);
        GET_ENTRY_AND_TAKE_ACTION(blue = circle.getFillColor().b);
        GET_ENTRY_AND_TAKE_ACTION(alpha = circle.getFillColor().a);
        circle.setFillColor(sf::Color(red, green, blue, alpha));
        float x, y, rotation;
        GET_ENTRY_AND_TAKE_ACTION(x = stof(s));
        GET_ENTRY_AND_TAKE_ACTION(y = stof(s));
        circle.setPosition(sf::Vector2f(x, y));
        GET_ENTRY_AND_TAKE_ACTION(rotation = stof(s));
        circle.setRotation(rotation);
        return b;
    }

    Box::Ptr RenderableSprite::createWidget() {
        Box::Ptr box = ED_ASSET_WIDGET_LIST::Create();

        initRenderableWidget(box, *this);
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Sprite Sheet File Path", dataAssetFormatter);
        setAssetWidgetValue(box, textureFileName.c_str());
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Animation To Use", dataAssetFormatter);
        setAssetWidgetValue(box, animName.c_str());
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Rectangle Color R", dataAssetFormatter);
        setAssetWidgetValue(box, std::to_string(sprite.getColor().r));
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Rectangle Color G", dataAssetFormatter);
        setAssetWidgetValue(box, std::to_string(sprite.getColor().g));
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Rectangle Color B", dataAssetFormatter);
        setAssetWidgetValue(box, std::to_string(sprite.getColor().b));
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Rectangle Color A", dataAssetFormatter);
        setAssetWidgetValue(box, std::to_string(sprite.getColor().a));
        initTransformableWidget(box, (sf::Transformable*) this, "Rectangle");

        return box;
    }

    bool RenderableSprite::deserializeWidget(Box::Ptr box) {
        std::string s;
        bool b = true;
        int index = 0;
        GET_ENTRY_AND_TAKE_ACTION(offsetX = stof(s));
        GET_ENTRY_AND_TAKE_ACTION(offsetY = stof(s));
        GET_ENTRY_AND_TAKE_ACTION(renderLayer = translateRenderingLayer(s));
        GET_ENTRY_AND_TAKE_ACTION(renderPriority = stoi(s));
        GET_ENTRY_AND_TAKE_ACTION(textureFileName = s);
        GET_ENTRY_AND_TAKE_ACTION(animName = s);
        sf::Uint8 red, green, blue, alpha;
        GET_ENTRY_AND_TAKE_ACTION(red = sprite.getColor().r);
        GET_ENTRY_AND_TAKE_ACTION(green = sprite.getColor().g);
        GET_ENTRY_AND_TAKE_ACTION(blue = sprite.getColor().b);
        GET_ENTRY_AND_TAKE_ACTION(alpha = sprite.getColor().a);
        sprite.setColor(sf::Color(red, green, blue, alpha));
        float x, y, rotation;
        GET_ENTRY_AND_TAKE_ACTION(x = stof(s));
        GET_ENTRY_AND_TAKE_ACTION(y = stof(s));
        sprite.setPosition(sf::Vector2f(x, y));
        GET_ENTRY_AND_TAKE_ACTION(rotation = stof(s));
        sprite.setRotation(rotation);
        return b;
    }

    Box::Ptr Animation::createWidget() {
        Box::Ptr box = ED_ASSET_WIDGET_LIST::Create();

        ED_ASSET_WIDGET_LIST::appendWidget(box, "Sprite Sheet File Path", dataAssetFormatter);
        setAssetWidgetValue(box, textureFileName);
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Number of Frames", dataAssetFormatter);
        setAssetWidgetValue(box, std::to_string(size));
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Looping? (0/1)", dataAssetFormatter);
        setAssetWidgetValue(box, std::to_string(isLooping));
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Animation Speed", dataAssetFormatter);
        setAssetWidgetValue(box, std::to_string(animationSpeed));
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Frame Width (Pixels)", dataAssetFormatter);
        setAssetWidgetValue(box, std::to_string(frameWidth));
        ED_ASSET_WIDGET_LIST::appendWidget(box, "Frame Height (Pixels)", dataAssetFormatter);
        setAssetWidgetValue(box, std::to_string(frameHeight));

        return box;
    }

    bool Animation::deserializeWidget(Box::Ptr box) {
        std::string s;
        bool b;
        int index = 0;
        GET_ENTRY_AND_TAKE_ACTION(textureFileName = s);
        GET_ENTRY_AND_TAKE_ACTION(size = stoi(s));
        GET_ENTRY_AND_TAKE_ACTION(isLooping = (bool) stoi(s));
        GET_ENTRY_AND_TAKE_ACTION(animationSpeed = stof(s));
        GET_ENTRY_AND_TAKE_ACTION(frameWidth = stoi(s));
        GET_ENTRY_AND_TAKE_ACTION(frameHeight = stoi(s));
        return b;
    }
}
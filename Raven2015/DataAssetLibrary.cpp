#include "DataAssetLibrary.h"

namespace Raven {

    void(*dataAssetFormatter)(Box::Ptr) = [](Box::Ptr box) {
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

    Box::Ptr RenderableText::createWidget() {
        Box::Ptr box = ED_ASSET_WIDGET_LIST::Create();

        return box;
    }

    bool RenderableText::deserializeWidget(Box::Ptr box) {
        return false;
    }

    Box::Ptr RenderableRectangle::createWidget() {
        Box::Ptr box = ED_ASSET_WIDGET_LIST::Create();

        return box;
    }

    bool RenderableRectangle::deserializeWidget(Box::Ptr box) {
        return false;
    }

    Box::Ptr RenderableCircle::createWidget() {
        Box::Ptr box = ED_ASSET_WIDGET_LIST::Create();
        
        return box;
    }

    bool RenderableCircle::deserializeWidget(Box::Ptr box) {
        return false;
    }

    Box::Ptr RenderableSprite::createWidget() {
        Box::Ptr box = ED_ASSET_WIDGET_LIST::Create();

        return box;
    }

    bool RenderableSprite::deserializeWidget(Box::Ptr box) {
        return false;
    }

    Box::Ptr Animation::createWidget() {
        Box::Ptr box = ED_ASSET_WIDGET_LIST::Create();

        return box;
    }

    bool Animation::deserializeWidget(Box::Ptr box) {
        return false;
    }
}
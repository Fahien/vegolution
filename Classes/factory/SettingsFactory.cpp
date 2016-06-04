#include "SettingsFactory.h"

USING_NS_CC;

SettingsFactory::SettingsFactory(DataManager *data)
        : visibleSize_{Director::getInstance()->getVisibleSize()}
        , data_{data}
        , fontPath_{data_->getString("font.path")}
        , fontSize_{std::stof(data_->getString("font.size").c_str())}
        , textContentSize_{64.0f, 24.0f}
        , layout_{ui::LinearLayoutParameter::create()}
        , shadowOffset_{0.0f, -4.0f}
        , shadowBlur_{8}
        , background_{nullptr}
        , back_{nullptr}
        , menu_{nullptr}
{
    layout_->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    layout_->setMargin(ui::Margin{0.0f, 0.0f, 0.0f, 8.0f});
}

Sprite* SettingsFactory::createBackground() {
    if (background_ == nullptr) {
        log("Creating background");
        background_ = Sprite::create("settings/background.png");
        float scaleX{visibleSize_.width / background_->getContentSize().width};
        background_->setScaleX(scaleX);
        background_->setPosition(visibleSize_.width / 2.0f, visibleSize_.height / 2.0f);
    }
    return background_;
}

ui::Text* SettingsFactory::createBackText()
{
    if (back_ == nullptr) {
        log("Creating back");
        // Get localized string
        std::string settingsText{data_->getString("settings.back")};
        back_ = ui::Text::create(settingsText, fontPath_, fontSize_);
        back_->setContentSize(textContentSize_);
        back_->setPositionX(-textContentSize_.width / 2.0f);
        back_->setLayoutParameter(layout_);
        back_->enableShadow(Color4B::BLACK, shadowOffset_, shadowBlur_);
        back_->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type) {
            ui::Text *target{static_cast<ui::Text *>(sender)};
            switch (type) {
                case ui::Widget::TouchEventType::BEGAN :
                    target->runAction(ScaleTo::create(0.125f, 1.25f));
                    break;
                case ui::Widget::TouchEventType::ENDED :
                    target->runAction(ScaleTo::create(0.125f, 1.0f));
                    Director::getInstance()->popScene();
                    target->setTouchEnabled(false);
                    break;
                case ui::Widget::TouchEventType::CANCELED :
                    target->runAction(ScaleTo::create(0.125f, 1.0f));
                    break;
                default:
                    break;
            }
        });
    }
    back_->setTouchEnabled(true);
    return back_;
}

ui::Layout* SettingsFactory::createMenu() {
    if (menu_ == nullptr) {
        log("Creating menu");
        menu_ = ui::Layout::create();
        Size size {Size{64.0, 64.0f}};
        menu_->setContentSize(size);
        Vec2 center{visibleSize_.width / 2.0f, visibleSize_.height / 2.0f};
        menu_->setPositionX(center.x - size.width / 2);
        menu_->setPositionY(center.y - size.height / 2);
        menu_->setLayoutType(ui::Layout::Type::VERTICAL);
        // Add all children
        menu_->addChild(createBackText());
    }
    return menu_;
}
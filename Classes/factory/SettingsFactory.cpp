#include "SettingsFactory.h"
#include "scene/MainScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

SettingsFactory::SettingsFactory(DataManager* data)
        : visibleSize_{ Director::getInstance()->getVisibleSize() }
        , textContentSize_{ 64.0f, 24.0f }
        , layout_{ ui::LinearLayoutParameter::create() }
        , shadowOffset_{ 0.0f, -4.0f }
        , shadowBlur_{ 8 }
        , background_{ nullptr }
        , resolution_ { nullptr }
        , audio_ { nullptr }
        , back_{ nullptr }
        , menu_{ nullptr }
        , board_{ nullptr }
{
    log("Constructing SettingsFactory");
    fontPath_ = data->getString("font.path");
    fontSize_ = static_cast<float>(data->getInteger("font.size"));
    layout_->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    layout_->setMargin(ui::Margin{0.0f, 0.0f, 0.0f, 8.0f});

    createBackground();
    createResolution(data);
    createAudio(data);
    createBackText(data);
    createMenu();
    createBoard();
}

void SettingsFactory::createBackground() {
    if (background_ == nullptr) {
        log("Creating background");
        background_ = Sprite::create("settings/background.png");
        float scaleX{visibleSize_.width / background_->getContentSize().width};
        background_->setScaleX(scaleX);
        background_->setPosition(visibleSize_.width / 2.0f, visibleSize_.height / 2.0f);
    }
}

void SettingsFactory::createBackText(DataManager* data)
{
    if (back_ != nullptr) return;

    log("Creating back");
    // Get localized string
    std::string settingsText{data->getString("settings.back")};
    back_ = ui::Text::create(settingsText, fontPath_, fontSize_);
    back_->setContentSize(textContentSize_);
    back_->setPositionX(-textContentSize_.width / 2.0f);
    back_->setLayoutParameter(layout_);
    back_->enableShadow(Color4B::BLACK, shadowOffset_, shadowBlur_);

    back_->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type) {
        ui::Text *target{static_cast<ui::Text *>(sender)};
        Scene* scene{ nullptr };
        TransitionFade* transition{ nullptr };
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN :
                target->runAction(ScaleTo::create(0.125f, 1.25f));
                break;
            case ui::Widget::TouchEventType::ENDED :
                target->runAction(ScaleTo::create(0.125f, 1.0f));
                scene = MainScene::create(data);
                transition = TransitionFade::create(0.5f, scene, Color3B::BLACK);
                Director::getInstance()->replaceScene(transition);
                target->setTouchEnabled(false);
                break;
            case ui::Widget::TouchEventType::CANCELED :
                target->runAction(ScaleTo::create(0.125f, 1.0f));
                break;
            default:
                break;
        }
    });
    back_->setTouchEnabled(true);
}

void SettingsFactory::createMenu() {
    if (menu_ != nullptr) return;

    log("Creating menu");
    menu_ = ui::Layout::create();
    Size size {Size{64.0, 64.0f}};
    menu_->setContentSize(size);
    Vec2 center{visibleSize_.width / 2.0f, visibleSize_.height / 2.0f};
    menu_->setPositionX(center.x - size.width / 2);
    menu_->setPositionY(center.y - size.height / 2);
    menu_->setLayoutType(ui::Layout::Type::VERTICAL);
    // Add all children
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    menu_->addChild(resolution_);
#endif
    menu_->addChild(audio_);
    menu_->addChild(back_);
}

void SettingsFactory::createBoard() {
    if (board_ != nullptr) return;

    log("Creating board");
    board_ = Sprite::create("misc/board.png");
    float scaleX{visibleSize_.width / board_->getContentSize().width};
    board_->setScaleX(scaleX);
    Vec2 center{visibleSize_.width / 2.0f, visibleSize_.height / 2.0f};
    board_->setPosition(center);
    board_->setGlobalZOrder(4);
}

void SettingsFactory::createResolution(DataManager *data) {
    if (resolution_ != nullptr) return;

    std::string low {"570x320"};
    std::string mid {"1365x768"};
    std::string high {"1920x1080"};
    int height {data->getInteger("window.height")};
    std::string actual {};
    // LOL
    if (height == 320) { actual = low; mid += " [restart]"; high += " [restart]"; }
    else if (height == 768) { actual = mid; low += " [restart]"; high += " [restart]"; }
    else if (height == 1080) { actual = high; mid += " [restart]"; low += " [restart]"; }

    resolution_ = ui::Text::create(actual, fontPath_, fontSize_);
    resolution_->setContentSize(textContentSize_);
    resolution_->setPositionX(-textContentSize_.width / 2.0f);
    resolution_->setLayoutParameter(layout_);
    resolution_->enableShadow(Color4B::BLACK, shadowOffset_, shadowBlur_);

    resolution_->setTouchEnabled(true);
    resolution_->addTouchEventListener([data, low, mid, high](Ref* sender, ui::Widget::TouchEventType type) {
        ui::Text* resolution {static_cast<ui::Text*>(sender)};
        // Animate
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN :
                resolution->runAction(ScaleTo::create(0.125f, 1.25f));
                break;
            case ui::Widget::TouchEventType::ENDED :
                resolution->runAction(ScaleTo::create(0.125f, 1.0f));
                // Logic
                if (resolution->getString() == low) {
                    resolution->setString(mid);
                    data->saveInteger("window.width", 1365);
                    data->saveInteger("window.height", 768);
                } else if (resolution->getString() == mid) {
                    resolution->setString(high);
                    data->saveInteger("window.width", 1920);
                    data->saveInteger("window.height", 1080);
                } else if (resolution->getString() == high) {
                    resolution->setString(low);
                    data->saveInteger("window.width", 570);
                    data->saveInteger("window.height", 320);
                }
                break;
            case ui::Widget::TouchEventType::CANCELED :
                resolution->runAction(ScaleTo::create(0.125f, 1.0f));
                break;
            default:
                break;
        }
        return true;
    });
}

void SettingsFactory::createAudio(DataManager* data)
{
    if (audio_ != nullptr) return;

    std::string audioOn { "Audio [on]" };
    std::string audioOff { "Audio [off]" };
    int status {data->getInteger("audio.status")};

    std::string audio {(status == 1) ? audioOn : audioOff};

    audio_ = ui::Text::create(audio, fontPath_, fontSize_);
    audio_->setContentSize(textContentSize_);
    audio_->setPositionX(-textContentSize_.width / 2.0f);
    audio_->setLayoutParameter(layout_);
    audio_->enableShadow(Color4B::BLACK, shadowOffset_, shadowBlur_);

    audio_->setTouchEnabled(true);
    audio_->addTouchEventListener([data, audioOn, audioOff](Ref* sender, ui::Widget::TouchEventType type) {
        ui::Text* target {static_cast<ui::Text*>(sender)};
        // Animate
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN :
                target->runAction(ScaleTo::create(0.125f, 1.25f));
                break;
            case ui::Widget::TouchEventType::ENDED :
                target->runAction(ScaleTo::create(0.125f, 1.0f));
                // Audio on
                if (target->getString() == audioOn) {
                    target->setString(audioOff);
                    data->saveInteger("audio.status", 0);
                    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
                    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0);
                } else {
                    target->setString(audioOn);
                    data->saveInteger("audio.status", 1);
                    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
                    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1);
                }
                break;
            case ui::Widget::TouchEventType::CANCELED :
                target->runAction(ScaleTo::create(0.125f, 1.0f));
                break;
            default:
                break;
        }
        return true;
    });
}
#include "SettingsFactory.h"
#include "SimpleAudioEngine.h"
#include "scene/MainScene.h"

USING_NS_CC;

SettingsFactory::SettingsFactory(DataManager& data, TextFactory& textFactory)
        : background_{ Sprite::create("settings/background.png") }
        , resolution_{ nullptr }
        , audio_{ nullptr }
        , back_{ nullptr }
        , menu_{ nullptr }
        , board_{ Sprite::create("misc/board.png") }
{
    log("Constructing SettingsFactory");
    createBackground(textFactory.getVisibleSize());
    createResolution(textFactory, data);
    createAudio(textFactory, data);
    createBackText(data.getString("settings.back"), textFactory, data);
    createMenu(textFactory.getCenter());
    createBoard(textFactory.getVisibleSize());
}

void SettingsFactory::createBackground(Size& visibleSize)
{
        log("Creating background");
        float scaleX{ visibleSize.width / background_->getContentSize().width };
        background_->setScaleX(scaleX);
        background_->setPosition(visibleSize.width / 2.0f, visibleSize.height / 2.0f);
}

void SettingsFactory::createBackText(std::string name, TextFactory& textFactory, DataManager& data)
{
    log("Creating back");
    back_ = textFactory.createText(name);

    back_->setTouchEnabled(true);
    back_->addTouchEventListener([ &data, &textFactory ](Ref* sender, ui::Widget::TouchEventType type) {
        ui::Text* target{ static_cast<ui::Text*>(sender) };
        Scene* scene{ nullptr };
        TransitionFade* transition{ nullptr };
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN :
                target->runAction(ScaleTo::create(0.125f, 1.25f));
                break;
            case ui::Widget::TouchEventType::ENDED :
                target->runAction(ScaleTo::create(0.125f, 1.0f));
                scene = MainScene::create(data, textFactory);
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
}

void SettingsFactory::createMenu(Vec2& center)
{
    log("Creating menu");
    menu_ = ui::Layout::create();
    Size size{ Size{ 64.0, 64.0f }};
    menu_->setContentSize(size);
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

void SettingsFactory::createBoard(Size& visibleSize)
{
    log("Creating board");
    float scaleX{ visibleSize.width / board_->getContentSize().width };
    board_->setScaleX(scaleX);
    Vec2 center{ visibleSize.width / 2.0f, visibleSize.height / 2.0f };
    board_->setPosition(center);
    board_->setGlobalZOrder(4);
}

void SettingsFactory::createResolution(TextFactory& textFactory, DataManager& data)
{
    std::string low{ "570x320" };
    std::string mid{ "1365x768" };
    std::string high{ "1920x1080" };
    int height{ data.getInteger("window.height") };
    std::string actual{ };
    // LOL
    if (height == 320) {
        actual = low;
        mid += " [restart]";
        high += " [restart]";
    }
    else if (height == 768) {
        actual = mid;
        low += " [restart]";
        high += " [restart]";
    }
    else if (height == 1080) {
        actual = high;
        mid += " [restart]";
        low += " [restart]";
    }

    resolution_ = textFactory.createText(actual);

    resolution_->setTouchEnabled(true);
    resolution_->addTouchEventListener([ &data, low, mid, high ](Ref* sender, ui::Widget::TouchEventType type) {
        ui::Text* resolution{ static_cast<ui::Text*>(sender) };
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
                    data.saveInteger("window.width", 1365);
                    data.saveInteger("window.height", 768);
                } else if (resolution->getString() == mid) {
                    resolution->setString(high);
                    data.saveInteger("window.width", 1920);
                    data.saveInteger("window.height", 1080);
                } else if (resolution->getString() == high) {
                    resolution->setString(low);
                    data.saveInteger("window.width", 570);
                    data.saveInteger("window.height", 320);
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

void SettingsFactory::createAudio(TextFactory& textFactory, DataManager& data)
{
    std::string audioOn{ "Audio [on]" };
    std::string audioOff{ "Audio [off]" };
    int status{ data.getInteger("audio.status") };

    std::string audio{ (status == 1) ? audioOn : audioOff };

    audio_ = textFactory.createText(audio);

    audio_->setTouchEnabled(true);
    audio_->addTouchEventListener([ &data, audioOn, audioOff ](Ref* sender, ui::Widget::TouchEventType type) {
        ui::Text* target{ static_cast<ui::Text*>(sender) };
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
                    data.saveInteger("audio.status", 0);
                    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
                    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0);
                } else {
                    target->setString(audioOn);
                    data.saveInteger("audio.status", 1);
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
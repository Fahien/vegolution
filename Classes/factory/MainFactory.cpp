#include "MainFactory.h"
#include "scene/GameScene.h"
#include "scene/SettingsScene.h"

USING_NS_CC;

MainFactory::MainFactory(DataManager* data)
        : visibleSize_{ Director::getInstance()->getVisibleSize() }
        , center_{ visibleSize_.width / 2.0f + Director::getInstance()->getVisibleOrigin().x,
                   visibleSize_.height / 2.0f + Director::getInstance()->getVisibleOrigin().y }
        , shadowOffset_{ 0.0f, -4.0f }
        , shadowBlur_{ 8 }
        , textContentSize_{ 64.0f, 24.0f }
        , layout_{ ui::LinearLayoutParameter::create() }
{
    log("Creating MainFactory");

    // Load and cache the menu sprite frame
    // SpriteFrameCache* cacher {SpriteFrameCache::getInstance()};
    // cacher->addSpriteFramesWithFile("menu.plist");

    // Set text settings
    log("Setting text");
    fontPath_ = data->getString("font.path");
    fontSize_ = static_cast<float>(data->getInteger("font.size"));

    layout_->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    layout_->setMargin(ui::Margin{0.0f, 0.0f, 0.0f, 8.0f});

    // Set menu settings
    log("Setting menu");
    menuSize_ = Size{64.0f, 64.0f};

    createBackground();
    createPlayText(data->getString("main.play"), data);
    createSettingsText(data->getString("main.settings"), data);
    createQuitText(data->getString("main.quit"));
    createMenu();
    createBoard();
}

MainFactory::~MainFactory() {
    log("Destructing MainFactory");
}

void MainFactory::createBackground() {
    if (background_ == nullptr) {
        log("Creating background");
        background_ = Sprite::create("main/background.png");
        float scaleX{visibleSize_.width / background_->getContentSize().width};
        background_->setScaleX(scaleX);
        background_->setPosition(center_);
    }
}

void MainFactory::createPlayText(std::string name, DataManager* data) {
    if (play_ == nullptr) {
        log("Creating play");
        // Get localized string
        play_ = ui::Text::create(name, fontPath_, fontSize_);
        play_->setContentSize(textContentSize_);
        play_->setPositionX(-textContentSize_.width / 2);
        play_->setLayoutParameter(layout_);
        play_->enableShadow(Color4B::BLACK, shadowOffset_, shadowBlur_);
        play_->addTouchEventListener([data](Ref *sender, ui::Widget::TouchEventType type) {
            ui::Text *target{static_cast<ui::Text *>(sender)};
            Scene *scene{nullptr};
            TransitionFade *transition{nullptr};
            switch (type) {
                case ui::Widget::TouchEventType::BEGAN :
                    target->runAction(ScaleTo::create(0.125f, 1.25f));
                    break;
                case ui::Widget::TouchEventType::ENDED :
                    target->runAction(ScaleTo::create(0.125f, 1.0f));
                    scene = GameScene::create(data);
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
    play_->setTouchEnabled(true);
}

void MainFactory::createSettingsText(std::string name, DataManager* data) {
    if (settings_ == nullptr) {
        log("Creating settings");
        // Get localized string
        settings_ = ui::Text::create(name, fontPath_, fontSize_);
        settings_->setContentSize(textContentSize_);
        settings_->setPositionX(-textContentSize_.width / 2);
        settings_->setLayoutParameter(layout_);
        settings_->enableShadow(Color4B::BLACK, shadowOffset_, shadowBlur_);
        settings_->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type) {
            ui::Text *target{static_cast<ui::Text *>(sender)};
            SettingsScene *scene{nullptr};
            TransitionFade* transition{nullptr};
            switch (type) {
                case ui::Widget::TouchEventType::BEGAN :
                    target->runAction(ScaleTo::create(0.125f, 1.25f));
                    break;
                case ui::Widget::TouchEventType::ENDED :
                    target->runAction(ScaleTo::create(0.125f, 1.0f));
                    scene = SettingsScene::create(data);
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
    settings_->setTouchEnabled(true);
}

void MainFactory::createQuitText(std::string name) {
    if (quit_ == nullptr) {
        log("Creating quit");
        quit_ = ui::Text::create(name, fontPath_, fontSize_);
        quit_->setContentSize(textContentSize_);
        quit_->setPositionX(-textContentSize_.width / 2);
        quit_->enableShadow(Color4B::BLACK, shadowOffset_, shadowBlur_);
        quit_->setLayoutParameter(layout_);
        quit_->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type) {
            ui::Text *target{static_cast<ui::Text *>(sender)};
            switch (type) {
                case ui::Widget::TouchEventType::BEGAN :
                    target->runAction(ScaleTo::create(0.125f, 1.25f));
                    break;
                case ui::Widget::TouchEventType::ENDED :
                    target->runAction(ScaleTo::create(0.125f, 1.0f));
                    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                    exit(0);
#endif
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
    quit_->setTouchEnabled(true);
}

void MainFactory::createMenu() {
    if (menu_ == nullptr) {
        log("Creating menu");
        menu_ = ui::Layout::create();
        menu_->setContentSize(menuSize_);
        menu_->setPositionX(center_.x - menuSize_.width / 2);
        menu_->setPositionY(center_.y - menuSize_.height / 2);
        menu_->setLayoutType(ui::Layout::Type::VERTICAL);
        // Get the play text
        menu_->addChild(play_);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        // Get the settings text
        menu_->addChild(settings_);
#endif
        // Get the quit text
        menu_->addChild(quit_);
    }
}

void MainFactory::createBoard() {
    if (board_ == nullptr) {
        log("Creating board");
        board_ = Sprite::create("misc/board.png");
        float scaleX{visibleSize_.width / board_->getContentSize().width};
        board_->setScaleX(scaleX);
        board_->setPosition(center_);
        board_->setGlobalZOrder(4);
    }
}

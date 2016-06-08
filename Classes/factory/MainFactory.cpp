#include "MainFactory.h"
#include "scene/GameScene.h"
#include "scene/SettingsScene.h"

USING_NS_CC;

MainFactory::MainFactory(DataManager& data, TextFactory& textFactory)
        : background_{ Sprite::create("main/background.png") }
        , play_{ nullptr }
        , settings_{ nullptr }
        , quit_{ nullptr }
        , menu_{ ui::Layout::create() }
        , board_{ Sprite::create("misc/board.png") }
{
    log("Creating MainFactory");

    // Load and cache the menu sprite frame
    // SpriteFrameCache* cacher {SpriteFrameCache::getInstance()};
    // cacher->addSpriteFramesWithFile("menu.plist");

    Size visibleSize{ textFactory.getVisibleSize() };
    Vec2 center{ textFactory.getCenter() };

    createBackground(textFactory.getVisibleSize(), center);
    createPlayText(data.getString("main.play"), data, textFactory);
    createSettingsText(data.getString("main.settings"), data, textFactory);
    createQuitText(data.getString("main.quit"), textFactory);
    createMenu(center);
    createBoard(visibleSize, center);
}

MainFactory::~MainFactory()
{
    log("Destructing MainFactory");
}

void MainFactory::createBackground(Size& visibleSize, Vec2& center)
{
    log("Creating background");
    float scaleX{ visibleSize.width / background_->getContentSize().width };
    background_->setScaleX(scaleX);
    background_->setPosition(center);
}

void MainFactory::createPlayText(std::string name, DataManager& data, TextFactory& textFactory)
{
    log("Creating play");
    play_ = textFactory.createText(name);
    play_->setTouchEnabled(true);
    play_->addTouchEventListener([ &data, &textFactory ](Ref* sender, ui::Widget::TouchEventType type) {
        ui::Text* target{ static_cast<ui::Text*>(sender) };
        Scene* scene{ nullptr };
        TransitionFade* transition{ nullptr };
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN :
                target->runAction(ScaleTo::create(0.125f, 1.25f));
                break;
            case ui::Widget::TouchEventType::ENDED :
                target->runAction(ScaleTo::create(0.125f, 1.0f));
                // Create the GameScene
                scene = GameScene::create(data, textFactory);
                // Create a transition
                transition = TransitionFade::create(0.5f, scene, Color3B::BLACK);
                // Replace the MenuScene with the GameScene
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

void MainFactory::createSettingsText(std::string name, DataManager& data, TextFactory& textFactory)
{
    log("Creating settings");
    // Get localized string
    settings_ = textFactory.createText(name);
    settings_->setTouchEnabled(true);
    settings_->addTouchEventListener([ &data, &textFactory ](Ref* sender, ui::Widget::TouchEventType type) {
        ui::Text* target{ static_cast<ui::Text*>(sender) };
        SettingsScene* scene{ nullptr };
        TransitionFade* transition{ nullptr };
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN :
                target->runAction(ScaleTo::create(0.125f, 1.25f));
                break;
            case ui::Widget::TouchEventType::ENDED :
                target->runAction(ScaleTo::create(0.125f, 1.0f));
                scene = SettingsScene::create(data, textFactory);
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

void MainFactory::createQuitText(std::string name, TextFactory& textFactory)
{
    log("Creating quit");
    quit_ = textFactory.createText(name);
    quit_->setTouchEnabled(true);
    quit_->addTouchEventListener([ & ](Ref* sender, ui::Widget::TouchEventType type) {
        ui::Text* target{ static_cast<ui::Text*>(sender) };
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

void MainFactory::createMenu(Vec2& center)
{
    log("Creating menu");
    Size size{ 64.0f, 64.0f };
    menu_->setContentSize(size);
    menu_->setPositionX(center.x - size.width / 2.0f);
    menu_->setPositionY(center.y - size.height / 2.0f);
    menu_->setLayoutType(ui::Layout::Type::VERTICAL);
    // Get the play text
    menu_->addChild(play_);
    // Get the settings text
    menu_->addChild(settings_);
    // Get the quit text
    menu_->addChild(quit_);
}

void MainFactory::createBoard(Size& visibleSize, Vec2& center)
{
    log("Creating board");
    float scaleX{ visibleSize.width / board_->getContentSize().width };
    board_->setScaleX(scaleX);
    board_->setPosition(center);
    board_->setGlobalZOrder(4);
}

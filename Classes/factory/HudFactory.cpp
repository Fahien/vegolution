#include "HudFactory.h"
#include "scene/MainScene.h"

USING_NS_CC;

HudFactory::HudFactory(DataManager& data, TextFactory& textFactory)
        : quit_{ nullptr }
        , score_{ nullptr }
        , rightGear_{ nullptr }
        , leftGear_{ nullptr }
{
    log("Creating HudFactory");
    createScoreText(textFactory);
    createQuitText(data, textFactory);
}

HudFactory::~HudFactory()
{
    log("Destructing HudFactory");
    // Release retained resources
    quit_->release();
}

void HudFactory::createQuitText(DataManager& data, TextFactory& textFactory)
{
    log("Creating quit text");
    std::string name{ "Quit" };
    quit_ = textFactory.createText(name);
    quit_->retain();
    quit_->setPosition(textFactory.getCenter());
    quit_->setTouchEnabled(true);
    quit_->addTouchEventListener(
            [ &data, &textFactory ](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
                ui::Text* target{ static_cast<ui::Text*>(sender) };
                Scene* scene{ nullptr };
                switch (type) {
                    case ui::Widget::TouchEventType::BEGAN :
                        target->runAction(ScaleTo::create(0.125f, 1.25f));
                        break;
                    case ui::Widget::TouchEventType::ENDED :
                        target->runAction(ScaleTo::create(0.125f, 1.0f));
                        scene = MainScene::create(data, textFactory);
                        Director::getInstance()->replaceScene(scene);
                        Director::getInstance()->resume();
                        target->setTouchEnabled(false);
                        break;
                    case ui::Widget::TouchEventType::CANCELED :
                        target->runAction(ScaleTo::create(0.125f, 1.0f));
                        break;
                    default:
                        break;
                }
                return true;
            }
    );
}

void HudFactory::createScoreText(TextFactory& textFactory)
{
    log("Creating score text");
    std::string text{ "0" };
    score_ = textFactory.createText(text);
    score_->setAnchorPoint(Vec2{ 0.5f, 1.0f });
    Vec2 position{ textFactory.getCenter().x, textFactory.getVisibleSize().height };
    score_->setPosition(position);
}

LeftGear* HudFactory::getLeftGear(MainActor* actor, Size& visibleSize)
{
    if (leftGear_ == nullptr) {
        log("Creating left gear");
        leftGear_ = LeftGear::create();
        // Add this vehicle listener to the actor
        actor->addGear(leftGear_);
        // Update texture now according to the vehicle
        leftGear_->onVehicleChange(actor->getVehicle());
        leftGear_->setPositionY(visibleSize.height);
        leftGear_->setAnchorPoint(Vec2{ 0.0f, 1.0f });
        leftGear_->setTouchEnabled(true);
        leftGear_->addTouchEventListener(
                [ actor ](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
                    if (type == ui::Widget::TouchEventType::ENDED) { actor->switchVehicle(); }
                    return true;
                }
        );
    }
    return leftGear_;
}

RightGear* HudFactory::getRightGear(MainActor* actor, Size& visibleSize)
{
    if (rightGear_ == nullptr) {
        log("Creating right gear");
        rightGear_ = RightGear::create();
        // Set vehicle health listener
        actor->setRightGear(rightGear_);
        // Add to vehicle change listener
        actor->addGear(rightGear_);
        std::string filename{ "hud/right-gear.png" };
        rightGear_->loadTexture(filename);
        rightGear_->setPositionY(visibleSize.height);
        rightGear_->setPositionX(visibleSize.width);
        rightGear_->setAnchorPoint(Vec2{ 1.0f, 1.0f });
        rightGear_->setTouchEnabled(true);
        // Load health
        filename = std::string{ "hud/right-gear-health.png" };
        Sprite* health{ Sprite::create(filename) };
        Size size{ rightGear_->getContentSize() };
        health->setPosition(Vec2{ size.width / 6.0f, size.height });
        health->setAnchorPoint(Vec2{ 0.0f, 1.0f });
        rightGear_->setHealthBar(health);
        rightGear_->addChild(health, -2);

        // Load energy
        filename = std::string{ "hud/right-gear-energy.png" };
        Sprite* energy{ Sprite::create(filename) };
        energy->setPosition(Vec2{ size.width, size.height });
        energy->setAnchorPoint(Vec2{ 1.0f, 1.0f });
        rightGear_->addChild(energy, -1);
    }
    return rightGear_;
}

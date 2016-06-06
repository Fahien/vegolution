#include "HudFactory.h"

USING_NS_CC;

HudFactory::HudFactory(TextFactory &textFactory)
        : textFactory_{ textFactory }
        , score_{ nullptr }
        , rightGear_{ nullptr }
        , leftGear_{ nullptr }
{
    log("Creating HudFactory");
}

HudFactory::~HudFactory()
{ }

cocos2d::ui::Text *HudFactory::getScoreText()
{
    if (score_ == nullptr) {
        log("Creating score text");
        std::string text{ "0" };
        score_ = textFactory_.createText(text);
        score_->setAnchorPoint(Vec2{0.5f, 1.0f});
        Vec2 position{ textFactory_.getCenter().x, textFactory_.getVisibleSize().height };
        score_->setPosition(position);
    }
    return score_;
}

LeftGear *HudFactory::getLeftGear(MainActor *actor)
{
    if (leftGear_ == nullptr) {
        log("Creating left gear");
        leftGear_ = LeftGear::create();
        // Add this vehicle listener to the actor
        actor->addGear(leftGear_);
        // Update texture now according to the vehicle
        leftGear_->onVehicleChange(actor->getVehicle());
        leftGear_->setPositionY(textFactory_.getVisibleSize().height);
        leftGear_->setAnchorPoint(Vec2{ 0.0f, 1.0f });
        leftGear_->setTouchEnabled(true);
        leftGear_->addTouchEventListener(
                [ actor ](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
                    if (type == ui::Widget::TouchEventType::ENDED) { actor->switchVehicle(); }
                    return true;
                }
        );
    }
    return leftGear_;
}

RightGear *HudFactory::getRightGear(MainActor *actor)
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
        Size visibleSize{ textFactory_.getVisibleSize() };
        rightGear_->setPositionY(visibleSize.height);
        rightGear_->setPositionX(visibleSize.width);
        rightGear_->setAnchorPoint(Vec2{ 1.0f, 1.0f });
        rightGear_->setTouchEnabled(true);
        // Load health
        filename = std::string{ "hud/right-gear-health.png" };
        Sprite *health{ Sprite::create(filename) };
        Size size{ rightGear_->getContentSize() };
        health->setPosition(Vec2{ size.width / 6.0f, size.height });
        health->setAnchorPoint(Vec2{ 0.0f, 1.0f });
        rightGear_->setHealthBar(health);
        rightGear_->addChild(health, -2);

        // Load energy
        filename = std::string{ "hud/right-gear-energy.png" };
        Sprite *energy{ Sprite::create(filename) };
        energy->setPosition(Vec2{ size.width, size.height });
        energy->setAnchorPoint(Vec2{ 1.0f, 1.0f });
        rightGear_->addChild(energy, -1);
    }
    return rightGear_;
}

#include "HudFactory.h"

USING_NS_CC;

HudFactory::HudFactory(const Size visibleSize)
        : visibleSize_{ visibleSize }
        , rightGear_{ nullptr }
        , leftGear_{ nullptr }
{}

HudFactory::~HudFactory(){}

LeftGear* HudFactory::getLeftGear(MainActor* actor)
{
    if (leftGear_ == nullptr) {
        cocos2d::log("Creating left gear");
        leftGear_ = LeftGear::create();
        // Add this vehicle listener to the actor
        actor->addGear(leftGear_);
        // Update texture now according to the vehicle
        leftGear_->onVehicleChange(actor->getVehicle());
        leftGear_->setPositionY(visibleSize_.height);
        leftGear_->setAnchorPoint(Vec2{ 0.0f, 1.0f });
        leftGear_->setTouchEnabled(true);
        leftGear_->addTouchEventListener(
                [actor](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
                    if (type == ui::Widget::TouchEventType::ENDED) { actor->switchVehicle(); }
                    return true;
                }
        );
    }
    return leftGear_;
}

RightGear* HudFactory::getRightGear(MainActor* actor)
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
        rightGear_->setPositionY(visibleSize_.height);
        rightGear_->setPositionX(visibleSize_.width);
        rightGear_->setAnchorPoint(Vec2{ 1.0f, 1.0f });
        rightGear_->setTouchEnabled(true);
        rightGear_->addTouchEventListener(
                [](Ref* sender, ui::Widget::TouchEventType type) {
                    if (type == ui::Widget::TouchEventType::ENDED) {
                        log("Should open modal");
                    }
                    return true;
                }
        );
        // Load health
        filename = std::string{ "hud/right-gear-health.png" };
        Sprite* health{ Sprite::create(filename) };
        Size size{ rightGear_->getContentSize() };
        health->setPosition(Vec2{ 0.0f, size.height });
        health->setAnchorPoint(Vec2{ 0.0f, 1.0f });
        rightGear_->setHealthBar(health);
        rightGear_->addChild(health, -2);

        // Load energy
        filename = std::string{ "hud/right-gear-energy.png" };
        Sprite* energy{ Sprite::create(filename) };
        energy->setPosition(Vec2{ size.width / 2.0f, size.height });
        energy->setAnchorPoint(Vec2{ 0.0f, 1.0f });
        rightGear_->addChild(energy, -1);
    }
    return rightGear_;
}

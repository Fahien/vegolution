#include "GameController.h"

USING_NS_CC;

GameController::GameController(MainActor* actor, Parallax* parallax)
: actor_ {actor}
, parallax_ {parallax}
{
    onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 location {touch->getLocationInView()};
        Size visibleSize {Director::getInstance()->getVisibleSize()};
        Vec2 center {visibleSize.width / 4, visibleSize.height / 2};
        Vec2 force {128.0f, 0.0f};
        if (location.x < center.x) {
            force.x = 0.0f;
            Vehicle* vehicle {this->actor_->getVehicles()[0]};
            this->actor_->setVehicle(vehicle);
        }
        // Move actor
        this->actor_->getPhysicsBody()->setVelocity(force);
        return true;
    };
}

GameController* GameController::create(MainActor* actor, Parallax* parallax)
{
    GameController* controller {new (std::nothrow) GameController{actor, parallax}};

    // Initialize
    if (controller && controller->init()) {
        controller->autorelease();
        return controller;
    }
    
    CC_SAFE_DELETE(controller);
    return controller;
}

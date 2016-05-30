#include "GameController.h"

USING_NS_CC;

GameController::GameController(MainActor* actor, Parallax* parallax)
	: actor_{ actor }
	, parallax_{ parallax }
{
    onTouchBegan = [this](Touch* touch, Event* event) {
		Vec2 location{ touch->getLocationInView() };
		Size visibleSize{ Director::getInstance()->getVisibleSize() };
		Vec2 center{ visibleSize.width / 4.0f, visibleSize.height / 2.0f };
		Vec2 velocity{ actor_->getVehicle()->getVelocity() * 32.0f, 0.0f };
        if (location.x < center.x) {
			return true;
        }
        // Move actor
        actor_->getPhysicsBody()->setVelocity(velocity);
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

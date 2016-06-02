#include "GameController.h"

USING_NS_CC;

// Constructor
GameController::GameController(MainActor* actor, const Size contentSize)
	: actor_{ actor }
	, centerX_{ contentSize.width / 4.0f }
	, centerY_{ contentSize.height / 2.0f }
{
    onTouchBegan = [this](Touch* touch, Event* event) {
		actor_->setMoving(touch->getLocationInView().x > centerX_);
		Vec2 location{ touch->getLocation() };
		location.y -= centerY_;
		actor_->tap(location);
        return true;
    };
}

// Create method
GameController* GameController::create(MainActor* actor, const Size contentSize)
{
    GameController* controller {new (std::nothrow) GameController{actor, contentSize }}; // Construct
    if (controller && controller->init()) { controller->autorelease(); } // Initialize
	else { CC_SAFE_DELETE(controller); } // Error
    return controller;
}

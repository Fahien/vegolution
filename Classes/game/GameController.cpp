#include "GameController.h"

USING_NS_CC;

// Constructor
GameController::GameController(MainActor* actor, float centerX)
	: actor_{ actor }
	, centerX_{ centerX / 2.0f }
{
    onTouchBegan = [this](Touch* touch, Event* event) {
		Vec2 location{ touch->getLocationInView() };
		actor_->setMoving(location.x > centerX_);
        return true;
    };
}

// Create method
GameController* GameController::create(MainActor* actor, float centerX)
{
	// Construct
    GameController* controller {new (std::nothrow) GameController{actor, centerX}};

    // Initialize
    if (controller && controller->init()) {
        controller->autorelease();
        return controller;
    }
    
	// Error
    CC_SAFE_DELETE(controller);
    return controller;
}

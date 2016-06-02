#include "Actor.h"

USING_NS_CC;

Actor* Actor::create(std::string name)
{
    // Construct
	Actor* actor{ new (std::nothrow) Actor{} };

    // Initialize
    if (actor && actor->initWithFile(name)) {
        actor->autorelease();
        // Hook the template method for creating physics body
        actor->createPhysicsBody(name);
        return actor;
    }

	// Error
    CC_SAFE_DELETE(actor);
    return nullptr;
}

void Actor::createPhysicsBody(std::string name)
{
    // Create a material
	PhysicsMaterial material{ 0.0f, 0.8f, 0.0f };
    // Create the body
    physicsBody_ = PhysicsBody::createBox(getContentSize(), material);
    physicsBody_->setName(name);
    addComponent(physicsBody_);
}

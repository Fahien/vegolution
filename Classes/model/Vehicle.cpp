#include "Vehicle.h"

USING_NS_CC;

// Constructor
Vehicle::Vehicle(int velocity, int health, int delay, Bullet* bullet, Vec2 offset)
	: velocity_ { velocity }
	, health_{ health }
	, delay_{ delay }
	, bullet_{ bullet }
	, offset_{ offset }
	, action_{ nullptr }
{
	// Create new shot action
	CallFunc* shot{ CallFunc::create([this]() {
		log("Shot bullet");
		if (bullet_->getParent() != nullptr) bullet_->removeFromParent();
		getParent()->addChild(bullet_);
		bullet_->setPosition(getPositionX() + offset_.x, getPositionY() + offset_.y);
		bullet_->getPhysicsBody()->setVelocity(Vec2{ bullet_->getVelocity() * 64.0f, 0.0f });
		DelayTime* time{ DelayTime::create(delay_ * 0.25f) };
		CallFunc* remove{ CallFunc::create([this]() { if (bullet_->getParent() != nullptr) bullet_->removeFromParent(); }) };
		Sequence* sequence{ Sequence::createWithTwoActions(time, remove) };
		bullet_->runAction(sequence);
	}) };
	// Wait
	DelayTime* time{ DelayTime::create(delay_ * 0.25f) };
	Sequence* sequence{ Sequence::createWithTwoActions(shot, time) };
	action_ = RepeatForever::create(sequence);
	runAction(action_);
}

// Create method
Vehicle* Vehicle::create(std::string vehiclename, int velocity, int health, int delay, Bullet* bullet, Vec2 offset)
{
    // Construct
    Vehicle* vehicle {new (std::nothrow) Vehicle{velocity, health, delay, bullet, offset}};

	std::string filename{ StringUtils::format("vehicle/%s/%s.png", vehiclename.c_str(), vehiclename.c_str()) };
    // Initialize
    if (vehicle && vehicle->initWithFile(filename)) {
        vehicle->setName(vehiclename);
        vehicle->createPhysicsBody(vehiclename);
        vehicle->physicsBody_->setDynamic(false);
        // Set MainActor category
        vehicle->physicsBody_->setCategoryBitmask(1);
        // Collide with Terrain and Enemies
        vehicle->physicsBody_->setCollisionBitmask(6);
        return vehicle;
    }

	// Error
    CC_SAFE_DELETE(vehicle);
    return nullptr;
}

#include "Vehicle.h"

USING_NS_CC;

// Constructor
Vehicle::Vehicle(int healthMax, int velocity, int delay, Bullet* bullet, Vec2 offset)
	: healthMax_{ healthMax }
	, health_{ static_cast<float>(healthMax) }
	, velocity_ { static_cast<float>(velocity) }
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
		float velocityX{ physicsBody_->getVelocity().x + bullet_->getVelocity() };
		bullet_->getPhysicsBody()->setVelocity(Vec2{velocityX, 0.0f });
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
Vehicle* Vehicle::create(std::string vehiclename, int healthMax, int velocity, int delay, Bullet* bullet, Vec2 offset, bool gravity)
{
    // Construct
    Vehicle* vehicle {new (std::nothrow) Vehicle{ healthMax, velocity, delay, bullet, offset}};

	std::string filename{ StringUtils::format("vehicle/%s/%s.png", vehiclename.c_str(), vehiclename.c_str()) };
    // Initialize
    if (vehicle && vehicle->initWithFile(filename)) {
        vehicle->setName(vehiclename);
        vehicle->createPhysicsBody(vehiclename);
		// Set gravity enable
		vehicle->physicsBody_->setGravityEnable(gravity);
		// Initialize with no dynamic
        vehicle->physicsBody_->setDynamic(false);
		// Set group 1
		vehicle->physicsBody_->setGroup(1);
        // Set MainActor category
        vehicle->physicsBody_->setCategoryBitmask(1);
        // Collide with Terrain, Enemies and Enemy bullets
        vehicle->physicsBody_->setCollisionBitmask(22);
		// Contact test with enemy bullets
		vehicle->physicsBody_->setContactTestBitmask(16);
        return vehicle;
    }

	// Error
    CC_SAFE_DELETE(vehicle);
    return nullptr;
}

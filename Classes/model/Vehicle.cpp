#include "Vehicle.h"

USING_NS_CC;

// Constructor
Vehicle::Vehicle(int velocity, int health, int delay, Bullet* bullet)
	: velocity_ { velocity }
	, health_{ health }
	, delay_{ delay }
	, bullet_{ bullet }
{
	// Create new shot action
	CallFunc* shot{ CallFunc::create([this]() {
		log("Shot bullet");
		if (bullet_->getParent() != nullptr) bullet_->removeFromParent();
		addChild(bullet_);
		Size size{ getContentSize() };
		bullet_->setPosition(size.width, size.height / 2.0f);
		bullet_->getPhysicsBody()->setVelocity(Vec2{ bullet_->getVelocity() * 64.0f, 0.0f });
	}) };
	// Wait
	DelayTime* time{ DelayTime::create(delay_ * 0.25f) };
	Sequence* sequence{ Sequence::create(shot, time, nullptr) };
	runAction(RepeatForever::create(sequence));
}

// Create method
Vehicle* Vehicle::create(std::string vehiclename, int velocity, int health, int delay, Bullet* bullet)
{
    // Construct
    Vehicle* vehicle {new (std::nothrow) Vehicle{velocity, health, delay, bullet}};

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

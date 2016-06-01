#include "Bullet.h"

USING_NS_CC;

// Constructor
Bullet::Bullet(int damage, int velocity)
	: damage_{ damage }
	, velocity_{ velocity }
{
	log("Creating a bullet");
}

// Create method
Bullet* Bullet::create(std::string name, int damage, int velocity)
{
    // Construct
    Bullet* bullet {new (std::nothrow) Bullet{damage, velocity}};

	std::string filename{ StringUtils::format("bullet/%s.png", name.c_str(), name.c_str()) };
    // Initialize
    if (bullet && bullet->initWithFile(filename)) {
        bullet->setName(name);
		bullet->setTag(8);
        bullet->createPhysicsBody(name);
		bullet->physicsBody_->setGravityEnable(false);
		bullet->physicsBody_->setRotationEnable(false);
		// Contact test
		bullet->physicsBody_->setContactTestBitmask(4);
		// Player bullet category is 8
		bullet->physicsBody_->setCategoryBitmask(8);
		// Collide with enemies
        bullet->physicsBody_->setCollisionBitmask(4);
        return bullet;
    }

	// Error
    CC_SAFE_DELETE(bullet);
    return nullptr;
}
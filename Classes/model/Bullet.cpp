#include "Bullet.h"

USING_NS_CC;

Bullet::Bullet(int damage, float velocity)
	: damage_{ damage }
	, velocity_{ velocity }
{}

Bullet* Bullet::create(std::string name, int damage, float velocity)
{
    // Construct
    Bullet* bullet {new (std::nothrow) Bullet{damage, velocity}};
    // Initialize
	std::string filename{ StringUtils::format("bullet/%s.png", name.c_str()) };
    if (bullet && bullet->initWithFile(filename)) {
        bullet->setName(name);
		bullet->setTag(8);
        bullet->createPhysicsBody(name);
		bullet->physicsBody_->setAngularVelocityLimit(0.0f);
		// Set group 2
		bullet->physicsBody_->setGroup(2);
		// No gravity and no rotation
		bullet->physicsBody_->setGravityEnable(false);
		// Contact test with enemy category
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

void Bullet::remove()
{
	physicsBody_->resetForces();
	physicsBody_->setVelocity(Vec2::ZERO);
	physicsBody_->setAngularVelocity(0.0f);
	if (getParent()) { removeFromParentAndCleanup(true); }
}
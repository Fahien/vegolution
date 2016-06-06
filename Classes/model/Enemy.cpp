#include "Enemy.h"

USING_NS_CC;

Enemy::Enemy(int healthMax, int velocity, int delay, int offsetY, Bullet* bullet)
	: healthMax_{ healthMax }
	, health_ { healthMax }
	, velocity_{ static_cast<float>(velocity) }
	, delay_{ delay }
	, offsetY_{ static_cast<float>(offsetY) }
	, bullet_{ bullet }
{
	if (!bullet) return;	// End if no shotting
	// Adjust group
	bullet->getPhysicsBody()->setGroup(1);
	// Enemy bullet category is 8
	bullet->getPhysicsBody()->setCategoryBitmask(16);
	// Collide with the main actor
	bullet->getPhysicsBody()->setCollisionBitmask(1);
	// Contact test with the main actor
	bullet->getPhysicsBody()->setContactTestBitmask(1);

	CallFunc* shot{ CallFunc::create([this]() {
		if (bullet_->getParent()) { bullet_->removeFromParent(); }
		// Add the bullet to the scene (parent)
		getParent()->addChild(bullet_);
		bullet_->setPosition(getPosition());
		bullet_->setRotation(180.0f); // Shot the main actor
		bullet_->getPhysicsBody()->setVelocity(Vec2::ZERO);
		bullet_->getPhysicsBody()->applyImpulse(Vec2{ bullet_->getVelocity(), 0.0f });
		DelayTime* time{ DelayTime::create(delay_ * 0.25f) };
		CallFunc* remove{ CallFunc::create([this]() { bullet_->remove(); }) };
		Sequence* sequence{ Sequence::createWithTwoActions(time, remove) };
		bullet_->runAction(sequence);
	}) };	// Shot bullet
	DelayTime* time{ DelayTime::create(delay_ * 0.25f) };	// And wait delay time
	Sequence* sequence{ Sequence::createWithTwoActions(shot, time) };
	runAction(RepeatForever::create(sequence));
}

Enemy::~Enemy()
{
	bullet_->release();
}

Enemy* Enemy::create(std::string name, int healthMax, int velocity, int delay, Bullet* bullet, bool gravity, int y)
{
	// Construct
	Enemy* enemy{ new (std::nothrow) Enemy{ healthMax, velocity, delay, y, bullet } };

	std::string filename{ StringUtils::format("enemy/%s/%s0.png", name.c_str(), name.c_str()) };
    // Initialize                                                                                             
    if (enemy && enemy->initWithFile(filename)) {
		enemy->setName(name);
        // Hook the template method for creating physics body
        enemy->createPhysicsBody(name);
		// Set gravity
		enemy->physicsBody_->setGravityEnable(gravity);
		// Set velocity and angular limit
		enemy->physicsBody_->setAngularVelocityLimit(0.0f);
		// Set group 2
		enemy->physicsBody_->setGroup(2);
		// Do not rotate
		enemy->physicsBody_->setRotationEnable(false);
		// Contact test
		enemy->physicsBody_->setContactTestBitmask(8);
		// Enemy category is 4
        enemy->physicsBody_->setCategoryBitmask(4);
		// Collide with main actor, terrain and player bullet
        enemy->physicsBody_->setCollisionBitmask(11);
    } else { CC_SAFE_DELETE(enemy); }
	return enemy;
}

void Enemy::spawn(Vec2 position)
{
	position.y += offsetY_;
	log("Spawning %s at (%.2f, %.2f)", getName().c_str(), position.x, position.y);
	setPosition(position);
}

void Enemy::remove()
{
	pause(); // Pause all actions
	health_ = healthMax_; // Reset health
	// Reset physics
	physicsBody_->resetForces();
	physicsBody_->setVelocity(Vec2::ZERO);
	physicsBody_->setAngularVelocity(0.0f);
	// Remove from the scene
	if (getParent()) { removeFromParentAndCleanup(false); }
}

void Enemy::update(float delta)
{
	// Walk to the main actor
	if (physicsBody_->getVelocity().x > -velocity_) {
		physicsBody_->applyForce(Vec2{ -velocity_, 0.0f });
	}
}

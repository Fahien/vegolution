#include "Enemy.h"

USING_NS_CC;

// Constructor
Enemy::Enemy(int healthMax, int velocity, int delay, Bullet* bullet)
	: healthMax_{ healthMax }
	, health_ { healthMax }
	, velocity_{ static_cast<float>(velocity) }
	, delay_{ delay }
	, bullet_{ bullet }
{
	// Rotate
	bullet_->setRotation(180.0f);
	// Adjust group
	bullet_->getPhysicsBody()->setGroup(1);
	// Enemy bullet category is 8
	bullet_->getPhysicsBody()->setCategoryBitmask(16);
	// Collide with the main actor
	bullet_->getPhysicsBody()->setCollisionBitmask(1);
	// Contact test with the main actor
	bullet_->getPhysicsBody()->setContactTestBitmask(1);
}

// Destructor
Enemy::~Enemy()
{
	log("Releasing enemy bullet");
	bullet_->release();
}

// Create method
Enemy* Enemy::create(std::string name, int healthMax, int velocity, int delay, Bullet* bullet)
{
	// Construct
	Enemy* enemy{ new (std::nothrow) Enemy{ healthMax, velocity, delay, bullet } };

	std::string filename{ StringUtils::format("enemy/%s/%s.png", name.c_str(), name.c_str()) };
    // Initialize                                                                                             
    if (enemy && enemy->initWithFile(filename)) {
		enemy->setName(name);
        // Hook the template method for creating physics body
        enemy->createPhysicsBody(name);
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
        return enemy;
    }

	CC_SAFE_DELETE(enemy);
    return nullptr;
}

void Enemy::scheduleShot()
{
	// Create new shot action
	CallFunc* shot{ CallFunc::create([this]() {
		log("Shot enemy bullet");
		if (bullet_->getParent() != nullptr) bullet_->removeFromParent();
		getParent()->addChild(bullet_);
		bullet_->setPosition(getPosition());
		bullet_->getPhysicsBody()->setVelocity(Vec2{ -bullet_->getVelocity(), 0.0f });
		DelayTime* time{ DelayTime::create(delay_ * 0.25f) };
		CallFunc* remove{ CallFunc::create([this]() { if (bullet_->getParent() != nullptr) bullet_->removeFromParent(); }) };
		Sequence* sequence{ Sequence::createWithTwoActions(time, remove) };
		bullet_->runAction(sequence);
	}) };
	// Wait
	DelayTime* time{ DelayTime::create(delay_ * 0.25f) };
	Sequence* sequence{ Sequence::createWithTwoActions(shot, time) };
	runAction(RepeatForever::create(sequence));
}

// Update method
void Enemy::update(float delta) {
	// Get current velocity
	Vec2 velocity{ physicsBody_->getVelocity() };
	// Get enemy velocity
	float enemyVelocity{ -velocity_};
	// If is not the max velocity
	if (velocity.x > enemyVelocity) {
		velocity.x += enemyVelocity * delta * 2.0f; // Increment
		if (velocity.x < enemyVelocity) {
			velocity.x = enemyVelocity;
		}
	}
	else {
		velocity.x -= enemyVelocity * delta * 2.0f; // Decrement
	}
	// Set the new velocity
	physicsBody_->setVelocity(velocity);
}
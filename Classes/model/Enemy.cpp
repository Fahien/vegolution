#include "Enemy.h"

USING_NS_CC;

// Constructor
Enemy::Enemy(int healthMax, int velocity, int delay, Bullet* bullet)
	: healthMax_{ healthMax }
	, health_ { healthMax }
	, velocity_{ velocity }
	, delay_{ delay }
	, bullet_{ bullet }
{}

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
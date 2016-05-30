#include "Enemy.h"

USING_NS_CC;

Enemy* Enemy::create(std::string name, int health, int velocity, int delay, Bullet* bullet)
{
	// Construct
    Enemy* enemy {new (std::nothrow) Enemy{health, velocity, delay, bullet}};

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

// Constructor
Enemy::Enemy(int health, int velocity, int delay, Bullet* bullet)
	: health_{ health }
	, velocity_{ velocity }
	, delay_{ delay }
	, bullet_{ bullet }
{}

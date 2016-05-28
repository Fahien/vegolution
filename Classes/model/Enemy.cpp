#include "Enemy.h"

USING_NS_CC;

Enemy* Enemy::create(std::string name)
{
	// Construct                                                                                              
    Enemy* enemy {new (std::nothrow) Enemy{}};                                                                
                                                                                                           
    // Initialize                                                                                             
    if (enemy && enemy->initWithFile(name)) {                                                                 
		enemy->setName(name);
        //enemy->autorelease();                                                                                 
        // Hook the template method for creating physics body                                                 
        enemy->createPhysicsBody(name);                                                                       
        enemy->getPhysicsBody()->setCategoryBitmask(4);
        enemy->getPhysicsBody()->setCollisionBitmask(3);
        return enemy;                                                                                         
    }                                                                                                         
                                                                                                           
    CC_SAFE_DELETE(enemy);                                                                                    
    return nullptr;
}

Enemy* Enemy::clone()
{
	Enemy* cloned {create(getName())};
	cloned->health_ = health_;
	return cloned;
}

#include "Bullet.h"

USING_NS_CC;

Bullet* Bullet::create(std::string name)
{
    // Construct
    Bullet* bullet {new (std::nothrow) Bullet{}};

    // Initialize
    if (bullet && bullet->initWithFile(name)) {
        bullet->setName(name);
        bullet->createPhysicsBody(name);
        bullet->getPhysicsBody()->setCollisionBitmask(5);
        return bullet;
    }

    CC_SAFE_DELETE(bullet);
    return nullptr;
}

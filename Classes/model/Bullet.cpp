#include "Bullet.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Bullet::Bullet(int damage, float velocity)
        : damage_{ damage }
        , velocity_{ velocity }
{ }

Bullet *Bullet::create(std::string name, int damage, float velocity)
{
    // Construct
    Bullet *bullet{ new(std::nothrow) Bullet{ damage, velocity }};
    // Initialize
    std::string filename{ StringUtils::format("bullet/%s.png", name.c_str()) };
    if (bullet && bullet->initWithFile(filename)) {
        // Load sound effect
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        std::string audioPath{ StringUtils::format("audio/%s.wav", name.c_str()) };
#else
        std::string audioPath{ StringUtils::format("audio/%s.ogg", name.c_str()) };
#endif
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(audioPath.c_str());
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

void Bullet::playEffect()
{
    // Play sound effect
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    std::string audioPath{ StringUtils::format("audio/%s.wav", getName().c_str()) };
#else
    std::string audioPath{ StringUtils::format("audio/%s.ogg", getName().c_str()) };
#endif
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(audioPath.c_str());
}
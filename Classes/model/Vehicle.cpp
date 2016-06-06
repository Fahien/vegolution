#include "Vehicle.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

// Constructor
Vehicle::Vehicle(int healthMax, int velocity, int delay, Bullet *bullet, Vec2 offset)
        : healthMax_{ healthMax }
        , health_{ static_cast<float>(healthMax) }
        , velocity_{ static_cast<float>(velocity) }
        , delay_{ delay }
        , bullet_{ bullet }
        , offset_{ offset }
        , tap_{ Vec2::ZERO }
        , specificUpdate_{ nullptr }
        , onTap_{ nullptr }
{
    scheduleUpdate();        // Adjusting velocity, rotation and specific updates
    if (!bullet_) return;    // End if no shot

    CallFunc *shot{ CallFunc::create([ this ]() {
        // Check for parent error
        if (bullet_->getParent() != nullptr) bullet_->removeFromParent();
        // Spawn bullet one level above
        getParent()->addChild(bullet_);
        bullet_->setPosition(getPositionX() + offset_.x, getPositionY() + offset_.y);
        bullet_->setRotation(0.0f);
        bullet_->getPhysicsBody()->setVelocity(Vec2::ZERO);
        bullet_->getPhysicsBody()->applyImpulse(Vec2{ bullet_->getVelocity(), 0.0f });
        DelayTime *time{ DelayTime::create(delay_ * 0.25f) };
        CallFunc *remove{ CallFunc::create([ this ]() { bullet_->remove(); }) };
        Sequence *sequence{ Sequence::createWithTwoActions(time, remove) };
        bullet_->runAction(sequence);
        // Play bullet sound
        bullet_->playEffect();
    }) };    // Shot the bullet
    DelayTime *time{ DelayTime::create(delay_ * 0.25f) };    // And wait delay time
    Sequence *sequence{ Sequence::createWithTwoActions(shot, time) };
    runAction(RepeatForever::create(sequence));
}

// Create method
Vehicle *Vehicle::create(std::string name, int healthMax, int velocity, int delay, Bullet *bullet, Vec2 offset,
                         bool gravity)
{
    // Construct
    Vehicle *vehicle{ new(std::nothrow) Vehicle{ healthMax, velocity, delay, bullet, offset }};

    std::string filename{ StringUtils::format("vehicle/%s/%s0.png", name.c_str(), name.c_str()) };
    // Initialize
    if (vehicle && vehicle->initWithFile(filename)) {
        // Load sound effect
        std::string audioPath{ StringUtils::format("audio/%s.ogg", name.c_str()) };
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(audioPath.c_str());
        // Set the name
        vehicle->setName(name);
        vehicle->createPhysicsBody(name);
        // Set gravity enable
        vehicle->physicsBody_->setGravityEnable(gravity);
        // Initialize with no dynamic
        vehicle->physicsBody_->setDynamic(false);
        // Limit velocity
        vehicle->physicsBody_->setVelocityLimit(vehicle->velocity_);
        vehicle->physicsBody_->setAngularVelocityLimit(0.0f);
        // Do not rotate
        // Set group 1
        vehicle->physicsBody_->setGroup(1);
        // Set MainActor category
        vehicle->physicsBody_->setCategoryBitmask(1);
        // Collide with Terrain, Enemy bullets
        vehicle->physicsBody_->setCollisionBitmask(18);
        // Contact test with enemy bullets
        vehicle->physicsBody_->setContactTestBitmask(18);
        return vehicle;
    }

    // Error
    CC_SAFE_DELETE(vehicle);
    return nullptr;
}

void Vehicle::move()
{
    physicsBody_->applyForce(Vec2{ velocity_, 0.0f });
}

void Vehicle::stop()
{
    if (physicsBody_->getVelocity() != Vec2::ZERO) {
        physicsBody_->applyForce(-physicsBody_->getVelocity() * 2.0f);
    }
}

void Vehicle::tap(Vec2 &location)
{
    tap_ = location;
    if (onTap_) onTap_(location);
}

void Vehicle::update(float delta)
{
    // Adjust velocity
    float velocity{ physicsBody_->getVelocity().x };
    if (velocity < 0.0f) {
        physicsBody_->applyForce(Vec2{ -velocity * 4.0f, 0.0f });
    }
    // Adjust rotation
    float rotation{ physicsBody_->getRotation() };
    if (rotation < -8.0f || rotation > 8.0f) {
        log("Adjusting rotation %f", rotation);
        rotation -= rotation * delta;
        physicsBody_->setRotation(rotation);
    }
    if (specificUpdate_) specificUpdate_(delta);
}

void Vehicle::playEffect()
{
    // Play sound effect
    std::string audioPath{ StringUtils::format("audio/%s.ogg", getName().c_str()) };
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(audioPath.c_str());
}
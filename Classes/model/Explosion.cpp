#include "Explosion.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

// Constructor
Explosion::Explosion(Animate *animate)
        : sequence_{ nullptr }
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/explosion.wav");
#else
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/explosion.ogg");
#endif
    CallFunc *remove{ CallFunc::create([ this ]() {
        if (getParent() != nullptr) removeFromParent();
    }) };
    sequence_ = Sequence::createWithTwoActions(animate, remove);
    sequence_->retain();
}

Explosion::~Explosion()
{
    sequence_->release();
}

// Create method
Explosion *Explosion::create(Animate *animate)
{
    // Construct
    Explosion *explosion{ new(std::nothrow) Explosion{ animate }};
    // Initialize
    if (explosion && explosion->init()) {
        return explosion;
    }
    // Error
    CC_SAFE_DELETE(explosion);
    return nullptr;
}

void Explosion::animate()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/explosion.wav");
#else
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/explosion.ogg");
#endif
    runAction(sequence_);
}
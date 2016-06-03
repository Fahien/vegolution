#include "Explosion.h"

USING_NS_CC;

// Constructor
Explosion::Explosion(Animate* animate)
 : sequence_{ nullptr }
{
	CallFunc* remove{ CallFunc::create([this]() {
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
Explosion* Explosion::create(Animate* animate) {
	// Construct
	Explosion* explosion{ new (std::nothrow) Explosion{animate} };
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
	runAction(sequence_);
}
#include "Terrain2D.h"

USING_NS_CC;

Terrain2D::Terrain2D(MainActor* actor)
: actor_ {actor}
{}

Terrain2D* Terrain2D::create(MainActor* actor)
{
    // Construct
    Terrain2D* terrain {new (std::nothrow) Terrain2D(actor)};

    // Initialize
    if (terrain && terrain->init()) {
        terrain->autorelease();
        // Hook template method for creating physics body
        terrain->createPhysicsBody();
        return terrain;
    }

    CC_SAFE_DELETE(terrain);
    return nullptr;
}

void Terrain2D::createPhysicsBody()
{
    Size size {Director::getInstance()->getVisibleSize()};
    size.height /= 3;
    width_ = size.width;
    // Create material
    PhysicsMaterial material {1.0f, 0.2f, 0.5f};
    // Create physics body
    physicsBody_ = PhysicsBody::createBox(size, material);
    physicsBody_->setDynamic(false);
    this->addComponent(physicsBody_);
}

void Terrain2D::update(float delta)
{
    float x {this->getPositionX()};
    float actorX {actor_->getPositionX() + actor_->getVehicle()->getPositionX()};
    if (actorX > x) {
        if (actorX - x > width_ / 2) {
            this->setPositionX(x + width_);
        }
    }
}

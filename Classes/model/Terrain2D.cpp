#include "Terrain2D.h"

USING_NS_CC;

Terrain2D::Terrain2D(MainActor* actor, float width)
: physicsBody_ {nullptr}
, actor_       {actor}
, width_       {width}
{}

Terrain2D* Terrain2D::create(MainActor* actor, Size& size)
{
    // Construct
    Terrain2D* terrain {new (std::nothrow) Terrain2D{actor, size.width}};

    // Initialize
    if (terrain && terrain->init()) {
        terrain->autorelease();
        // Hook template method for creating physics body
        terrain->createPhysicsBody(size);
        return terrain;
    }

    CC_SAFE_DELETE(terrain);
    return nullptr;
}

void Terrain2D::createPhysicsBody(Size size)
{
    size.width *= 3;
    size.height /= 3;
    // Create material
    PhysicsMaterial material {1.0f, 0.2f, 0.5f};
    // Create physics body
    physicsBody_ = PhysicsBody::createBox(size, material);
    physicsBody_->setDynamic(false);
    this->addComponent(physicsBody_);
}

void Terrain2D::update(float delta)
{
    // Get actual X
    float x {this->getPositionX()};
    // Get actor X
    float actorX {actor_->getPositionX() + actor_->getVehicle()->getPositionX()};
    if (actorX - x > 0) {
        log("Moving terrain from %f", x);
        this->setPositionX(x + width_);
        log("To %f", this->getPositionX());
    }
}

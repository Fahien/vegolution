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
    size.height = static_cast<float>(static_cast<int>(size.height / 3));
    // Create material
	PhysicsMaterial material{ 4.0f, 0.125f, 0.5f };
    // Create physics body
    physicsBody_ = PhysicsBody::createBox(size, material);
    physicsBody_->setDynamic(false);
	// Terrain category is 2
    physicsBody_->setCategoryBitmask(2);
	// Collide with MainActor and Enemies
    physicsBody_->setCollisionBitmask(5);
    addComponent(physicsBody_);
}

void Terrain2D::update(float delta)
{
    // Get actual X
    float x {getPositionX()};
    // Get actor X
    float actorX {actor_->getPositionX() + actor_->getVehicle()->getPositionX()};
    if (actorX - x > 0) {
        log("Moving terrain from %f", x);
        setPositionX(x + width_);
        log("To %f", getPositionX());
    }
    // Error check
    float actorY {actor_->getPositionY() + actor_->getVehicle()->getPositionY()};
    if (actorY < getPositionY() + getContentSize().height) {
        log("Actor is going down?");
        actor_->getVehicle()->setPositionY(getPositionY() + getContentSize().height);
    }
}

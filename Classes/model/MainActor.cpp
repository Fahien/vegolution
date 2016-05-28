#include "MainActor.h"

USING_NS_CC;

MainActor::MainActor() {
    setName("MainActor");
}

MainActor* MainActor::create()
{
    // Construct
    MainActor* actor {new (std::nothrow) MainActor{}};

    // Initialize
    if (actor && actor->init()) {
        actor->autorelease();
        return actor;
    }

    CC_SAFE_DELETE(actor);
    return nullptr;
}

void MainActor::setOffset(float offsetX) { offsetX_ = offsetX; }

PhysicsBody* MainActor::getPhysicsBody()
{
    if (vehicle_ != nullptr) {
        return vehicle_->getPhysicsBody();
    }
    return nullptr;
}

void MainActor::setVehicle(Vehicle* vehicle)
{
    if (vehicle == nullptr || vehicle == vehicle_) return;
    if (vehicle->getPhysicsBody()->getOwner() != nullptr) {
        log("Vehicle has an owner");
        log("%s", vehicle->getName().c_str());
        log("%s", vehicle->getPhysicsBody()->getName().c_str());
        log("Owner: %s", vehicle->getPhysicsBody()->getName().c_str());
    }
    // If has a vehicle
    if (vehicle_ != nullptr) {
        // Remove the vehicle
        log("Removing old vehicle");
        vehicle->setPosition(vehicle_->getPosition());
        this->removeChild(vehicle_, false);
    }
    // Set new vehicle
    vehicle_ = vehicle;
    log("Setting new vehicle");
    this->addChild(vehicle);
}

void MainActor::setBullet(Bullet* bullet)
{
    bullet_ = bullet;
}

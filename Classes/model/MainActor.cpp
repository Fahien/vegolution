#include "MainActor.h"

USING_NS_CC;

MainActor::MainActor() { setName("Celly"); }

MainActor::~MainActor() {}

MainActor* MainActor::create()
{
    // Construct
    MainActor* actor {new (std::nothrow) MainActor{}};

    // Initialize
    if (actor && actor->init()) {
        actor->autorelease();
		actor->scheduleUpdate();
        return actor;
    }

    CC_SAFE_DELETE(actor);
    return nullptr;
}

PhysicsBody* MainActor::getPhysicsBody()
{
    if (vehicle_ != nullptr) {
        return vehicle_->getPhysicsBody();
    }
    return nullptr;
}

bool MainActor::switchVehicle()
{
	// Check if there are vehicles
	if (vehicles_.empty()) return false;
	// Get the next vehicle
	Vehicle* vehicle{ vehicles_.front() };
	// Check for equality
	if (vehicle_ == vehicle) return false;
	// Remove vehicle from the list
	vehicles_.erase(vehicles_.begin());
	log("New vehicle is %s", vehicle->getName().c_str());
	// Set vehicle settings
    vehicle->getPhysicsBody()->setDynamic(true);
	vehicle->getPhysicsBody()->resetForces();
	if (vehicle_ != nullptr) {
		// Set new vehicle position
		vehicle->setPosition(vehicle_->getPosition());
		vehicle->getPhysicsBody()->setVelocity(vehicle_->getPhysicsBody()->getVelocity());
		// Remove old vehicle
		vehicle_->getPhysicsBody()->setDynamic(false);
        removeChild(vehicle_, false);
		vehicles_.push_back(vehicle_);
	}
	// Set new vehicle
	vehicle_ = vehicle;
	// Check for error
	if (vehicle->getParent() != nullptr) vehicle->removeFromParent();
	addChild(vehicle);
	// Update listeners
	for (Gear* gear : gears_) { gear->onVehicleChange(vehicle); }
	return true;
}

void MainActor::update(float delta) {
	// Get current body
	PhysicsBody* body{ vehicle_->getPhysicsBody() };
	// Get current velocity
	Vec2 velocity{ body->getVelocity() };
	float vehicleVelocity{ vehicle_->getVelocity() };
	if (moving_) {
		// If is not the max velocity
		if (velocity.x < vehicleVelocity) {
			velocity.x += vehicleVelocity * delta * 2.0f; // Increment
			if (velocity.x > vehicleVelocity) {
				velocity.x = vehicleVelocity;
			}
		} else {
			velocity.x -= vehicleVelocity * delta * 2.0f; // Decrement
		}
	}
	else {
		// If velocity is greater than zero
		if (velocity.x > 0.0f) {
			velocity.x -= vehicleVelocity * delta * 2.0f; // Decrement
			if (velocity.x < 0.0f) velocity.x = 0.0f; // Set zero
		} else {
			velocity.x = 0.0f; // Set zero
		}
	}
	// Set the new velocity
	body->setVelocity(velocity);
}
#include "MainActor.h"

USING_NS_CC;

MainActor::MainActor()
{
    setName("Celly");
}

MainActor::~MainActor()
{}

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
	if (vehicle_ == vehicle) return false;
    vehicle->getPhysicsBody()->setDynamic(true);
	if (vehicle_ != nullptr) {
		log("New vehicle is %s", vehicle->getName().c_str());
        vehicle->getPhysicsBody()->resetForces();
		// Set new vehicle position
		vehicle->setPosition(vehicle_->getPosition());
		log("New vehicle position is %f, %f", vehicle->getPositionX(), vehicle->getPositionY());
		vehicle->getPhysicsBody()->setVelocity(vehicle_->getPhysicsBody()->getVelocity());
		log("New vehicle velocity is %f, %f", vehicle->getPhysicsBody()->getVelocity().x, getPhysicsBody()->getVelocity().y);
        removeChild(vehicle_, false);
        vehicle_->getPhysicsBody()->setDynamic(false);
		vehicles_.push_back(vehicle_);
	}
	vehicles_.erase(vehicles_.begin());
	// Set new vehicle
	vehicle_ = vehicle;
	addChild(vehicle);
	return true;
}

void MainActor::update(float delta) {
	// Get current body
	PhysicsBody* body{ vehicle_->getPhysicsBody() };
	// Get current velocity
	Vec2 velocity{ body->getVelocity() };
	float vehicleVelocity{ vehicle_->getVelocity() * 32.0f };
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
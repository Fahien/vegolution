#include "MainActor.h"

USING_NS_CC;

MainActor::MainActor()
	: offsetX_{ 0.0f }
	, moving_ { false }
	, vehicle_{ nullptr }
{ setName("Celly"); }

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
	// Error
    CC_SAFE_DELETE(actor);
    return nullptr;
}

PhysicsBody* MainActor::getPhysicsBody()
{
    if (vehicle_ != nullptr) { return vehicle_->getPhysicsBody(); }
	else { return nullptr; }
}

int MainActor::getHealth() const
{
	return vehicle_->getHealth();
}

void MainActor::setHealth(int health)
{
	vehicle_->setHealth(health);
	// Update health listener
	if (rightgear_) { rightgear_->onVehicleHealthChange(vehicle_); }
}

void MainActor::tap(Vec2 location)
{
	if (vehicle_) { vehicle_->tap(location); }
}

bool MainActor::canSwitchVehicle()
{
	return vehicle_->getPhysicsBody()->getVelocity().lengthSquared() < 4.0f;
}

bool MainActor::switchVehicle()
{
	// Check if there are vehicles
	if (vehicles_.empty()) { return false; }
	// Get the next vehicle
	Vehicle* vehicle{ vehicles_.front() };
	// Check for equality
	if (vehicle_ == vehicle) { return false; }
	// Remove vehicle from the list
	vehicles_.erase(vehicles_.begin());
	log("New vehicle is %s", vehicle->getName().c_str());
	// Set vehicle settings
    vehicle->getPhysicsBody()->setDynamic(true);
	vehicle->getPhysicsBody()->setRotationEnable(false);
	if (vehicle_ != nullptr) {
		// Set new vehicle position and velocity
		vehicle->setPosition(vehicle_->getPosition());
		// Remove old vehicle
		vehicle_->getPhysicsBody()->resetForces();
		vehicle_->getPhysicsBody()->setDynamic(false);
		vehicle_->pause();
        removeChild(vehicle_, false);
		vehicles_.push_back(vehicle_);
	}
	// Set new vehicle
	vehicle_ = vehicle;
	// Check for error
	if (vehicle->getParent() != nullptr) { vehicle->removeFromParent(); }
	addChild(vehicle);
	vehicle->resume();
	// Update listeners
	for (Gear* gear : gears_) { gear->onVehicleChange(vehicle); }
	return true;
}

void MainActor::update(float delta) {
	if (moving_) { vehicle_->move(delta); }
	else { vehicle_->stop(delta); }
}
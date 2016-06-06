#include "MainActor.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

MainActor::MainActor()
        : offsetX_{0.0f}
        , moving_{false}
        , vehicle_{nullptr}
        , rightGear_{nullptr}
{ setName("Celly"); }

MainActor::~MainActor() { }

MainActor *MainActor::create() {
    // Construct
    MainActor *actor{new(std::nothrow) MainActor{}};
    // Initialize
    if (actor && actor->init()) {
        actor->autorelease();
        // Load game over sound effect
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/gameover.ogg");
        actor->scheduleUpdate();
    }
    else { CC_SAFE_DELETE(actor); } // Error
    return actor;
}

float MainActor::getHealth() const {
    return vehicle_->getHealth();
}

void MainActor::setHealth(float health) {
    vehicle_->setHealth(health);
    // Update health listener
    if (rightGear_) { rightGear_->onVehicleHealthChange(vehicle_); }
}

void MainActor::tap(Vec2 location) {
    if (vehicle_) { vehicle_->tap(location); }
}

bool MainActor::switchVehicle() {
    // Check if there are vehicles
    if (vehicles_.empty()) { return false; }
    // Get the next vehicle
    Vehicle *vehicle{vehicles_.front()};
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
    vehicle->playEffect();
    // Update listeners
    for (Gear *gear : gears_) { gear->onVehicleChange(vehicle); }
    return true;
}

void MainActor::update(float delta) {
    if (moving_) { vehicle_->move(); }
    else { vehicle_->stop(); }
}

void MainActor::die() {
    moving_ = false;
    vehicle_->stopAllActions();
    vehicle_->setTexture("misc/celly-dead.png");
    vehicle_->removeAllComponents();
    vehicle_->createPhysicsBody("dead");
    // Play sound effect
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/gameover.ogg");
}
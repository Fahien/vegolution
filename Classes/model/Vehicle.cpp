#include "Vehicle.h"

USING_NS_CC;

Vehicle::Vehicle(float velocity, float armor)
: velocity_ {velocity}
, armor_    {armor}
{}

Vehicle* Vehicle::create(std::string name, float velocity, float armor)
{
    // Construct
    Vehicle* vehicle {new (std::nothrow) Vehicle{velocity, armor}};

    // Initialize
    if (vehicle && vehicle->initWithFile(name)) {
        vehicle->autorelease();
        vehicle->setName(name);
        vehicle->createPhysicsBody(name);
        return vehicle;
    }

    CC_SAFE_DELETE(vehicle);
    return nullptr;
}

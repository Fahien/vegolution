#ifndef __MAIN_ACTOR_H__
#define __MAIN_ACTOR_H__

#include "cocos2d.h"
#include "Vehicle.h"

class MainActor : public cocos2d::Node
{
public:
    MainActor();
	~MainActor();
    static MainActor* create();

    inline float getOffsetX() const { return offsetX_; }
    void setOffset(float offset);

    cocos2d::PhysicsBody* getPhysicsBody();

    inline Vehicle* getVehicle() const { return vehicle_; }

    inline std::vector<Vehicle*>& getVehicles() { return vehicles_; }

	bool switchVehicle();

private:
    float offsetX_;

    float velocity;
    float attack;
    float health;

    Vehicle* vehicle_ {nullptr};

    std::vector<Vehicle*> vehicles_;
};

#endif // __MAIN_ACTOR_H__

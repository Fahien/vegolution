#ifndef __MAIN_ACTOR_H__
#define __MAIN_ACTOR_H__

#include "cocos2d.h"
#include "Vehicle.h"
#include "hud/Gear.h"
#include "hud/RightGear.h"

class MainActor : public cocos2d::Node
{
public:
    MainActor();
	~MainActor();
    static MainActor* create();

    inline float getOffsetX() const { return offsetX_; }
	inline void setOffsetX(float offsetX) { offsetX_ = offsetX; }
	inline bool isMoving() const { return moving_; }
	inline void setMoving(bool moving) { moving_ = moving; }

    cocos2d::PhysicsBody* getPhysicsBody();

    inline Vehicle* getVehicle() const { return vehicle_; }
    inline std::vector<Vehicle*>& getVehicles() { return vehicles_; }
	bool switchVehicle();

	inline int getHealth() const { return vehicle_->getHealth(); }
	inline void setHealth(int health)
	{
		vehicle_->setHealth(health);
		// Update health listener
		rightgear_->onVehicleHealthChange(vehicle_);
	}

	inline void addGear(Gear* gear) { gears_.push_back(gear); }
	inline void setRightGear(RightGear* gear) { rightgear_ = gear; }

	void update(float delta);

private:
	// Vehicle offset
    float offsetX_;
	// Wether is moving
	bool moving_;

	// Current vehicle
	Vehicle* vehicle_{ nullptr };
	// Vehicles
    std::vector<Vehicle*> vehicles_;

	// Vehicle listeners
	std::vector<Gear*> gears_;
	// Vehicle health listener
	RightGear* rightgear_{ nullptr };
};

#endif // __MAIN_ACTOR_H__
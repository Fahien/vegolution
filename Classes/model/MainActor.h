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

    inline std::vector<Vehicle*>& getVehicles() { return vehicles_; }

	/// Get the current vehicle
	inline Vehicle* getVehicle() const { return vehicle_; }
	/// Switch the current vehicle to the next
	bool switchVehicle();
	/// Get the current vehicle physics body
	cocos2d::PhysicsBody* getPhysicsBody();
	/// Get the current vehicle health
	int getHealth() const;
	/// Set the current vehicle health
	void setHealth(int health);
	/// Implement specific functionality for vehicle
	void tap(cocos2d::Vec2 location);
	/// Tests whether could switch vehicle
	bool canSwitchVehicle();

	/// Add a gear vehicle listener
	inline void addGear(Gear* gear) { gears_.push_back(gear); }
	/// Set the right gear vehicle health listener
	inline void setRightGear(RightGear* gear) { rightgear_ = gear; }

	/// Update method
	void update(float delta);

private:
	// Vehicle offset
    float offsetX_;
	// Wether is moving
	bool moving_;

	// Current vehicle
	Vehicle* vehicle_;
	// Vehicles
    std::vector<Vehicle*> vehicles_;

	// Vehicle listeners
	std::vector<Gear*> gears_;
	// Vehicle health listener
	RightGear* rightgear_{ nullptr };
};

#endif // __MAIN_ACTOR_H__
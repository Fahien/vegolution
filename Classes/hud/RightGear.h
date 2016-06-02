#ifndef __RIGHT_GEAR_H__
#define __RIGHT_GEAR_H__

#include "Gear.h"

class RightGear : public Gear
{
public:
	// Health bar setter
	virtual void setHealthBar(cocos2d::Sprite* healthBar) {
		healthBar_ = healthBar;
		width_ = healthBar->getContentSize().width;
		height_ = healthBar->getContentSize().height;
	}

	// Callback for vehicle change
	virtual void onVehicleChange(Vehicle* vehicle);

	// Calback for vehicle hit
	virtual void onVehicleHealthChange(Vehicle* vehicle);

	// Cocos create function
	CREATE_FUNC(RightGear)
private:
	float width_;
	float height_;
	cocos2d::Sprite* healthBar_{ nullptr };
};

#endif // __RIGHT_GEAR_H__

#ifndef __LEFT_GEAR_H__
#define __LEFT_GEAR_H__

#include "cocos2d.h"
#include "Gear.h"

class LeftGear : public Gear
{
public:
	/// Callback for vehicle change
	virtual void onVehicleChange(Vehicle* vehicle);

	// Cocos create func
	CREATE_FUNC(LeftGear)
};

#endif // __LEFT_GEAR_H__

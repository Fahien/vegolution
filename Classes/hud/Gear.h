#ifndef __GEAR_H__
#define __GEAR_H__

#include "ui/CocosGUI.h"
#include "model/Vehicle.h"

/*
 * This is a Vehicle Listener
 */
class Gear : public cocos2d::ui::ImageView
{
public:
	virtual void onVehicleChange(Vehicle* vehicle) = 0;
};

#endif // __GEAR_H__

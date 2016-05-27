#ifndef __VEHICLE_VIEW_H__
#define __VEHICLE_VIEW_H__

#include "ui/CocosGUI.h"
#include "model/Vehicle.h"

class VehicleView : public cocos2d::ui::ImageView
{
public:
    VehicleView(Vehicle* vehicle);

    static VehicleView* create(Vehicle* vehicle);

    inline Vehicle* getVehicle() const { return vehicle_; }

private:
    Vehicle* vehicle_;
};

#endif // __VEHICLE_VIEW_H__

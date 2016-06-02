#include "LeftGear.h"

USING_NS_CC;

void LeftGear::onVehicleChange(Vehicle* vehicle)
{
	filename_ = StringUtils::format("misc/leftgear-%s.png", vehicle->getName().c_str());
	loadTexture(filename_);
}
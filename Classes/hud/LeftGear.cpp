#include "LeftGear.h"

USING_NS_CC;

void LeftGear::onVehicleChange(Vehicle* vehicle)
{
	std::string filename { StringUtils::format("hud/left-gear-%s.png", vehicle->getName().c_str()) };
	loadTexture(filename);
}
#include "RightGear.h"

USING_NS_CC;

void RightGear::onVehicleChange(Vehicle* vehicle)
{
	onVehicleHealthChange(vehicle); // Delegate
}

void RightGear::onVehicleHealthChange(Vehicle* vehicle)
{
	float healthPercent{ vehicle->getHealth() / vehicle->getHealthMax() };
	healthBar_->setTextureRect(Rect{ 0.0f, 0.0f, width_, height_ * healthPercent });
}

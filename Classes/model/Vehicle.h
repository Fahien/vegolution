#ifndef __VEHICLE_H__
#define __VEHICLE_H__

#include "Actor.h"
#include "Bullet.h"

class Vehicle : public Actor
{
public:
    Vehicle(int velocity, int health, int delay, Bullet* bullet);
    static Vehicle* create(std::string name, int velocity, int health, int delay, Bullet* bullet);

    inline int getVelocity() const { return velocity_; }
    inline int getHealth() const { return health_; }
	inline int getDelay() const { return delay_; }

	inline Bullet* getBullet() const { return bullet_; }

private:
	int velocity_;
	int health_;
	int delay_;

	Bullet* bullet_;
};

#endif // __VEHICLE_H__

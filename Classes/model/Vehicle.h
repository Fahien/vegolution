#ifndef __VEHICLE_H__
#define __VEHICLE_H__

#include "Actor.h"
#include "Bullet.h"

class Vehicle : public Actor
{
public:
    Vehicle(int velocity, int health, int delay, Bullet* bullet, cocos2d::Vec2 offset);
    static Vehicle* create(std::string name, int velocity, int health, int delay, Bullet* bullet, cocos2d::Vec2 offset);

    inline int getVelocity() const { return velocity_; }
    inline int getHealth() const { return health_; }
	inline int getDelay() const { return delay_; }

	inline Bullet* getBullet() const { return bullet_; }

	inline cocos2d::RepeatForever* getAction() const { return action_; }

private:
	int velocity_;
	int health_;
	int delay_;

	Bullet* bullet_;
	cocos2d::Vec2 offset_;

	cocos2d::RepeatForever* action_;
};

#endif // __VEHICLE_H__

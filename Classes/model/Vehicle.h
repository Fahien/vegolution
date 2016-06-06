#ifndef __VEHICLE_H__
#define __VEHICLE_H__

#include "Actor.h"
#include "Bullet.h"

class Vehicle : public Actor
{
public:
    Vehicle(int healthMax, int velocity, int delay, Bullet* bullet, cocos2d::Vec2 offset);
    static Vehicle* create(std::string name, int healthMax, int velocity, int delay, Bullet* bullet, cocos2d::Vec2 offset, bool gravity);

    inline float getVelocity() const { return velocity_; }
	inline int getHealthMax() const { return healthMax_; }
    inline float getHealth() const { return health_; }
	inline void setHealth(float health) { health_ = health; }
	inline int getDelay() const { return delay_; }
	inline Bullet* getBullet() const { return bullet_; }

	void tap(cocos2d::Vec2& location);
	inline cocos2d::Vec2 getTap() const { return tap_; }
	std::function<void(cocos2d::Vec2)> onTap_;

	void move();
	void stop();

	// Update function
	void update(float delta);
	std::function<void(float)> specificUpdate_;

	void playEffect();

private:
	int healthMax_;
	float health_;
	float velocity_;
	int delay_;

	Bullet* bullet_;
	cocos2d::Vec2 offset_;

	cocos2d::Vec2 tap_;
};

#endif // __VEHICLE_H__

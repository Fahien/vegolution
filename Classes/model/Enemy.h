#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Bullet.h"

class Enemy : public Actor
{
public:
	Enemy(int healthMax, int velocity, int delay, int offsetY, Bullet* bullet);
	~Enemy();

	/// Create method
    static Enemy* create(std::string name, int healthMax, int velocity, int delay, Bullet* bullet, bool gravity, int y);

	inline int getHealthMax() const { return healthMax_; }
    inline int getHealth() const { return health_; }
    inline void setHealth(unsigned health) { health_ = health; }
	inline float getVelocity() const { return velocity_; }
	inline float getOffsetY() const { return offsetY_; }
	inline Bullet* getBullet() const { return bullet_; }

	/// Spawn an enemy
	void spawn(cocos2d::Vec2 position);
	/// Update method
	void update(float delta);
	/// Properly remove
	void remove();

private:
	int healthMax_;
    int health_;
	float velocity_;
	int delay_;
	float offsetY_;

	Bullet* bullet_;
};

#endif // __ENEMY_H__

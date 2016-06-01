#ifndef __ENEMY_ACTOR_H__
#define __ENEMY_ACTOR_H__

#include "Actor.h"
#include "Bullet.h"

class Enemy : public Actor
{
public:
	Enemy(int healthMax, int velocity, int delay, Bullet* bullet);

	// Create method
    static Enemy* create(std::string name, int healthMax, int velocity, int delay, Bullet* bullet);

	inline int getHealthMax() const { return healthMax_; }
    inline int getHealth() const { return health_; }
    inline void setHealth(unsigned health) { health_ = health; }

	inline int getVelocity() const { return velocity_; }

	inline Bullet* getBullet() const { return bullet_; }

private:
	int healthMax_;
    int health_;
	int velocity_;
	int delay_;

	Bullet* bullet_;
};

#endif // __ENEMY_ACTOR_H__

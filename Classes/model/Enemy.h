#ifndef __ENEMY_ACTOR_H__
#define __ENEMY_ACTOR_H__

#include "Actor.h"
#include "Bullet.h"

class Enemy : public Actor
{
public:
	Enemy(int healthMax, int velocity, int delay, Bullet* bullet);
	~Enemy();

	/// Create method
    static Enemy* create(std::string name, int healthMax, int velocity, int delay, Bullet* bullet);

	inline int getHealthMax() const { return healthMax_; }
    inline int getHealth() const { return health_; }
    inline void setHealth(unsigned health) { health_ = health; }
	inline float getVelocity() const { return velocity_; }
	inline Bullet* getBullet() const { return bullet_; }

	/// Update method
	void update(float delta);
	/// Properly remove
	void remove();

private:
	int healthMax_;
    int health_;
	float velocity_;
	int delay_;

	Bullet* bullet_;
};

#endif // __ENEMY_ACTOR_H__

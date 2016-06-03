#ifndef __BULLET_ACTOR_H__
#define __BULLET_ACTOR_H__

#include "Actor.h"

class Bullet : public Actor {
public:
	Bullet(int damage, float velocity);

    static Bullet* create(std::string name, int damage, float velocity);

    inline int getDamage() const { return damage_; }
	inline float getVelocity() const { return velocity_; }

	void remove();
private:
    int damage_;
	float velocity_;
};

#endif // __BULLET_ACTOR_H__

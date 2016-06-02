#ifndef __BULLET_ACTOR_H__
#define __BULLET_ACTOR_H__

#include "Actor.h"

class Bullet : public Actor
{
public:
	Bullet(int damage, int velocity);

    static Bullet* create(std::string imagename, int damage, int velocity);

    inline int getDamage() const { return damage_; }
	inline float getVelocity() const { return velocity_; }

private:
    int damage_;
	float velocity_;
};

#endif // __BULLET_ACTOR_H__

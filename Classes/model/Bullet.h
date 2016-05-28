#ifndef __BULLET_ACTOR_H__
#define __BULLET_ACTOR_H__

#include "Actor.h"

class Bullet : public Actor
{
public:
    static Bullet* create(std::string name);

    inline float getDamage() const { return damage_; }
    inline void setDamage(float damage) { damage_ = damage; }

private:
    float damage_;

};

#endif // __BULLET_ACTOR_H__

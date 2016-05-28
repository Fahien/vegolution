#ifndef __ENEMY_ACTOR_H__
#define __ENEMY_ACTOR_H__

#include "Actor.h"

class Enemy : public Actor
{
public:
    static Enemy* create(std::string name);

    Enemy* clone();

    inline unsigned getHealth() const { return health_; }
    inline void setHealth(unsigned health) { health_ = health; }

private:
    unsigned health_;
};

#endif // __ENEMY_ACTOR_H__

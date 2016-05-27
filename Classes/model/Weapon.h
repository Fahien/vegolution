#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "cocos2d.h"

class Weapon : cocos2d::Sprite
{
public:
    Weapon(float attack, unsigned int ammo);
    inline float getAttack() const { return attack_; }
    inline unsigned int getAmmo() const { return ammo_; }

private:
    float        attack_;
    unsigned int ammo_;
};

#endif // __WEAPON_H__

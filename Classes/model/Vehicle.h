#ifndef __VEHICLE_H__
#define __VEHICLE_H__

#include "Actor.h"

class Vehicle : public Actor
{
public:
    Vehicle(float velocity, float armor);
    static Vehicle* create(std::string name, float velocity, float armor);

    inline float getVelocity() const { return velocity_; }
    inline float getArmor() const {return armor_; }

private:
    float velocity_;
    float armor_;
};

#endif // __VEHICLE_H__

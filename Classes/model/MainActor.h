#ifndef __MAIN_ACTOR_H__
#define __MAIN_ACTOR_H__

#include "cocos2d.h"
#include "Vehicle.h"
#include "Weapon.h"

class MainActor : public cocos2d::Node
{
public:
    MainActor();
    static MainActor* create();

    inline float getOffsetX() const { return offsetX_; }
    void setOffset(float offset);

    cocos2d::PhysicsBody* getPhysicsBody();

    inline Vehicle* getVehicle() const { return vehicle_; }
    void setVehicle(Vehicle* vehicle);
    void setWeapon(Weapon* weapon);

    inline std::vector<Vehicle*>& getVehicles() { return vehicles_; }
    inline std::vector<Weapon*>& getWeapons() { return weapons_; }
private:
    float offsetX_;

    float velocity;
    float attack;
    float health;

    Vehicle* vehicle_ {nullptr};
    Weapon*  weapon_  {nullptr};

    std::vector<Vehicle*> vehicles_;
    std::vector<Weapon*> weapons_;
};

#endif // __MAIN_ACTOR_H__

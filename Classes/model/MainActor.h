#ifndef __MAIN_ACTOR_H__
#define __MAIN_ACTOR_H__

#include "cocos2d.h"
#include "Vehicle.h"
#include "Bullet.h"

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

    inline Bullet* getBullet() const { return bullet_; }
    void setBullet(Bullet* bullet);

    inline std::vector<Vehicle*>& getVehicles() { return vehicles_; }
    inline std::vector<Bullet*>& getBullets() { return bullets_; }
private:
    float offsetX_;

    float velocity;
    float attack;
    float health;

    Vehicle* vehicle_ {nullptr};
    Bullet*  bullet_  {nullptr};

    std::vector<Vehicle*> vehicles_;
    std::vector<Bullet*> bullets_;
};

#endif // __MAIN_ACTOR_H__

#ifndef __TERRAIN_2D_H__
#define __TERRAIN_2D_H__

#include "cocos2d.h"
#include "MainActor.h"

class Terrain2D : public cocos2d::Node
{
public:
    Terrain2D(MainActor* actor);
    static Terrain2D* create(MainActor* actor);

    void update(float delta);

    void createPhysicsBody();
    inline cocos2d::PhysicsBody* getPhysicsBody() const { return physicsBody_; }
private:
    float width_;
    cocos2d::PhysicsBody* physicsBody_;
    MainActor* actor_;
};

#endif // __TERRAIN_2D_H__

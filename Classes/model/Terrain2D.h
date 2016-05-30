#ifndef __TERRAIN_2D_H__
#define __TERRAIN_2D_H__

#include "cocos2d.h"
#include "MainActor.h"

class Terrain2D : public cocos2d::Node
{
public:
    Terrain2D(MainActor* actor, float width);
    static Terrain2D* create(MainActor* actor, cocos2d::Size& size);

    void createPhysicsBody(cocos2d::Size size);

    void update(float delta);

    inline cocos2d::PhysicsBody* getPhysicsBody() const { return physicsBody_; }

protected:
    cocos2d::PhysicsBody* physicsBody_;

private:
    MainActor* actor_;
    float width_;
};

#endif // __TERRAIN_2D_H__

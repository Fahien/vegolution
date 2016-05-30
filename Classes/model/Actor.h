#ifndef __ACTOR_H__
#define __ACTOR_H__

#include "cocos2d.h"

class Terrain2D;

class Actor : public cocos2d::Sprite
{
public:
    static Actor* create(std::string name);

    virtual void createPhysicsBody(std::string name);
    inline cocos2d::PhysicsBody* getPhysicsBody() const { return physicsBody_; }

    // Implement the static create method
    CREATE_FUNC(Actor)

protected:
    cocos2d::PhysicsBody* physicsBody_;

private:
    friend class Terrain2D;
};

#endif // __ACTOR_H__

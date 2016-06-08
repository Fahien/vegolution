#ifndef __ADD_CHILD_H__
#define __ADD_CHILD_H__

#include "cocos2d.h"

class AddChild : public cocos2d::ActionInstant
{
public:
    AddChild();
    ~AddChild();

    static AddChild* create(cocos2d::Node* addedNode);

    bool initWithTarget(cocos2d::Node* addedNode);

    virtual void update(float time) override;
private:
    cocos2d::Node* addedNode_;
};

#endif // __ADD_CHILD_H__

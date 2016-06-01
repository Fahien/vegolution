#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "model/MainActor.h"

class GameController : public cocos2d::EventListenerTouchOneByOne
{
public:
    GameController(MainActor* actor, float centerX);
    static GameController* create(MainActor* actor, float centerX);

private:
    MainActor* actor_;
    float centerX_;
};

#endif // __GAME_CONTROLLER_H__

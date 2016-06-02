#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "model/MainActor.h"

class GameController : public cocos2d::EventListenerTouchOneByOne
{
public:
    GameController(MainActor* actor, const cocos2d::Size contentSize);
    static GameController* create(MainActor* actor, const cocos2d::Size contentSize);

private:
    MainActor* actor_;
    float centerX_;
	float centerY_;
};

#endif // __GAME_CONTROLLER_H__

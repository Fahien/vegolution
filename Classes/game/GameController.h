#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "model/MainActor.h"
#include "model/Parallax.h"

class GameController : public cocos2d::EventListenerTouchOneByOne
{
public:
    GameController(MainActor* actor, Parallax* parallax);

    static GameController* create(MainActor* actor, Parallax* parallax);
private:
    MainActor*      actor_;
    Parallax*   parallax_;
};

#endif // __GAME_CONTROLLER_H__

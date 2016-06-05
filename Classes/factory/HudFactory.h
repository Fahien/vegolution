#ifndef __HUD_FACTORY_H__
#define __HUD_FACTORY_H__

#include "model/MainActor.h"
#include "hud/LeftGear.h"

class HudFactory {
public:
    HudFactory(const cocos2d::Size visibleSize);
    ~HudFactory();

    LeftGear* getLeftGear(MainActor* actor);
    RightGear* getRightGear(MainActor *actor);

private:
    const cocos2d::Size visibleSize_;

    RightGear* rightGear_;
    LeftGear* leftGear_;
};

#endif // __HUD__FACTORY_H__

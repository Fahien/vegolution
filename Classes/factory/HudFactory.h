#ifndef __HUD_FACTORY_H__
#define __HUD_FACTORY_H__

#include "TextFactory.h"
#include "model/MainActor.h"
#include "hud/LeftGear.h"

class HudFactory {
public:
    HudFactory(TextFactory& textFactory);
    ~HudFactory();

    cocos2d::ui::Text* getScoreText();
    LeftGear* getLeftGear(MainActor* actor);
    RightGear* getRightGear(MainActor *actor);

private:
    TextFactory textFactory_;

    cocos2d::ui::Text* score_;
    RightGear* rightGear_;
    LeftGear* leftGear_;
};

#endif // __HUD__FACTORY_H__

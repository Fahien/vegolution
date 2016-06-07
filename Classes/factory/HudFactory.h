#ifndef __HUD_FACTORY_H__
#define __HUD_FACTORY_H__

#include "TextFactory.h"
#include "model/MainActor.h"
#include "hud/LeftGear.h"

class HudFactory {
public:
    HudFactory(DataManager& data, TextFactory& textFactory);

    ~HudFactory();

    cocos2d::ui::Text* getScoreText()
    { return score_; }

    cocos2d::ui::Text* getQuitText()
    { return quit_; };

    LeftGear* getLeftGear(MainActor* actor, cocos2d::Size& visibleSize);

    RightGear* getRightGear(MainActor* actor, cocos2d::Size& visibleSize);

protected:
    void createQuitText(DataManager& data, TextFactory& textFactory);

    void createScoreText(TextFactory& textFactory);

private:
    cocos2d::ui::Text* quit_;
    cocos2d::ui::Text* score_;
    RightGear* rightGear_;
    LeftGear* leftGear_;
};

#endif // __HUD__FACTORY_H__

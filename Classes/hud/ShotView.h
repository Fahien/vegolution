#ifndef __SHOT_VIEW_H__
#define __SHOT_VIEW_H__

#include "ui/CocosGUI.h"
#include "model/MainActor.h"

class ShotView : public cocos2d::ui::ImageView
{
public:
    ShotView(MainActor* actor, float width);

    static ShotView* create(std::string name, MainActor* actor, float width);

    void toggle();

protected:
    void createChildren();

private:
    MainActor* actor_;
    float      width_;
    bool       open_;
};

#endif // __SHOT_VIEW_H__

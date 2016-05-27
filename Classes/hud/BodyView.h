#ifndef __BODY_VIEW_H__
#define __BODY_VIEW_H__

#include "ui/CocosGUI.h"
#include "model/MainActor.h"

class BodyView : public cocos2d::ui::ImageView
{
public:
    BodyView(MainActor* actor, float width);

    static BodyView* create(std::string name, MainActor* actor, float width);

    void toggle();

protected:
    void createChildren();

private:
    MainActor* actor_;
    float      width_;
    bool       open_;
};

#endif // __BODY_VIEW_H__

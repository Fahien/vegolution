#ifndef __BULLET_VIEW_H__
#define __BULLET_VIEW_H__

#include "ui/CocosGUI.h"
#include "model/Bullet.h"

class BulletView : public cocos2d::ui::ImageView
{
public:
    BulletView(Bullet* bullet);

    static BulletView* create(Bullet* bullet);

    inline Bullet* getBullet() const { return bullet_; }

private:
    Bullet* bullet_;
};

#endif // __BULLET_VIEW_H__

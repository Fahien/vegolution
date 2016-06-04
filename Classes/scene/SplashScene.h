#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"
#include "Vegolution.h"

class SplashScene : public cocos2d::Scene {
public:
    bool init(Vegolution* game);

    static SplashScene* create(Vegolution* game);
};

#endif //__SPLASH_SCENE_H__

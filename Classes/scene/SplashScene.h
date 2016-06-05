#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"
#include "data/DataManager.h"

class SplashScene : public cocos2d::Scene {
public:
    bool init(DataManager* data);

    static SplashScene* create(DataManager* data);
};

#endif //__SPLASH_SCENE_H__

#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"
#include "data/DataManager.h"
#include "factory/TextFactory.h"

class SplashScene : public cocos2d::Scene {
public:
    bool init(DataManager& data, TextFactory& textFactory);

    static SplashScene* create(DataManager& data, TextFactory& textFactory);
};

#endif //__SPLASH_SCENE_H__

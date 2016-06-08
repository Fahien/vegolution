#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"
#include "factory/SplashFactory.h"

class SplashScene : public cocos2d::Scene {
public:
    bool init(DataManager& data, TextFactory& textFactory);

    static SplashScene* create(DataManager& data, TextFactory& textFactory);

private:
    SplashFactory factory_;
};

#endif //__SPLASH_SCENE_H__

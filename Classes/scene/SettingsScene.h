#ifndef __SETTINGS_SCENE_H__
#define __SETTINGS_SCENE_H__

#include "cocos2d.h"
#include "Vegolution.h"

class SettingsScene : public cocos2d::Scene {
public:
    bool init(Vegolution* game);

    static SettingsScene* create(Vegolution* game);
};

#endif //__SETTINGS_SCENE_H__

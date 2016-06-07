#ifndef __SETTINGS_SCENE_H__
#define __SETTINGS_SCENE_H__

#include "cocos2d.h"
#include "data/DataManager.h"
#include "factory/TextFactory.h"

class SettingsScene : public cocos2d::Scene {
public:
    SettingsScene();
    ~SettingsScene();

    bool init(DataManager& data, TextFactory& textFactory);

    static SettingsScene* create(DataManager& data, TextFactory& textFactory);
};

#endif //__SETTINGS_SCENE_H__

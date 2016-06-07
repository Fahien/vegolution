#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "data/DataManager.h"
#include "factory/TextFactory.h"

class MainScene : public cocos2d::Scene {
public:
    MainScene();
    ~MainScene();

    bool init(DataManager& data, TextFactory& textFactory);

    static MainScene* create(DataManager& data, TextFactory& textFactory);
};

#endif // __MAIN_SCENE_H__

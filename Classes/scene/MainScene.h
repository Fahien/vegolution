#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "data/DataManager.h"

class MainScene : public cocos2d::Scene {
public:
    MainScene();
    ~MainScene();

    bool init(DataManager* data);

    static MainScene* create(DataManager* data);
};

#endif // __MAIN_SCENE_H__

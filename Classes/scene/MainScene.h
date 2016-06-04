#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "Vegolution.h"

class MainScene : public cocos2d::Scene {
public:
    bool init(Vegolution* game);

    static MainScene* create(Vegolution* game);


};

#endif // __MAIN_SCENE_H__

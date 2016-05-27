#ifndef __MAIN_LAYER_H__
#define __MAIN_LAYER_H__

#include "cocos2d.h"
#include "Vegolution.h"
#include "MainFactory.h"

class MainLayer : public cocos2d::Layer
{
public:
    MainLayer(Vegolution* game);
    ~MainLayer();

    static MainLayer* create(Vegolution* game);

    static cocos2d::Scene* createScene(Vegolution* game);

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    

private:
    Vegolution* game_;
    MainFactory factory_;
};

#endif // __MAIN_LAYER_H__

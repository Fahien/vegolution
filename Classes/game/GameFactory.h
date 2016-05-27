#ifndef __GAME_FACTORY_H__
#define __GAME_FACTORY_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Vegolution.h"
#include "model/Vehicle.h"
#include "model/MainActor.h"
#include "model/Terrain2D.h"
#include "model/Parallax.h"
#include "hud/BodyView.h"

class GameFactory
{
public:
    GameFactory(Vegolution* game);

    Vehicle*         createVehicle(std::string name);
    MainActor*       createActor();
    Terrain2D*       createTerrain();
    Parallax*        createParallax();
    cocos2d::Sprite* createBoard();

    BodyView* createBodyMenu();

private:
    Vegolution* game_;

    cocos2d::Size    visibleSize_;
    cocos2d::Vec2    origin_;
    cocos2d::Vec2    center_;
    float            offsetX_;

    MainActor*       actor_     {nullptr};
    Terrain2D*       terrain_   {nullptr};
    Parallax*        parallax_  {nullptr};
    cocos2d::Sprite* board_     {nullptr};

    BodyView* body_ {nullptr};
};

#endif // __GAME_FACTORY_H__

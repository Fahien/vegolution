#ifndef __GAME_FACTORY_H__
#define __GAME_FACTORY_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "data/DataManager.h"
#include "model/Vehicle.h"
#include "model/MainActor.h"
#include "model/Terrain2D.h"
#include "model/Parallax.h"
#include "model/Explosion.h"
#include "hud/LeftGear.h"
#include "hud/RightGear.h"

class GameFactory
{
public:
    GameFactory(DataManager& data);
    ~GameFactory();

    MainActor* createActor();
    Terrain2D* createTerrain();
    Parallax* createParallax();
    cocos2d::Sprite* createBoard();

    inline std::vector<Bullet*>& getBullets() { return bullets_; }

    void createExplosion(cocos2d::Sprite* node);

private:
    cocos2d::Size visibleSize_;
	cocos2d::Vec2 origin_;
    cocos2d::Vec2 center_;
    float offsetX_;

    std::vector<Bullet*> bullets_;
	std::vector<Vehicle*> vehicles_;

	MainActor* actor_;
	Terrain2D* terrain_;
	Parallax* parallax_;
	cocos2d::Sprite* board_;
	Explosion* explosion_;
};

#endif // __GAME_FACTORY_H__

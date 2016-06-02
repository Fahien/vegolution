#ifndef __GAME_FACTORY_H__
#define __GAME_FACTORY_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Vegolution.h"
#include "model/Enemy.h"
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
    GameFactory(Vegolution* game);
    ~GameFactory();

	Vehicle* getVehicle(std::string& vehiclename);

    Enemy* spawnEnemy();
	void despawnEnemy(Enemy* enemy);

    MainActor* createActor();
    Terrain2D* createTerrain();
    Parallax* createParallax();
    cocos2d::Sprite* createBoard();

    LeftGear* createLeftGear();
	RightGear* createRightGear();

	void createExplosion(Bullet* node);
private:
	// SMELL variable
	bool jumping_;

    Vegolution* game_;
    DataManager* data_;

    cocos2d::Size visibleSize_;
    cocos2d::Vec2 origin_;
    cocos2d::Vec2 center_;
    float offsetX_;

	std::vector<Bullet*> bullets_;
	std::vector<Vehicle*> vehicles_;
    std::vector<Enemy*> enemies_;
    std::vector<Enemy*> enemyPool_;

	MainActor* actor_{ nullptr };
	Terrain2D* terrain_{ nullptr };
	Parallax* parallax_{ nullptr };
	cocos2d::Sprite* board_{ nullptr };

	LeftGear* leftgear_{ nullptr };
	RightGear* rightgear_{ nullptr };

	Explosion* explosion_{ nullptr };
};

#endif // __GAME_FACTORY_H__

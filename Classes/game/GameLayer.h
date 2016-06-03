#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

static const float deltaMin = 0.0625f;

static const float deltaMax = 0.015625f;

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Vegolution.h"
#include "GameController.h"
#include "factory/GameFactory.h"
#include "factory/HudFactory.h"
#include "factory/EnemyFactory.h"
#include "model/MainActor.h"

class GameLayer : public cocos2d::Layer
{
public:
    GameLayer(Vegolution* game);

    static GameLayer* create(Vegolution* game);

    // Return the scene instance pointer
    static cocos2d::Scene* createScene(Vegolution* game);

    virtual bool init();

    // Update method
    void update(float delta);
	void scheduleSpawning();
	void listenPlayerBullet();
	void listenEnemyBullet();
private:
    Vegolution* game_;
    GameFactory factory_;
	HudFactory hudFactory_;
	EnemyFactory enemyFactory_;

	cocos2d::Layer* menuLayer_{ nullptr };

	cocos2d::Scene* scene_{ nullptr };
	MainActor* actor_{ nullptr };
	cocos2d::Sprite* board_{ nullptr };
    float centerX_;
    float offsetX_;
};

#endif // __GAME_LAYER_H__

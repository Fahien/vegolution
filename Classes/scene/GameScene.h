#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

static const float deltaMin = 0.0625f;

static const float deltaMax = 0.015625f;

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "data/DataManager.h"
#include "game/GameController.h"
#include "factory/GameFactory.h"
#include "factory/HudFactory.h"
#include "factory/EnemyFactory.h"
#include "model/MainActor.h"

class GameScene : public cocos2d::Scene
{
public:
    GameScene(DataManager* data);
	~GameScene();

    static GameScene* create(DataManager* data);

	/// Init method
    virtual bool init();
	virtual void willEnterForeground();
    /// Update method
    void update(float delta);
	void scheduleSpawning();
	void listenPlayerBullet();
	void listenEnemyBullet();

private:
    DataManager* data_;
    GameFactory factory_;
    EnemyFactory enemyFactory_;
    int score_{ 0 };
	cocos2d::ui::Text* scoreText_{ nullptr };
	cocos2d::Layer* menuLayer_{ nullptr };

	MainActor* actor_{ nullptr };
	cocos2d::Sprite* board_{ nullptr };
    float centerX_;
    float offsetX_;

    bool pause_;
};

#endif // __GAME_LAYER_H__

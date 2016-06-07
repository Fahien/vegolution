#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

static const float deltaMin = 0.0625f;

static const float deltaMax = 0.015625f;

#include "cocos2d.h"
#include "data/DataManager.h"
#include "factory/EnemyFactory.h"
#include "factory/GameFactory.h"
#include "factory/HudFactory.h"
#include "controller/GameController.h"
#include "model/MainActor.h"
#include "ui/CocosGUI.h"

class GameScene : public cocos2d::Scene {
public:
    GameScene(DataManager& data, TextFactory& textFactory);

    ~GameScene();

    static GameScene* create(DataManager& data, TextFactory& textFactory);

    /// Init method
    virtual bool init(DataManager& data, TextFactory& textFactory);

    virtual void willEnterForeground();

    /// Update method
    void update(float delta);

    void scheduleSpawning(MainActor* actor, EnemyFactory& enemyFactory, GameFactory& gameFactory);

    void listenPlayerBullet();

    void listenEnemyBullet(DataManager& data, TextFactory& textFactory);

private:
    GameFactory factory_;
    HudFactory hudFactory_;
    EnemyFactory enemyFactory_;
    int score_{ 0 };
    cocos2d::ui::Text* scoreText_{ nullptr };
    cocos2d::Layer* menuLayer_{ nullptr };
	GameController* controller_{ nullptr };

    MainActor* actor_{ nullptr };
    cocos2d::Sprite* board_{ nullptr };
    float centerX_;
    float offsetX_;

    bool pause_;
};

#endif // __GAME_LAYER_H__

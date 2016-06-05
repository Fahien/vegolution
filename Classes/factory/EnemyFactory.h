#ifndef __ENEMY_FACTORY_H__
#define __ENEMY_FACTORY_H__

#include "cocos2d.h"
#include "data/DataManager.h"

class EnemyFactory
{
public:
	EnemyFactory(DataManager* data, std::vector<Bullet*>& bullets);
    ~EnemyFactory();

    Enemy* spawn(cocos2d::Vec2 position);
	void despawn(Enemy* enemy);

private:
    std::vector<Enemy*> enemies_;
    std::vector<Enemy*> enemyPool_;
};

#endif // __ENEMY_FACTORY_H__

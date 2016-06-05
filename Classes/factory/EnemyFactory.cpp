#include "EnemyFactory.h"

USING_NS_CC;

EnemyFactory::EnemyFactory(DataManager* data, std::vector<Bullet*>& bullets)
{
    log("Creating EnemyFactory");
	data->loadEnemies(enemies_, bullets);
	for (Enemy* enemy : enemies_) { enemyPool_.push_back(enemy); }	// Fill enemy pool
}

EnemyFactory::~EnemyFactory()
{
    log("Destructing EnemyFactory");
	log("Releasing enemies");
	for (Enemy* enemy : enemies_) enemy->release();
}

// Get an enemy from the pool
Enemy* EnemyFactory::spawn(Vec2 position)
{
	if (enemyPool_.empty()) { return nullptr; }	// No enemies available

	Enemy* enemy{ enemyPool_.front() };
	enemy->spawn(position);
    enemyPool_.erase(enemyPool_.begin());
    return enemy;
}

// Return an enemy to the pool
void EnemyFactory::despawn(Enemy* enemy)
{
	enemy->remove();
	enemyPool_.push_back(enemy);
}
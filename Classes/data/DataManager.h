#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__

static const char *const selectAllFromBullet = "SELECT * FROM bullet";

#include "cocos2d.h"
#include "sqlite3.h"
#include "model/Bullet.h"
#include "model/Vehicle.h"
#include "model/Enemy.h"

class DataManager {
public:
    DataManager();
    ~DataManager();

	/// Initialize with FileUtils instance
    void init(cocos2d::FileUtils* fileUtils_);

    /// Open data connection
    int open();
    /// Close data connection
    void close();
    /// Release bullets, vehicles and enemies
	void release();

	std::vector<Bullet*>& getBullets();
	std::vector<Vehicle*>& getVehicles();
    std::vector<Enemy*>& getEnemies();

	Bullet* getBullet(std::string& name);

    std::string getString(std::string key);

private:
    cocos2d::FileUtils* fileUtils_;
    sqlite3* db_;

	std::vector<Bullet*> bullets_;
	std::vector<Vehicle*> vehicles_;
	std::vector<Enemy*> enemies_;
};

#endif // __DATA_MANAGER_H__

#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__

#include "cocos2d.h"
#include "sqlite3.h"
#include "model/Bullet.h"
#include "model/Vehicle.h"
#include "model/Enemy.h"

class DataManager
{
public:
    DataManager();
    ~DataManager();

    void init(cocos2d::FileUtils* fileUtils_);

    int open();
    void close();

	void release();

	std::vector<Bullet*>& getBullets();
	std::vector<Vehicle*>& getVehicles();
    std::vector<Enemy*>& getEnemies();

	Bullet* getBullet(std::string& bulletname);
	Vehicle* getVehicle(std::string& vehiclename);

    void save(std::string key, std::string value);
    std::string getString(std::string key);

private:
    cocos2d::FileUtils* fileUtils_;
    sqlite3*            db_;

	std::vector<Bullet*>  bullets_;
	std::vector<Vehicle*> vehicles_;
	std::vector<Enemy*> enemies_;
};

#endif // __DATA_MANAGER_H__

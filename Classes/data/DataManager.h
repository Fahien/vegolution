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

    /// Load all bullets
    void loadBullets(std::vector<Bullet*>& bullets);

    /// Load all vehicles
	void loadVehicles(std::vector<Vehicle*>& vehicles, std::vector<Bullet*>& bullets);

    /// Load all enemies
    void loadEnemies(std::vector<Enemy*>& enemies, std::vector<Bullet*>& bullets);

    std::string getString(std::string key) const;
    int getInteger(std::string key) const;
    void saveInteger(std::string key, int value);

protected:
    /// Open data connection
    int open(sqlite3** db) const;
    /// Close data connection
    void close(sqlite3* db) const;

	/// Get a bullet
	Bullet* getBullet(std::string& name, std::vector<Bullet*>& bullets);
};

#endif // __DATA_MANAGER_H__

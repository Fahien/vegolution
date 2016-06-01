#include "DataManager.h"

USING_NS_CC;

DataManager::DataManager()
	: fileUtils_{ nullptr }
	, db_{ nullptr }
{}


DataManager::~DataManager() {
	// Close the connection
	close();
	// Release enemies
	log("Releasing enemies");
	for (Enemy* enemy : enemies_) {
		enemy->release();
	}
	// Release vehicles
	log("Releasing vehicles");
	for (Vehicle* vehicle : vehicles_) {
		vehicle->release();
	}
	// Release bullets
	log("Releasing bullets");
	for (Bullet* bullet : bullets_) {
		bullet->release();
	}
}

void DataManager::init(FileUtils* fileUtils)
{
    fileUtils_ = fileUtils;

    // Open a connection
	int open_result{ this->open() };
    if (open_result != SQLITE_OK) {
        log("Could not open database file %d: %s", open_result, sqlite3_errmsg(db_));
        return;
    }

	/*
    sqlite3_stmt* statement;
    // Create data table
    std::string sql {"CREATE TABLE IF NOT EXISTS data(key TEXT PRIMARY KEY, value TEXT);"};
    // Prepare the statement
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &statement, nullptr) != SQLITE_OK) {
        log("Could not prepare the statement: %s", sqlite3_errmsg(db_));
    }
    else {
        // Execute the statement
        if (sqlite3_step(statement) != SQLITE_DONE) {
            log("Could not create data table: %s", sqlite3_errmsg(db_));
        }
    }

    // Reset the statement
    sqlite3_reset(statement);
    // Destroy the statement
    sqlite3_finalize(statement);
	*/
}

int DataManager::open()
{
    log("Opening Data");
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    // In Android get a writable path
    std::string path {fileUtils_->getWritablePath() + "data.db"};
    // Get data from the internal database
    log("Creating data");
    Data data {fileUtils_->getDataFromFile("data.db")};
    // Write data to an external database
    fileUtils_->writeDataToFile(data, path);
    // return sqlite3_open_v2(path.c_str(), &db_, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL) == SQLITE_OK;
    return sqlite3_open(path.c_str(), &db_);
#else
    std::string path {fileUtils_->fullPathForFilename("data.db")};
    return sqlite3_open(path.c_str(), &db_);
#endif
}

void DataManager::close()
{
    log("Closing Data");
    sqlite3_close(db_);
}

std::vector<Bullet*>& DataManager::getBullets()
{
	// Check if already loaded
	if (!bullets_.empty()) return bullets_;

	// Initialize variables
	sqlite3_stmt* statement{ nullptr };
	std::string sql{ "SELECT * FROM bullet" };

	// Prepare the statement
	if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &statement, NULL) != SQLITE_OK) {
		log("Could not prepare SELECT: %s", sqlite3_errmsg(db_));
	}
	else {
		// Step
		while (sqlite3_step(statement) == SQLITE_ROW) {
			// Get name
			const unsigned char* name = sqlite3_column_text(statement, 0);
			std::string imagename = std::string((char*)name);
			// Get damage
			int damage = sqlite3_column_int(statement, 1);
			// Get velocity
			int velocity = sqlite3_column_int(statement, 2);
			log("Found Bullet[%s][%d][%d]", name, damage, velocity);
			// Create the bullet
			Bullet* bullet = Bullet::create(imagename, damage, velocity);
			bullets_.push_back(bullet);
		}
	}

	// Reset the statement
	sqlite3_reset(statement);
	// Destroy the statement
	sqlite3_finalize(statement);
	// Return the bullets
	return bullets_;
}

std::vector<Vehicle*>& DataManager::getVehicles()
{
	// Check if already loaded
	if (!vehicles_.empty()) return vehicles_;

	// Initialize variables
	sqlite3_stmt* statement{ nullptr };
	std::string sql{ "SELECT * FROM vehicle" };

	// Prepare the statement
	if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &statement, NULL) != SQLITE_OK) {
		log("Could not prepare SELECT: %s", sqlite3_errmsg(db_));
	}
	else {
		// Step
		while (sqlite3_step(statement) == SQLITE_ROW) {
			// Get name
			const unsigned char* name = sqlite3_column_text(statement, 0);
			std::string vehiclename = std::string((char*)name);
			// Get health
			int health = sqlite3_column_int(statement, 1);
			// Get velocity
			int velocity = sqlite3_column_int(statement, 2);
			// Get delay
			int delay = sqlite3_column_int(statement, 3);
			// Get bullet
			const unsigned char* bname = sqlite3_column_text(statement, 4);
			std::string bulletname = std::string((char*)bname);
			log("Found Vehicle[%s][%d][%d][%s]", name, health, velocity, bname);
			Bullet* bullet = getBullet(bulletname);
			// Get offset
			int offsetX = sqlite3_column_int(statement, 5);
			int offsetY = sqlite3_column_int(statement, 6);
			Vec2 offset = Vec2{ static_cast<float>(offsetX), static_cast<float>(offsetY) };
			// Create the Vehicle
			Vehicle* vehicle = Vehicle::create(vehiclename, health, velocity, delay, bullet, offset);

			// Create animation
			Animation* animation{ Animation::create() };
			for (int i{ 0 }; i < 8; i++) {
				std::string filename = StringUtils::format("vehicle/%s/%s%d.png", vehiclename.c_str(), vehiclename.c_str(), i);
				if (!fileUtils_->isFileExist(filename)) break;
				animation->addSpriteFrameWithFile(filename);
			}
			animation->setDelayPerUnit(0.0625f);
			animation->setRestoreOriginalFrame(true);
			// Loop forever
			animation->setLoops(-1);
			vehicle->runAction(Animate::create(animation));

			// Save vehicle
			vehicles_.push_back(vehicle);
		}
	}

	// Reset the statement
	sqlite3_reset(statement);
	// Destroy the statement
	sqlite3_finalize(statement);
	// Return the vehicles
	return vehicles_;
}

std::vector<Enemy*>& DataManager::getEnemies()
{
	// Check if already loaded
	if (!enemies_.empty()) return enemies_;

    // Initialize variables
	sqlite3_stmt* statement{ nullptr };
	std::string sql{ "SELECT * FROM enemy" };

    // Prepare the statement
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &statement, NULL) != SQLITE_OK) {
        log("Could not prepare SELECT: %s", sqlite3_errmsg(db_));
    }
    else {
        // Step
        while (sqlite3_step(statement) == SQLITE_ROW) {
			// Get the name
            const unsigned char* name = sqlite3_column_text(statement, 0);
            std::string enemyname = std::string((char*)name);
			// Get health
			int health = sqlite3_column_int(statement, 1);
			// Get velocity
			int velocity = sqlite3_column_int(statement, 2);
			// Get delay
			int delay = sqlite3_column_int(statement, 3);
			// Get bullet name
			const unsigned char* bname = sqlite3_column_text(statement, 4);
			std::string bulletname = std::string((char*)bname);
			log("Found Enemy[%s][%d][%d][%d][%s]", name, health, velocity, delay, bname);
			// Get the bullet
			Bullet* bullet = getBullet(bulletname);
			// Create the Enemy
			Enemy* enemy = Enemy::create(enemyname, health, velocity, delay, bullet);
            enemies_.push_back(enemy);
        }
    }

    // Reset the statement
    sqlite3_reset(statement);
    // Destroy the statement
    sqlite3_finalize(statement);
    // Return the enemies
    return enemies_;
}


Bullet* DataManager::getBullet(std::string& bulletname)
{
	for (Bullet* bullet : getBullets()) {
		if (bullet->getName() == bulletname) return bullet;
	}
	log("Bullet %s not found!", bulletname.c_str());
	return nullptr;
}

Vehicle* DataManager::getVehicle(std::string& vehiclename)
{
	for (Vehicle* vehicle : getVehicles()) {
		if (vehicle->getName() == vehiclename) return vehicle;
	}
	log("Vehicle %s not found!", vehiclename.c_str());
	return nullptr;
}
void DataManager::save(std::string key, std::string value) {}

std::string DataManager::getString(std::string key) {
    // Initialize variables
	std::string value{ "Unknown" };
	sqlite3_stmt* statement{ nullptr };
	std::string sql{ "SELECT VALUE FROM string_en WHERE key=?" };

    // Prepare the statement
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &statement, NULL) != SQLITE_OK) {
        log("Could not prepare SELECT: %s", sqlite3_errmsg(db_));
    }
    else {
        // Bind key parameter
        sqlite3_bind_text(statement, 1, key.c_str(), -1, SQLITE_TRANSIENT);
        // Step
        if (sqlite3_step(statement) != SQLITE_ROW) {
            log("Could not step SELECT: %s", sqlite3_errmsg(db_));
        }
        else {
            const unsigned char* val {sqlite3_column_text(statement, 0)};
            value = std::string((char*)val);
            log("Found Key[%s] Value[%s]", key.c_str(), val);
        }
    }

    // Reset the statement
    sqlite3_reset(statement);
    // Destroy the statement
    sqlite3_finalize(statement);
    // Return the value
    return value;
}

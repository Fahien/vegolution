#include "DataManager.h"

USING_NS_CC;

DataManager::DataManager()
{
    log("Constructing DataManager");
}

DataManager::~DataManager()
{
    log("Destructing DataManager");
}

int DataManager::open(sqlite3 **db) const
{
    FileUtils* fileUtils{ FileUtils::getInstance() };
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    // In Android get a writable path
    std::string path{ fileUtils->getWritablePath() + "save.db" };
    // If file does not exist
    if (!fileUtils->isFileExist(path)) {
        // Get data from the internal database
        log("Creating data");
        Data data{ fileUtils->getDataFromFile("data.db") };
        // Write data to an external database
        fileUtils->writeDataToFile(data, path);
    }
    // return sqlite3_open_v2(path.c_str(), db, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL) == SQLITE_OK;
    return sqlite3_open(path.c_str(), db);
#else
    std::string path{ fileUtils->fullPathForFilename("data.db") };
    if (!fileUtils->isFileExist(path)) {
        log("Database does not exists at %s", path.c_str());
        Director::getInstance()->end();
    }
    // Open a connection
    int open_result{ sqlite3_open(path.c_str(), db) };
    // Check open error
    if (open_result != SQLITE_OK) {
        log("Could not open database file %d: %s", open_result, sqlite3_errmsg(*db));
    }
    return open_result;
#endif
}

void DataManager::close(sqlite3 *db) const
{
    log("Closing Data");
    sqlite3_close(db);
}

void DataManager::loadBullets(std::vector<Bullet *> &bullets)
{
    log("Loading bullets");
    // Open connection
    sqlite3 *db{ nullptr };
    open(&db);

    // Initialize variables
    sqlite3_stmt *statement{ nullptr };
    std::string sql{ selectAllFromBullet };

    // Prepare the statement
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, NULL) != SQLITE_OK) {
        log("Could not prepare SELECT: %s", sqlite3_errmsg(db));
    }
    else {
        // Step
        while (sqlite3_step(statement) == SQLITE_ROW) {
            // Get name
            const unsigned char *nam = sqlite3_column_text(statement, 0);
            std::string name = std::string((char *) nam);
            // Get damage
            int damage = sqlite3_column_int(statement, 1);
            // Get velocity
            float velocity = static_cast<float>(sqlite3_column_int(statement, 2));
            log("Found Bullet[%s][%d][%f]", nam, damage, velocity);
            // Create the bullet
            Bullet *bullet = Bullet::create(name, damage, velocity);
            bullets.push_back(bullet);
        }
    }

    // Reset the statement
    sqlite3_reset(statement);
    // Destroy the statement
    sqlite3_finalize(statement);
    // Close connection
    close(db);
}

void DataManager::loadVehicles(std::vector<Vehicle *> &vehicles, std::vector<Bullet *> &bullets)
{
    log("Loading vehicles");
    // Open connection
    sqlite3 *db{ nullptr };
    open(&db);

    // Initialize variables
    sqlite3_stmt *statement{ nullptr };
    std::string sql{ "SELECT * FROM vehicle" };

    // Prepare the statement
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, NULL) != SQLITE_OK) {
        log("Could not prepare SELECT: %s", sqlite3_errmsg(db));
    }
    else {
        // Step
        while (sqlite3_step(statement) == SQLITE_ROW) {
            // Get name
            const unsigned char *nam = sqlite3_column_text(statement, 0);
            std::string name = std::string((char *) nam);
            // Get health
            int health = sqlite3_column_int(statement, 1);
            // Get velocity
            int velocity = sqlite3_column_int(statement, 2);
            // Get delay
            int delay = sqlite3_column_int(statement, 3);
            // Get bullet
            const unsigned char *bname = sqlite3_column_text(statement, 4);
            std::string bulletName = std::string((char *) bname);
            log("Found Vehicle[%s][%d][%d][%s]", nam, health, velocity, bname);
            Bullet *bullet = getBullet(bulletName, bullets);
            // Get offset
            int offsetX = sqlite3_column_int(statement, 5);
            int offsetY = sqlite3_column_int(statement, 6);
            Vec2 offset = Vec2{ static_cast<float>(offsetX), static_cast<float>(offsetY) };
            // Get gravity enable
            bool gravity = sqlite3_column_int(statement, 7) == 1;

            // Create the Vehicle
            Vehicle *vehicle = Vehicle::create(name, health, velocity, delay, bullet, offset, gravity);

            // Create animation
            Animation *animation{ Animation::create() };
            for (int i{ 0 }; i < 32; i++) {
                std::string filename = StringUtils::format("vehicle/%s/%s%d.png", nam, nam, i);
                if (!FileUtils::getInstance()->isFileExist(filename)) break;
                animation->addSpriteFrameWithFile(filename);
            }
            animation->setDelayPerUnit(0.0625f);
            animation->setRestoreOriginalFrame(true);
            Animate *animate{ Animate::create(animation) };
            vehicle->runAction(RepeatForever::create(animate));

            // Save vehicle
            vehicles.push_back(vehicle);
        }
    }

    // Reset the statement
    sqlite3_reset(statement);
    // Destroy the statement
    sqlite3_finalize(statement);
    // Close db
    close(db);
}

void DataManager::loadEnemies(std::vector<Enemy *> &enemies, std::vector<Bullet *> &bullets)
{
    // Open connection
    sqlite3 *db{ nullptr };
    open(&db);

    // Initialize variables
    sqlite3_stmt *statement{ nullptr };
    std::string sql{ "SELECT * FROM enemy" };

    // Prepare the statement
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, NULL) != SQLITE_OK) {
        log("Could not prepare SELECT: %s", sqlite3_errmsg(db));
    }
    else {
        // Step
        while (sqlite3_step(statement) == SQLITE_ROW) {
            // Get the name
            const unsigned char *nam = sqlite3_column_text(statement, 0);
            std::string enemyName = std::string((char *) nam);
            // Get health
            int health = sqlite3_column_int(statement, 1);
            // Get velocity
            int velocity = sqlite3_column_int(statement, 2);
            // Get delay
            int delay = sqlite3_column_int(statement, 3);
            // Get bullet name
            const unsigned char *bname = sqlite3_column_text(statement, 4);
            std::string bulletName = std::string((char *) bname);
            log("Found Enemy[%s][%d][%d][%d][%s]", nam, health, velocity, delay, bname);
            // Get the bullet
            Bullet *b = getBullet(bulletName, bullets);
            Bullet *bullet{ nullptr };
            // Clone bullet
            if (b) bullet = Bullet::create(b->getName(), b->getDamage(), b->getVelocity());
            // Get gravity
            bool gravity = sqlite3_column_int(statement, 5) == 1;
            // Get y
            int y = sqlite3_column_int(statement, 6);
            // Create the Enemy
            Enemy *enemy = Enemy::create(enemyName, health, velocity, delay, bullet, gravity, y);

            // Create animation
            FileUtils *fileUtils = FileUtils::getInstance();
            std::string filename = StringUtils::format("enemy/%s/%s1.png", nam, nam);
            if (fileUtils->isFileExist(filename)) {
                Animation *animation{ Animation::create() };
                for (int i{ 0 }; i < 32; i++) {
                    filename = StringUtils::format("enemy/%s/%s%d.png", nam, nam, i);
                    if (!fileUtils->isFileExist(filename)) break;
                    animation->addSpriteFrameWithFile(filename);
                }
                animation->setDelayPerUnit(0.0625f);
                animation->setRestoreOriginalFrame(true);
                Animate *animate{ Animate::create(animation) };
                enemy->runAction(RepeatForever::create(animate));
            }

            enemies.push_back(enemy);
        }
    }

    // Reset the statement
    sqlite3_reset(statement);
    // Destroy the statement
    sqlite3_finalize(statement);
    // Close connection
    close(db);
}


Bullet *DataManager::getBullet(std::string &name, std::vector<Bullet *> &bullets)
{
    for (Bullet *bullet : bullets) {
        if (bullet->getName() == name) return bullet;
    }
    log("Bullet %s not found!", name.c_str());
    return nullptr;
}

std::string DataManager::getString(std::string key) const
{
    // Initialize variables
    std::string value{ "Unknown" };

    // Open connection
    sqlite3 *db{ nullptr };
    open(&db);

    sqlite3_stmt *statement{ nullptr };
    std::string sql{ "SELECT VALUE FROM string_en WHERE key=?" };

    // Prepare the statement
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, NULL) != SQLITE_OK) {
        log("Could not prepare SELECT: %s", sqlite3_errmsg(db));
    }
    else {
        // Bind key parameter
        sqlite3_bind_text(statement, 1, key.c_str(), -1, SQLITE_TRANSIENT);
        // Step
        if (sqlite3_step(statement) != SQLITE_ROW) {
            log("Could not step SELECT: %s", sqlite3_errmsg(db));
        }
        else {
            const unsigned char *val{ sqlite3_column_text(statement, 0) };
            value = std::string((char *) val);
            log("Found Key[%s] Value[%s]", key.c_str(), val);
        }
    }

    // Reset the statement
    sqlite3_reset(statement);
    // Destroy the statement
    sqlite3_finalize(statement);
    // Close connection
    close(db);
    // Return the value
    return value;
}


int DataManager::getInteger(std::string key) const
{
    // Initialize variables
    int value{ 0 };

    // Open connection
    sqlite3 *db{ nullptr };
    open(&db);

    sqlite3_stmt *statement{ nullptr };
    std::string sql{ "SELECT value FROM integer WHERE key=?" };

    // Prepare the statement
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, nullptr) != SQLITE_OK) {
        log("Could not prepare SELECT: %s", sqlite3_errmsg(db));
    }
    else {
        // Bind key parameter
        sqlite3_bind_text(statement, 1, key.c_str(), -1, SQLITE_TRANSIENT);
        // Step
        if (sqlite3_step(statement) != SQLITE_ROW) {
            log("Could not step SELECT: %s", sqlite3_errmsg(db));
        }
        else {
            value = sqlite3_column_int(statement, 0);
            log("Found Key[%s] Value[%d]", key.c_str(), value);
        }
    }

    // Reset the statement
    sqlite3_reset(statement);
    // Destroy the statement
    sqlite3_finalize(statement);
    // Close connection
    close(db);
    // Return the value
    return value;
}

void DataManager::saveInteger(std::string key, int value)
{
    // Open connection
    sqlite3 *db{ nullptr };
    open(&db);

    sqlite3_stmt *statement{ nullptr };
    std::string sql{ "UPDATE integer SET value=? WHERE key=?" };

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, nullptr) != SQLITE_OK) {
        log("Could not prepare INSERT: %s", sqlite3_errmsg(db));
    }
    else {
        // Bind value parameter
        sqlite3_bind_int(statement, 1, value);
        // Bind key parameter
        sqlite3_bind_text(statement, 2, key.c_str(), -1, SQLITE_TRANSIENT);
        // Step!
        if (sqlite3_step(statement) != SQLITE_DONE) {
            log("Could not step INSERT: %s", sqlite3_errmsg(db));
        }
        else {
            log("Saved Key[%s] Value[%d]", key.c_str(), value);
        }
    }

    // Reset the statement
    sqlite3_reset(statement);
    // Destroy the statement
    sqlite3_finalize(statement);
    // Close connection
    close(db);
}
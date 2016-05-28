#include "DataManager.h"

USING_NS_CC;

DataManager::DataManager()
: fileUtils_ {nullptr}
, db_        {nullptr}
{}

void DataManager::init(FileUtils* fileUtils)
{
    fileUtils_ = fileUtils;

    // Open a connection
    int open_result {this->open()};
    if (open_result != SQLITE_OK) {
        log("Could not open database file %d: %s", open_result, sqlite3_errmsg(db_));
        return;
    }

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
}

DataManager::~DataManager() {
    // Close the connection
    this->close();
}

int DataManager::open()
{
    log("Opening Data");
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    // In Android get a writable path
    std::string path {fileUtils_->getWritablePath() + "test.db"};
    // Get data from the internal database
    log("Creating data");
    Data data {fileUtils_->getDataFromFile("test.db")};
    // Write data to an external database
    fileUtils_->writeDataToFile(data, path);
    // return sqlite3_open_v2(path.c_str(), &db_, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL) == SQLITE_OK;
    return sqlite3_open(path.c_str(), &db_);
#else
    std::string path {fileUtils_->fullPathForFilename("test.db")};
    return sqlite3_open(path.c_str(), &db_);
#endif
}

void DataManager::close()
{
    log("Closing Data");
    sqlite3_close(db_);
}

std::vector<std::string> DataManager::getEnemies()
{
    // Initialize variables
    std::vector<std::string> enemies {};
    sqlite3_stmt* statement {nullptr};
    std::string sql {"SELECT name FROM enemy"};

    // Prepare the statement
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &statement, NULL) != SQLITE_OK) {
        log("Could not prepare SELECT: %s", sqlite3_errmsg(db_));
    }
    else {
        // Step
        while (sqlite3_step(statement) == SQLITE_ROW) {
            const unsigned char* val = sqlite3_column_text(statement, 0);
            std::string value = std::string((char*)val);
            log("Found Enemy[%s]", val);
            enemies.push_back(value);
        }
    }

    // Reset the statement
    sqlite3_reset(statement);
    // Destroy the statement
    sqlite3_finalize(statement);
    // Return the enemy names
    return enemies;
}

void DataManager::save(std::string key, std::string value) {}

std::string DataManager::get(std::string key) {
    // Initialize variables
    std::string value {"Unknown"};
    sqlite3_stmt* statement {nullptr};
    std::string sql {"SELECT VALUE FROM string_en WHERE key=?"};

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

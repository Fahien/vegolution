#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__

#include "cocos2d.h"
#include "sqlite3.h"

class DataManager
{
public:
    DataManager();
    ~DataManager();

    void init(cocos2d::FileUtils* fileUtils_);

    int open();
    void close();

    void save(std::string key, std::string value);
    std::string get(std::string key);
private:
    cocos2d::FileUtils* fileUtils_;
    sqlite3*            db_;
};

#endif // __DATA_MANAGER_H__

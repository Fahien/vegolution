#ifndef __SETTINGS_FACTORY_H__
#define __SETTINGS_FACTORY_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "data/DataManager.h"

class SettingsFactory {
public:
    SettingsFactory(DataManager* data);

    cocos2d::Sprite* createBackground();

    cocos2d::ui::Layout* createMenu();

protected:
    cocos2d::ui::Text* createBackText();

private:
    cocos2d::Size visibleSize_;
    DataManager* data_;
    std::string fontPath_;
    float fontSize_;
    cocos2d::Size textContentSize_;
    cocos2d::ui::LinearLayoutParameter* layout_;
    cocos2d::Size shadowOffset_;
    int shadowBlur_;

    cocos2d::Sprite* background_;
    cocos2d::ui::Text* back_;
    cocos2d::ui::Layout* menu_;
};

#endif // __SETTINGS_FACTORY_H__

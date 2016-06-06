#ifndef __SETTINGS_FACTORY_H__
#define __SETTINGS_FACTORY_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "data/DataManager.h"

class SettingsFactory {
public:
    SettingsFactory(DataManager* data);

    inline cocos2d::Sprite* const getBackground() { return background_; }

    inline cocos2d::ui::Layout* getMenu() { return menu_; }

    inline cocos2d::Sprite* getBoard() { return board_; }

protected:
    void createBackground();

    void createMenu();

    void createBoard();

    void createResolution(DataManager* data);

    void createAudio(DataManager* data);

    void createBackText(DataManager* data);

private:
    cocos2d::Size visibleSize_;
    std::string fontPath_;
    float fontSize_;
    cocos2d::Size textContentSize_;
    cocos2d::ui::LinearLayoutParameter* layout_;
    cocos2d::Size shadowOffset_;
    int shadowBlur_;

    cocos2d::Sprite* background_;
    cocos2d::ui::Text* resolution_;
    cocos2d::ui::Text* audio_;
    cocos2d::ui::Text* back_;
    cocos2d::ui::Layout* menu_;
    cocos2d::Sprite* board_;

};

#endif // __SETTINGS_FACTORY_H__

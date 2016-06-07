#ifndef __SETTINGS_FACTORY_H__
#define __SETTINGS_FACTORY_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "data/DataManager.h"
#include "TextFactory.h"

class SettingsFactory {
public:
    SettingsFactory(DataManager& data, TextFactory& textFactory);

    inline cocos2d::Sprite* const getBackground() { return background_; }

    inline cocos2d::ui::Layout* getMenu() { return menu_; }

    inline cocos2d::Sprite* getBoard() { return board_; }

protected:
    void createBackground(cocos2d::Size& visibleSize);

    void createMenu(cocos2d::Vec2& center);

    void createBoard(cocos2d::Size& visibleSize);

    void createResolution(TextFactory& textFactory, DataManager& data);

    void createAudio(TextFactory& textFactory, DataManager& data);

    void createBackText(std::string name, TextFactory& textFactory, DataManager& data);

private:
    cocos2d::Sprite* background_;
    cocos2d::ui::Text* resolution_;
    cocos2d::ui::Text* audio_;
    cocos2d::ui::Text* back_;
    cocos2d::ui::Layout* menu_;
    cocos2d::Sprite* board_;

};

#endif // __SETTINGS_FACTORY_H__

#ifndef __MAIN_FACTORY_H__
#define __MAIN_FACTORY_H__

#include "cocos2d.h"
#include "data/DataManager.h"
#include "TextFactory.h"
#include "ui/CocosGUI.h"

class MainFactory {
public:
    MainFactory(DataManager& data, TextFactory& textFactory);

    ~MainFactory();

    inline cocos2d::Sprite* getBackground() const
    { return background_; }

    inline cocos2d::ui::Layout* getMenu() const
    { return menu_; }

    inline cocos2d::Sprite* getBoard() const
    { return board_; }

protected:
    void createBackground(cocos2d::Size& visibleSize, cocos2d::Vec2& center);

    void createPlayText(std::string name, DataManager& data, TextFactory& textFactory);

    void createSettingsText(std::string name, DataManager& data, TextFactory& textFactory);

    void createQuitText(std::string name, TextFactory& textFactory);

    void createMenu(cocos2d::Vec2& center);

    void createBoard(cocos2d::Size& visibleSize, cocos2d::Vec2& center);

private:
    // Widgets
    cocos2d::Sprite* background_;
    cocos2d::ui::Text* play_;
    cocos2d::ui::Text* settings_;
    cocos2d::ui::Text* quit_;
    cocos2d::ui::Layout* menu_;
    cocos2d::Sprite* board_;
};

#endif // __MAIN_FACTORY_H__

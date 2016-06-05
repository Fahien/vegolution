#ifndef __MAIN_FACTORY_H__
#define __MAIN_FACTORY_H__

#include "cocos2d.h"
#include "data/DataManager.h"
#include "ui/CocosGUI.h"

class MainFactory {
public:
    MainFactory(DataManager* data);

    ~MainFactory();

    inline cocos2d::Sprite* getBackground() const { return background_; }

    inline cocos2d::ui::Layout* getMenu() const { return menu_; }

    inline cocos2d::Sprite* getBoard() const { return board_; }

protected:
    void createBackground();

    void createPlayText(std::string name, DataManager* data);

    void createSettingsText(std::string name, DataManager* data);

    void createQuitText(std::string name);

    void createMenu();

    void createBoard();
private:
    cocos2d::Size visibleSize_;
    cocos2d::Vec2 center_;

    // Text settings
    std::string fontPath_;
    float fontSize_;
    cocos2d::Size shadowOffset_;
    int shadowBlur_;
    cocos2d::Size textContentSize_;

    // Menu settings
    cocos2d::Size menuSize_;
    cocos2d::ui::LinearLayoutParameter *layout_;

    // Widgets
    cocos2d::Sprite *background_{nullptr};
    cocos2d::ui::Text *play_{nullptr};
    cocos2d::ui::Text *settings_{nullptr};
    cocos2d::ui::Text *quit_{nullptr};
    cocos2d::ui::Layout *menu_{nullptr};
    cocos2d::Sprite *board_{nullptr};
};

#endif // __MAIN_FACTORY_H__

#ifndef __MAIN_FACTORY_H__
#define __MAIN_FACTORY_H__

#include "cocos2d.h"
#include "Vegolution.h"
#include "ui/CocosGUI.h"

class MainFactory {
public:
    MainFactory(Vegolution *game);

    ~MainFactory();

    cocos2d::Sprite *createBackground();

    cocos2d::ui::Text *createPlayText();

    cocos2d::ui::Text *createSettingsText();

    cocos2d::ui::Text *createQuitText();

    cocos2d::ui::Layout *createMenu();

    cocos2d::Sprite *createBoard();

private:
    Vegolution *game_;
    cocos2d::Director *director_;
    DataManager *data_;

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

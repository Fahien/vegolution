#ifndef __SPLASH_FACTORY_H__
#define __SPLASH_FACTORY_H__

#include "data/DataManager.h"
#include "factory/TextFactory.h"

class SplashFactory
{
public:
    SplashFactory();
    ~SplashFactory();

    cocos2d::Sequence* createSequence(DataManager& data, TextFactory& text);

protected:
    void createRFPGlow(cocos2d::Vec2& center);
    void createRFPLogo(cocos2d::Vec2& center);
    void createVegolutionLogo(cocos2d::Vec2& center);
    void createEarth(cocos2d::Size& visibleSize, cocos2d::Vec2& center);
    void createBoard(cocos2d::Size& visibleSize, cocos2d::Vec2& center);

    void createLayout(cocos2d::Size& visibleSize, DataManager& data, TextFactory& text);

    void createStory(DataManager& data, TextFactory& text);

    void nextScene(DataManager& data, TextFactory& text);


private:
    cocos2d::Sprite* rfpGlow_;
    cocos2d::Sprite* rfpLogo_;
    cocos2d::Sprite* vegLogo_;
    cocos2d::Sprite* earth_;
    cocos2d::Sprite* board_;

    cocos2d::ui::Layout* layout_;

    std::vector<cocos2d::ui::Text*> story_;

    cocos2d::Sequence* sequence_;
};
#endif // __SPLASH_FACTORY_H__

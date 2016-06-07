#ifndef __TEXT_FACTORY_H__
#define __TEXT_FACTORY_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "data/DataManager.h"

class TextFactory {
public:
    TextFactory();
    ~TextFactory();
    void init(const DataManager& data);

    inline cocos2d::Size& getVisibleSize() { return visibleSize_; }
    inline cocos2d::Vec2& getOrigin() { return origin_; }
    inline cocos2d::Vec2& getCenter() { return center_; }

    inline std::string& getFontPath() { return fontPath_; }
    inline float getFontSize() const { return fontSize_; }
    inline cocos2d::Size& getTextContentSize() { return textContentSize_; }
    inline cocos2d::ui::LinearLayoutParameter* getLinearLayoutParameter() const { return layout_; }
    inline cocos2d::Size& getShadowOffset() { return shadowOffset_; }
    inline int getShadowBlur() const { return shadowBlur_; }

    /// Create a base Text for Vegolution
    virtual cocos2d::ui::Text* createText(std::string text);
    virtual cocos2d::ui::Text* createText(std::string text, float fontSize);
private:
    cocos2d::Size visibleSize_;
    cocos2d::Vec2 origin_;
    cocos2d::Vec2 center_;
    std::string fontPath_;
    float fontSize_;
    cocos2d::Size textContentSize_;
    cocos2d::ui::LinearLayoutParameter* layout_;
    cocos2d::Size shadowOffset_;
    int shadowBlur_;
};

#endif // __TEXT_FACTORY_H__

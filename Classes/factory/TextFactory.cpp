#include "TextFactory.h"

USING_NS_CC;

TextFactory::TextFactory(DataManager *data)
        : visibleSize_{ Director::getInstance()->getVisibleSize() }
        , origin_{ Director::getInstance()->getVisibleOrigin() }
        , center_{ visibleSize_.width / 2.0f + origin_.x, visibleSize_.height / 2.0f + origin_.y }
        , fontPath_{ data->getString("font.path") }
        , fontSize_{ static_cast<float>(data->getInteger("font.size")) }
        , textContentSize_{ 64.0f, 24.0f }
        , layout_{ ui::LinearLayoutParameter::create() }
        , shadowOffset_{ 0.0f, -4.0f }
        , shadowBlur_{ 8 }
{
    log("Constructing TextFactory");
    layout_->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    layout_->setMargin(ui::Margin{ 0.0f, 0.0f, 0.0f, 8.0f });
}

ui::Text* TextFactory::createText(std::string name)
{
    ui::Text* text{ ui::Text::create(name, fontPath_, fontSize_)};
    text->setName(name);
    text->setContentSize(textContentSize_);
    text->setPositionX(-textContentSize_.width / 2.0f);
    text->setLayoutParameter(layout_);
    text->enableShadow(Color4B::BLACK, shadowOffset_, shadowBlur_);
    return text;
}
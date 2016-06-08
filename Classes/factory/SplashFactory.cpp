#include "SplashFactory.h"
#include "action/AddChild.h"
#include "scene/MainScene.h"

USING_NS_CC;

SplashFactory::SplashFactory()
        : rfpGlow_{ nullptr }
        , rfpLogo_{ nullptr }
        , earth_{ nullptr }
        , board_{ nullptr }
        , layout_{ nullptr }
        , story_{ }
        , sequence_{ nullptr }
{ }

SplashFactory::~SplashFactory()
{
    if (layout_) layout_->release();
}

void SplashFactory::createRFPGlow(Vec2& center)
{
    std::string glowName{ "splash/rfp-glow.png" };
    rfpGlow_ = Sprite::create(glowName);
    rfpGlow_->setPosition(center);
    FadeOut* fadeOut{ FadeOut::create(0.0f) };
    FadeIn* fadeIn{ FadeIn::create(2.0f) };
    RemoveSelf* remove{ RemoveSelf::create(true) };
    Sequence* sequence{ Sequence::create(fadeOut, fadeIn, remove, nullptr) };
    rfpGlow_->runAction(sequence);
}

void SplashFactory::createRFPLogo(Vec2& center)
{
    std::string rfpName{ "splash/rfp.png" };
    rfpLogo_ = Sprite::create(rfpName);
    rfpLogo_->setPosition(center);
    FadeOut* fadeOut{ FadeOut::create(0.0f) };
    FadeIn* fadeIn{ FadeIn::create(2.0f) };
    RemoveSelf* remove{ RemoveSelf::create(true) };
    Sequence* sequence{ Sequence::create(fadeOut, fadeIn, remove, nullptr) };
    rfpLogo_->runAction(sequence);
}

void SplashFactory::createEarth(Size& visibleSize, Vec2& center)
{
    std::string earthName{ "splash/earth-l.png" };
    earth_ = Sprite::create(earthName);
    float scaleX{ visibleSize.width / earth_->getContentSize().width };
    earth_->setScaleX(scaleX);
    earth_->setPosition(center);
}

void SplashFactory::createBoard(Size& visibleSize, Vec2& center)
{
        log("Creating board");
        board_ = Sprite::create("misc/board.png");
        float scaleX{ visibleSize.width / board_->getContentSize().width };
        board_->setScaleX(scaleX);
        board_->setPosition(center);
        board_->setGlobalZOrder(4);
}

void SplashFactory::createLayout(Size& visibleSize, DataManager& data, TextFactory& text)
{
    layout_ = ui::Layout::create();
    layout_->retain();
    layout_->setContentSize(Size{ visibleSize.width, visibleSize.height * 4.0f / 5.0f });
    layout_->setLayoutType(ui::Layout::Type::VERTICAL);

    if (!story_.empty()) {
        layout_->addChild(story_.back());
        story_.pop_back();
    }

    layout_->setTouchEnabled(true);
    layout_->addTouchEventListener([ this, &data, &text ](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            for (Node* node : layout_->getChildren()) {
                node->removeFromParent();
                node->release();
            }

            if (story_.size() == 4) {
                earth_->setTexture("splash/earth.png");
            } else if (story_.size() == 2) {
                earth_->setTexture("splash/earth-c.png");
            }

            if (!story_.empty()) {
                layout_->addChild(story_.back());
                story_.pop_back();
            }
            else {
                nextScene(data, text);
            }
        }
    });

}

void SplashFactory::createStory(DataManager& data, TextFactory& text)
{
    std::string story{ };
    for (int i{ 5 }; i > 0; --i) {
        std::string key = StringUtils::format("story.%d", i);
        story = data.getString(key);
        ui::Text* storyText = text.createText(story, 18);
        storyText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        storyText->retain();
        story_.push_back(storyText);
    }
}

Sequence* SplashFactory::createSequence(DataManager& data, TextFactory& text)
{
    Size visibleSize { text.getVisibleSize() };
    Vec2 center{ text.getCenter() };
    createRFPGlow(center);
    createRFPLogo(center);
    createEarth(visibleSize, center);
    createBoard(visibleSize,center);

    createStory(data, text);
    createLayout(text.getVisibleSize(), data, text);

    return Sequence::create(
            AddChild::create(board_),
            AddChild::create(rfpGlow_),
            AddChild::create(rfpLogo_),
            DelayTime::create(2.0f),
            AddChild::create(earth_),
            DelayTime::create(2.0f),
            AddChild::create(layout_),
            nullptr);
}

void SplashFactory::nextScene(DataManager& data, TextFactory& text)
{
    MainScene* scene{ MainScene::create(data, text) };
    TransitionFade* transition{ TransitionFade::create(0.5f, scene, Color3B::BLACK) };
    Director::getInstance()->replaceScene(transition);
}
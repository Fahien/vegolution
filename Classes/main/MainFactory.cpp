#include "MainFactory.h"
#include "game/GameLayer.h"
#include "settings/SettingsLayer.h"

USING_NS_CC;

MainFactory::MainFactory(Vegolution* game)
: game_     {game}
, director_ {game->getDirector()}
, data_     {game->getDataManager()}
{
    log("Creagin MainFactory");

    // Set center
    log("Setting center");
    visibleSize_ = director_->getVisibleSize();
    Vec2 origin {director_->getVisibleOrigin()};
    center_.x = visibleSize_.width / 2 + origin.x;
    center_.y = visibleSize_.height / 2 + origin.y;

    // Load and cache the menu sprite fram
    // SpriteFrameCache* cacher {SpriteFrameCache::getInstance()};
    // cacher->addSpriteFramesWithFile("menu.plist");

    // Set text settings
    log("Setting text");
    fontPath_ = std::string{"fonts/Marker Felt.ttf"};
    fontSize_ = 48.0f;
    shadowOffset_ = Size{0.0f, -4.0f};
    shadowBlur_ = 8.0f;
    textContentSize_ = Size{64.0f, 24.0f};
    layout_ = ui::LinearLayoutParameter::create();
    layout_->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    layout_->setMargin(ui::Margin{0.0f, 0.0f, 0.0f, 8.0f});

    // Set menu settings
    log("Setting menu");
    menuSize_ = Size{64.0f, 64.0f};
}

MainFactory::~MainFactory()
{
    log("Destructing MainFactory");
}

Sprite* MainFactory::createBackground()
{
    if (background_ == nullptr) {
        log("Creating background");
        background_ = Sprite::create("main/background.png");
        float scaleX {visibleSize_.width / background_->getContentSize().width};
        background_->setScaleX(scaleX);
        background_->setPosition(center_);
    }
    return background_;
}

ui::Text* MainFactory::createPlayText()
{
	if (play_ == nullptr) {
        log("Creating play");
        // Get localized string
        std::string playText {data_->get("main.play")};
		play_ = ui::Text::create(playText, fontPath_, fontSize_);
		play_->setContentSize(textContentSize_);
		play_->setPositionX(-textContentSize_.width / 2);
		play_->setLayoutParameter(layout_);
		play_->enableShadow(Color4B::BLACK, shadowOffset_, shadowBlur_);
		play_->setTouchEnabled(true);
		play_->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
				ui::Text* target {static_cast<ui::Text*>(sender)};
                Scene* scene {nullptr};
				switch(type) {
				case ui::Widget::TouchEventType::BEGAN :
					target->runAction(ScaleTo::create(0.125f, 1.25f));
					break;
				case ui::Widget::TouchEventType::ENDED :
					target->runAction(ScaleTo::create(0.125f, 1.0f));
                    scene = GameLayer::createScene(this->game_);
                    if (this->director_ == nullptr) log ("Director is null");
                    this->director_->replaceScene(scene);
					break;
				case ui::Widget::TouchEventType::CANCELED :
					target->runAction(ScaleTo::create(0.125f, 1.0f));
					break;
				default: break;
				}
			});
	}
	return play_;
}

ui::Text* MainFactory::createSettingsText()
{
	if (settings_ == nullptr) {
        log("Creating settings");
        // Get localized string
        std::string settingsText {data_->get("main.settings")};
		settings_ = ui::Text::create(settingsText, fontPath_, fontSize_);
		settings_->setContentSize(textContentSize_);
		settings_->setPositionX(-textContentSize_.width / 2);
		settings_->setLayoutParameter(layout_);
		settings_->enableShadow(Color4B::BLACK, shadowOffset_, shadowBlur_);
		settings_->setTouchEnabled(true);
		settings_->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
				ui::Text* target {static_cast<ui::Text*>(sender)};
                Scene* scene {nullptr};
				switch(type) {
				case ui::Widget::TouchEventType::BEGAN :
					target->runAction(ScaleTo::create(0.125f, 1.25f));
					break;
				case ui::Widget::TouchEventType::ENDED :
					target->runAction(ScaleTo::create(0.125f, 1.0f));
                    scene = SettingsLayer::createScene();
                    director_->pushScene(scene);
					break;
				case ui::Widget::TouchEventType::CANCELED :
					target->runAction(ScaleTo::create(0.125f, 1.0f));
					break;
				default: break;
				}
			});
	}
	return settings_;
}

ui::Text* MainFactory::createQuitText()
{
	if (quit_ == nullptr) {
        log("Creating quit");
		std::string quitText {data_->get("main.quit")};
    	quit_ = ui::Text::create(quitText, fontPath_, fontSize_);
    	quit_->setContentSize(textContentSize_);
     	quit_->setPositionX(-textContentSize_.width / 2);
     	quit_->enableShadow(Color4B::BLACK, shadowOffset_, shadowBlur_);
     	quit_->setLayoutParameter(layout_);
     	quit_->setTouchEnabled(true);
     	quit_->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        	ui::Text* target {static_cast<ui::Text*>(sender)};
         	switch (type) {
         	case ui::Widget::TouchEventType::BEGAN :
            	target->runAction(ScaleTo::create(0.125f, 1.25f));
            	break;
         	case ui::Widget::TouchEventType::ENDED :
            	 target->runAction(ScaleTo::create(0.125f, 1.0f));
            	 director_->end();
            	 #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            	 exit(0);
            	 #endif
            	 break;
         	case ui::Widget::TouchEventType::CANCELED :
         	    target->runAction(ScaleTo::create(0.125f, 1.0f));
         	    break;
         	default: break;
         	}
    	});
	}
	return quit_;
}

ui::Layout* MainFactory::createMenu()
{
    if (menu_ == nullptr) {
        log("Creating menu");
        menu_ = ui::Layout::create();
        menu_->setContentSize(menuSize_);
        menu_->setPositionX(center_.x - menuSize_.width / 2);
        menu_->setPositionY(center_.y - menuSize_.height / 2);
        menu_->setLayoutType(ui::Layout::Type::VERTICAL);
    }
    return menu_;
}


Sprite* MainFactory::createBoard()
{
    if (board_ == nullptr) {
        log("Creating board");
        board_ = Sprite::create("misc/board.png");
        float scaleX {visibleSize_.width / board_->getContentSize().width};
        board_->setScaleX(scaleX);
        board_->setPosition(center_);
        board_->setGlobalZOrder(4);
    }
    return board_;
}

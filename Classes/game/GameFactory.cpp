#include "GameFactory.h"

USING_NS_CC;

GameFactory::GameFactory(Vegolution* game)
: game_ {game}
{
    Director* director {game_->getDirector()};
    visibleSize_ = director->getVisibleSize();
    origin_ = director->getVisibleOrigin();
    center_.x = visibleSize_.width / 2;
    center_.y = visibleSize_.height / 2;
    offsetX_ = visibleSize_.width / 4;
}

Vehicle* GameFactory::createVehicle(std::string vehicleName)
{
    // Create the vehicle
    std::string name = StringUtils::format("vehicle/%s/%s.png", vehicleName.c_str(), vehicleName.c_str());
    Vehicle* vehicle {Vehicle::create(name, 10.0f, 10.0f)};
    vehicle->setPositionX(-offsetX_);
    vehicle->getPhysicsBody()->setGravityEnable(true);

    // Create animation
    Animation* animation {Animation::create()};
    for (int i {0}; i < 8; i++) {
        name = StringUtils::format("vehicle/%s/%s%d.png", vehicleName.c_str(), vehicleName.c_str(), i);
        if (!game_->getFileUtils()->isFileExist(name)) break;
        animation->addSpriteFrameWithFile(name);
    }
    animation->setDelayPerUnit(0.125f / 2.0f);
    animation->setRestoreOriginalFrame(true);
    animation->setLoops(-1);
    Action* action {Animate::create(animation)};
    vehicle->runAction(action);

    return vehicle;
}

MainActor* GameFactory::createActor()
{
    if (actor_ == nullptr) {
        log("Creating MainActor");
        // Create the main actor
        actor_ = MainActor::create();
        actor_->setPosition(center_.x, center_.y);
        actor_->setOffset(offsetX_);

        // Create a standard Vehicle
        log("Creating Vehicle");
        std::string astroName {"astro"};
        Vehicle* vehicle {this->createVehicle(astroName)};
        actor_->getVehicles().push_back(vehicle);
        std::string spiderName {"spider"};
        Vehicle* vehicle2 {this->createVehicle(spiderName)};
        actor_->getVehicles().push_back(vehicle2);
        actor_->setVehicle(vehicle2);
    }
    
    return actor_;
}

Terrain2D* GameFactory::createTerrain()
{
    if (terrain_ == nullptr) {
        terrain_ = Terrain2D::create(this->createActor());    
        terrain_->setPosition(center_.x, origin_.y);
        terrain_->scheduleUpdate();
    }
    return terrain_;
}

Parallax* GameFactory::createParallax()
{
    if (parallax_ == nullptr) {
        parallax_ = Parallax::create("city/", this->createActor());
        parallax_->scheduleUpdate();
    }
    return parallax_;
}

Sprite* GameFactory::createBoard()
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

BodyView* GameFactory::createBodyMenu()
{
    if (body_ == nullptr) {
        log("Creating body menu");
        std::string bodyName {"misc/body.png"};
        body_ = BodyView::create(bodyName, createActor(), visibleSize_.width / 2.0f);
        body_->setPositionY(visibleSize_.height);
        body_->setAnchorPoint(Vec2{0.0f, 1.0f});
        body_->setTouchEnabled(true);
        body_->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
            if (type == ui::Widget::TouchEventType::ENDED) {
                body_->toggle();
            }
            return true;
        });
    }
    return body_;
}

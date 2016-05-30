#include "GameFactory.h"

USING_NS_CC;

GameFactory::GameFactory(Vegolution* game)
	: game_{ game }
	, data_{ game->getDataManager() }
	, bullets_{ data_->getBullets() }
	, vehicles_{ data_->getVehicles() }
	, enemies_{ data_->getEnemies() }
{
    log("Creating GameFactory");
    Director* director {game_->getDirector()};
    visibleSize_ = director->getVisibleSize();
    origin_ = director->getVisibleOrigin();
    center_.x = visibleSize_.width / 2;
    center_.y = visibleSize_.height / 2;
    offsetX_ = visibleSize_.width / 4;

	// Fill enemy pool
	for (Enemy* enemy : enemies_) {
		enemyPool_.push_back(enemy);
	}
}

GameFactory::~GameFactory()
{
    log("Destructing GameFactory");
}

// Get an enemy from the pool
Enemy* GameFactory::spawnEnemy()
{
    if (enemyPool_.empty()) return nullptr;

    Enemy* enemy {enemyPool_.back()};
    enemyPool_.pop_back();
    return enemy;
}

// Return an enemy to the pool
void GameFactory::despawnEnemy(Enemy * enemy)
{
	enemyPool_.push_back(enemy);
}

Vehicle* GameFactory::getVehicle(std::string& vehicleName)
{
    // Get the vehicle
	Vehicle* vehicle{ data_->getVehicle(vehicleName) };
    vehicle->setPositionX(-offsetX_);
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

        // Inject vehicles
		for (Vehicle* vehicle : vehicles_) {
			vehicle->setPositionX(-offsetX_);
			actor_->getVehicles().push_back(vehicle);
		}
        actor_->switchVehicle();
    }
    
    return actor_;
}

Terrain2D* GameFactory::createTerrain()
{
    if (terrain_ == nullptr) {
        terrain_ = Terrain2D::create(this->createActor(), visibleSize_);    
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

ui::ImageView* GameFactory::createLeftGear()
{
    if (leftgear_ == nullptr) {
        log("Creating left gear");
        std::string filename {"misc/leftgear.png"};
		leftgear_ = ui::ImageView::create(filename);
		leftgear_->setPositionY(visibleSize_.height);
		leftgear_->setAnchorPoint(Vec2{ 0.0f, 1.0f });
		leftgear_->setTouchEnabled(true);
		Sprite* vehicle{ Sprite::createWithSpriteFrame(createActor()->getVehicle()->getSpriteFrame()) };
        vehicle->setScale(0.75f, 0.75f);
        vehicle->setPositionY(leftgear_->getContentSize().height);
		vehicle->setAnchorPoint(Vec2{ 0.0f, 1.0f });
		leftgear_->addChild(vehicle);
		leftgear_->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
            if (type == ui::Widget::TouchEventType::ENDED) {
				if (actor_->switchVehicle()) {
					leftgear_->removeAllChildren();
					std::string vehiclename{ actor_->getVehicle()->getName() };
					std::string filename{ StringUtils::format("vehicle/%s/%s.png", vehiclename.c_str(), vehiclename.c_str()) };
					Sprite* vehicle{ Sprite::create(filename) };
                    vehicle->setScale(0.75f, 0.75f);
                    vehicle->setPositionY(leftgear_->getContentSize().height);
					vehicle->setAnchorPoint(Vec2{ 0.0f, 1.0f });
					leftgear_->addChild(vehicle);
				}
            }
            return true;
        });
    }
    return leftgear_;
}

ShotView* GameFactory::createShotMenu()
{
    if (shot_ == nullptr) {
        log("Creatin shot menu");
        std::string shotName{"misc/shot.png"};
        shot_ = ShotView::create(shotName, createActor(), visibleSize_.width / 2.0f);
        shot_->setPositionY(visibleSize_.height / 2);
        shot_->setAnchorPoint(Vec2{0.0f, 0.5f});
        shot_->setTouchEnabled(true);
        shot_->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
            if (type == ui::Widget::TouchEventType::ENDED) {
                shot_->toggle();
            }
            return true;
        });
    }
    return shot_;
}

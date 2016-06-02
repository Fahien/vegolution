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
	Director* director{ game_->getDirector() };
	visibleSize_ = director->getVisibleSize();
    origin_ = director->getVisibleOrigin();
    center_.x = visibleSize_.width / 2;
    center_.y = visibleSize_.height / 2;
    offsetX_ = visibleSize_.width / 4;

	// Fill enemy pool
	for (Enemy* enemy : enemies_) { enemyPool_.push_back(enemy); }
}

GameFactory::~GameFactory()
{
    log("Destructing GameFactory");
	log("Releasing explosion");
	if (explosion_) { explosion_->release(); }
}

// Get an enemy from the pool
Enemy* GameFactory::spawnEnemy()
{
	if (enemyPool_.empty()) { return nullptr; }

	Enemy* enemy{ enemyPool_.front() };
    enemyPool_.erase(enemyPool_.begin());
    return enemy;
}

// Return an enemy to the pool
void GameFactory::despawnEnemy(Enemy* enemy)
{
	enemy->remove();
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
        actor_->setOffsetX(offsetX_);

        // Inject vehicles
		for (Vehicle* vehicle : vehicles_) {
			vehicle->setPositionX(-offsetX_);
			// Generate specific updates SMELL CODE ALERT!
			if (vehicle->getName().compare("astro") == 0) {
				// Astro specific update
				vehicle->specificUpdate_ = [vehicle](float delta) {
					float positionY{ vehicle->getPositionY() };
					float destinationY{ vehicle->getTap().y };
					float velocity{ vehicle->getVelocity() };
					if (positionY < destinationY) {
						vehicle->getPhysicsBody()->applyForce(Vec2{ 0.0f, velocity }); // Increment
					}
					else if (positionY > destinationY) {
						vehicle->getPhysicsBody()->applyForce(Vec2{ 0.0f, -velocity }); // Decrement
					}
				};
			}
			else if (vehicle->getName().compare("spider") == 0) {
				// Spider specific update
				vehicle->onTap_ = [vehicle, this](Vec2 location) {
					if (vehicle->getPositionY() < -visibleSize_.height / 8.0f) {
						vehicle->getPhysicsBody()->applyImpulse(Vec2{ 0.0f, 256.0f });
					}
				};
			}
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

LeftGear* GameFactory::createLeftGear()
{
    if (leftgear_ == nullptr) {
        log("Creating left gear");
		leftgear_ = LeftGear::create();
		// Add this vehicle listener
		actor_->addGear(leftgear_);
		// Update texture
		leftgear_->onVehicleChange(actor_->getVehicle());
		leftgear_->setPositionY(visibleSize_.height);
		leftgear_->setAnchorPoint(Vec2{ 0.0f, 1.0f });
		leftgear_->setTouchEnabled(true);
		leftgear_->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
			if (type == ui::Widget::TouchEventType::ENDED && actor_->canSwitchVehicle()) { actor_->switchVehicle(); }
			else { log("Vehicle is not resting"); }
			return true;
        });
    }
    return leftgear_;
}

RightGear* GameFactory::createRightGear()
{
    if (rightgear_ == nullptr) {
        log("Creating right gear");
        rightgear_ = RightGear::create();
		// Set vehicle health listener
		actor_->setRightGear(rightgear_);
		// Add to vehicle change listener
		actor_->addGear(rightgear_);
		std::string filename{ "misc/rightgear.png" };
		rightgear_->loadTexture(filename);
        rightgear_->setPositionY(visibleSize_.height);
		rightgear_->setPositionX(visibleSize_.width);
        rightgear_->setAnchorPoint(Vec2{ 1.0f, 1.0f });
        rightgear_->setTouchEnabled(true);
        rightgear_->addTouchEventListener([](Ref* sender, ui::Widget::TouchEventType type) {
            if (type == ui::Widget::TouchEventType::ENDED) {
				log("Should open modal");
            }
            return true;
        });

		filename = std::string{ "misc/rightgear-health.png" };
		Sprite* health{ Sprite::create(filename) };
		Size size{ rightgear_->getContentSize() };
		health->setPosition(Vec2{ size.width / (2.0f - 0.0625f), size.height / 1.03125f });
		health->setAnchorPoint(Vec2{ 0.0f, 1.0f });
		rightgear_->setHealthBar(health);
		rightgear_->addChild(health);
    }
    return rightgear_;
}

void GameFactory::createExplosion(Bullet* bullet)
{
	if (explosion_ == nullptr) {
		// Create the animation
		Animation* animation{ Animation::create() };
		for (int i{ 0 }; i < 8; i++) {
			std::string filename = StringUtils::format("misc/explosion/explosion%d.png", i);
			if (!game_->getFileUtils()->isFileExist(filename)) break;
			animation->addSpriteFrameWithFile(filename);
		}
		animation->setDelayPerUnit(0.0625f);
		animation->setRestoreOriginalFrame(true);
		Animate* animate{ Animate::create(animation) };
		// Create the explosion
		explosion_ = Explosion::create(animate);
	}
	// If explosion has no parent
	if (explosion_->getParent() == nullptr) {
		bullet->getParent()->addChild(explosion_);
		explosion_->setPosition(bullet->getPosition());
		explosion_->animate();
	}
}
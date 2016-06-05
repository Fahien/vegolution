#include "GameFactory.h"

USING_NS_CC;

GameFactory::GameFactory(DataManager* data)
        : visibleSize_{ Director::getInstance()->getVisibleSize() }
        , origin_{ Director::getInstance()->getVisibleOrigin() }
        , center_{ visibleSize_.width / 2.0f + origin_.x, visibleSize_.height / 2.0f + origin_.y }
        , offsetX_{ visibleSize_.width / 4.0f }
        , bullets_{}
        , vehicles_{}
        , actor_{ nullptr }
        , terrain_{ nullptr }
        , parallax_{ nullptr }
        , board_{ nullptr }
        , explosion_{ nullptr }
{
    log("Creating GameFactory");
    data->loadBullets(bullets_);
    data->loadVehicles(vehicles_, bullets_);
}

GameFactory::~GameFactory()
{
    log("Destructing GameFactory");
	log("Releasing explosion");
	if (explosion_) { explosion_->release(); }
    log("Releasing bullets");
    for (Bullet* bullet: bullets_) bullet->release();
    log("Releasing vehicles");
    for (Vehicle* vehicle : vehicles_) vehicle->release();
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

void GameFactory::createExplosion(Sprite* node)
{
	if (explosion_ == nullptr) {
		// Create the animation
		Animation* animation{ Animation::create() };
		for (int i{ 0 }; i < 8; i++) {
			std::string filename = StringUtils::format("misc/explosion/explosion%d.png", i);
			if (!FileUtils::getInstance()->isFileExist(filename)) break;
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
		node->getParent()->addChild(explosion_);
		explosion_->setPosition(node->getPosition());
		explosion_->animate();
	}
}
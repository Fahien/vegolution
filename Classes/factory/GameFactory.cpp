#include "GameFactory.h"

USING_NS_CC;

GameFactory::GameFactory(Vegolution* game)
	: game_{ game }
	, data_{ game->getDataManager() }
	, bullets_{ data_->getBullets() }
	, vehicles_{ data_->getVehicles() }
{
    log("Creating GameFactory");
	Director* director{ game_->getDirector() };
	visibleSize_ = director->getVisibleSize();
    origin_ = director->getVisibleOrigin();
    center_.x = visibleSize_.width / 2;
    center_.y = visibleSize_.height / 2;
    offsetX_ = visibleSize_.width / 4;
}

GameFactory::~GameFactory()
{
    log("Destructing GameFactory");
	log("Releasing explosion");
	if (explosion_) { explosion_->release(); }
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
		node->getParent()->addChild(explosion_);
		explosion_->setPosition(node->getPosition());
		explosion_->animate();
	}
}
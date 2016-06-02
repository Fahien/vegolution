#include "GameLayer.h"

USING_NS_CC;

GameLayer::GameLayer(Vegolution* game)
: game_{ game }
, factory_{ game }
, offsetX_{ game->getDirector()->getVisibleSize().width / 4 }
{}

GameLayer* GameLayer::create(Vegolution* game)
{
    // Construct
	GameLayer *layer{ new (std::nothrow) GameLayer{ game } };

    // Initialize
    if (layer && layer->init()) {
        layer->autorelease();
        return layer;
    }

    // Error
    CC_SAFE_DELETE(layer);
    return nullptr;
}

Scene* GameLayer::createScene(Vegolution* game)
{
    // Scene is an autoreleased object
	Scene* scene{ Scene::createWithPhysics() };
	scene->getPhysicsWorld()->setAutoStep(false);
    // scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    // Layer is an autoreleased object
	GameLayer* layer{ GameLayer::create(game) };
    layer->scene_ = scene;
    // Add the layers as a child to the scene
    scene->addChild(layer);
    scene->addChild(layer->menuLayer_);
    // Return the scene
    return scene;
}

bool GameLayer::init()
{
    // Super init first
    if (!Layer::init()) return false;
    scheduleUpdateWithPriority(-8);
    setTag(0);
    
    // Get the main actor
    actor_ = factory_.createActor();
    this->addChild(actor_, -1);

    // Get the terrain
	Terrain2D* terrain{ factory_.createTerrain() };
    this->addChild(terrain);

    // Get the parallax
	Parallax* parallax{ factory_.createParallax() };
    this->addChild(parallax, -2);

	log("Creating menu layer");
    menuLayer_ = Layer::create();
    menuLayer_->setAnchorPoint(Vec2{ 0.5f, 0.5f });
    menuLayer_->setTag(1);

    // Get the left gear
    log("Getting left gear");
	LeftGear* leftgear{ factory_.createLeftGear() };
    menuLayer_->addChild(leftgear, -1);

	// Get the right gear
	log("Getting right gear");
	ui::ImageView* rightgear{ factory_.createRightGear() };
	menuLayer_->addChild(rightgear, -2);

    // Get the board
    board_ = factory_.createBoard();
    menuLayer_->addChild(board_, 1);

    // Create enemy spawning action
	scheduleSpawning();

	// Add contact event listener with group PlayerBullet - Enemy
	listenPlayerBullet();
	listenEnemyBullet();

    // Create the Game Controller
	float centerX{ getContentSize().width / 2.0f };
	GameController* controller{ GameController::create(actor_, centerX) };
    _eventDispatcher->addEventListenerWithFixedPriority(controller, 1);

    return true;
}

// Update method
void GameLayer::update(float delta)
{
	// Step physics
	scene_->getPhysicsWorld()->step(MAX(MIN(0.0625f, delta), 0.015625f));
    // Get the actor X
    centerX_ = actor_->getPositionX() + actor_->getVehicle()->getPositionX() + actor_->getOffsetX();
    // Update the camera X
    scene_->getDefaultCamera()->setPositionX(centerX_);
    // Update the board X
    menuLayer_->setPositionX(centerX_ - actor_->getOffsetX() * 2);
}

void GameLayer::scheduleSpawning()
{
	CallFunc* spawn{ CallFunc::create([this]() {
		Enemy* enemy{ factory_.spawnEnemy() };
		if (enemy == nullptr) return;
		log("Spawning enemy %s", enemy->getName().c_str());
		enemy->setPositionX(centerX_ + actor_->getOffsetX() * 3);
		enemy->setPositionY(actor_->getPositionY());
		addChild(enemy, 1);
	}) };
	DelayTime* delay{ DelayTime::create(3.0f) };
	Sequence* sequence{ Sequence::createWithTwoActions(delay, spawn) };
	RepeatForever* repeat{ RepeatForever::create(sequence) };
	runAction(repeat);
}

void GameLayer::listenPlayerBullet()
{
	// Create a contact listener
	EventListenerPhysicsContactWithGroup* contactListener{ EventListenerPhysicsContactWithGroup::create(2) };
	// Create a callback
	contactListener->onContactBegin = [this](PhysicsContact& contact) {
		log("Enemy hit");
		Enemy* enemy{ nullptr };
		Bullet* bullet{ nullptr };
		Node* nodeA{ contact.getShapeA()->getBody()->getNode() };
		Node* nodeB{ contact.getShapeB()->getBody()->getNode() };
		if (nodeA->getTag() == 8) {
			bullet = static_cast<Bullet*>(nodeA);
			enemy = static_cast<Enemy*>(nodeB);
		}
		else {
			bullet = static_cast<Bullet*>(nodeB);
			enemy = static_cast<Enemy*>(nodeA);
		}
		TintTo* toRed{ TintTo::create(0.125f, Color3B::RED) };
		TintTo* toWhite{ TintTo::create(0.125f, Color3B::WHITE) };
		enemy->runAction(Sequence::createWithTwoActions(toRed, toWhite));
		factory_.createExplosion(bullet);
		enemy->setHealth(enemy->getHealth() - bullet->getDamage());
		if (enemy->getHealth() <= 0) {
			DelayTime* time{ DelayTime::create(0.125f) };
			CallFunc* remove{ CallFunc::create([=]() {
				enemy->removeFromParent();
				factory_.despawnEnemy(enemy);
			}) };
			TintTo* white{ TintTo::create(0.0f, Color3B::WHITE) };
			enemy->runAction(Sequence::create(time, white, remove, nullptr));
		}
		bullet->removeFromParent();
		return true;
	};
	_eventDispatcher->addEventListenerWithFixedPriority(contactListener, 2);
}

void GameLayer::listenEnemyBullet()
{
	EventListenerPhysicsContactWithGroup* contactListener{ EventListenerPhysicsContactWithGroup::create(1) };
	contactListener->onContactBegin = [this](PhysicsContact& contact) {
		log("Actor hit");
		Bullet* bullet{ nullptr };
		Node* nodeA{ contact.getShapeA()->getBody()->getNode() };
		Node* nodeB{ contact.getShapeB()->getBody()->getNode() };
		if (nodeA->getTag() == 8) {
			bullet = static_cast<Bullet*>(nodeA);
		}
		else {
			bullet = static_cast<Bullet*>(nodeB);
		}
		TintTo* toRed{ TintTo::create(0.125f, Color3B::RED) };
		TintTo* toWhite{ TintTo::create(0.125f, Color3B::WHITE) };
		actor_->getVehicle()->runAction(Sequence::createWithTwoActions(toRed, toWhite));
		factory_.createExplosion(bullet);
		actor_->setHealth(actor_->getHealth() - bullet->getDamage());
		if (actor_->getHealth() <= 0) {
			DelayTime* time{ DelayTime::create(0.125f) };
			CallFunc* remove{ CallFunc::create([=]() {
				Vehicle* vehicle {actor_->getVehicle()};
				if (actor_->getHealth() <= 0) {
					if (actor_->getVehicles().empty()) {
						log("Should game over");
						return;
					}
					actor_->switchVehicle();
					actor_->getVehicles().pop_back();
				}
			}) };
			TintTo* white{ TintTo::create(0.0f, Color3B::WHITE) };
			actor_->getVehicle()->runAction(Sequence::create(time, white, remove, nullptr));
		}
		bullet->removeFromParent();
		return true;
	};
	_eventDispatcher->addEventListenerWithFixedPriority(contactListener, 1);
}
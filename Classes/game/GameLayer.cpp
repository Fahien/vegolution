#include "GameLayer.h"
#include "GameController.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

GameLayer::GameLayer(Vegolution* game)
: game_    {game}
, factory_ {game}
, offsetX_ {game->getDirector()->getVisibleSize().width / 4}
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
    // Add the layer as a child to the scene
    scene->addChild(layer);
    scene->addChild(layer->menuLayer_);
    // Return the scene
    return scene;
}

bool GameLayer::init()
{
    // Super init first
    if (!Layer::init()) return false;
    this->scheduleUpdateWithPriority(-8);

    this->setTag(0);
    
    // Get the main actor
    actor_ = factory_.createActor();
    this->addChild(actor_, 2);

    // Get the terrain
	Terrain2D* terrain{ factory_.createTerrain() };
    this->addChild(terrain, 2);

    // Get the parallax
	Parallax* parallax{ factory_.createParallax() };
    this->addChild(parallax, 1);

	log("Creating menu layer");
    menuLayer_ = Layer::create();
    menuLayer_->setAnchorPoint(Vec2{0.5f, 0.5f});
    menuLayer_->setTag(1);

    // Get the left gear
    log("Getting left gear");
	ui::ImageView* leftgear{ factory_.createLeftGear() };
    menuLayer_->addChild(leftgear, 6);

    // Get the board
    board_ = factory_.createBoard();
    menuLayer_->addChild(board_, 8);
    
    // Create enemy spawning action
    CallFunc* spawn{ CallFunc::create([this](){
        log("Spawning enemy");
        Enemy* enemy {factory_.spawnEnemy()};
        if (enemy == nullptr) return;
        enemy->setPositionX(centerX_ + actor_->getOffsetX() * 3);
        enemy->setPositionY(actor_->getPositionY());
        this->addChild(enemy, 3);
	}) };
	DelayTime* delay{ DelayTime::create(3.0f) };
	Sequence* sequence{ Sequence::createWithTwoActions(delay, spawn) };
	RepeatForever* repeat{ RepeatForever::create(sequence) };
    this->runAction(repeat);

	// Add contact event listener with group PlayerBullet - Enemy
	EventListenerPhysicsContact* contactListener{ EventListenerPhysicsContact::create() };
	contactListener->onContactBegin = [this](PhysicsContact& contact) {
		log("Contact callback");
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
		enemy->setHealth(enemy->getHealth() - bullet->getDamage());
		if (enemy->getHealth() <= 0) {
			enemy->removeFromParent();
			factory_.despawnEnemy(enemy);
		}
		bullet->removeFromParent();
		return true;
	};
	_eventDispatcher->addEventListenerWithFixedPriority(contactListener, 1);

    // Create the Game Controller
	GameController* controller{ GameController::create(actor_, parallax) };
    _eventDispatcher->addEventListenerWithFixedPriority(controller, 1);

    return true;
}

void GameLayer::update(float delta)
{
    // Update the physics engine
    scene_->getPhysicsWorld()->step(delta);
    // Get the actor X
    centerX_ = actor_->getPositionX() + actor_->getVehicle()->getPositionX() + actor_->getOffsetX();
    // Update the camera X
    scene_->getDefaultCamera()->setPositionX(centerX_);
    // Update the board X
    menuLayer_->setPositionX(centerX_ - actor_->getOffsetX() * 2);
}

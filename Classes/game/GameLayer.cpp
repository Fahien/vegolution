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
    GameLayer *ret {new (std::nothrow) GameLayer{game}};
    // Initialize
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    // Error
    CC_SAFE_DELETE(ret);
    return nullptr;
}

Scene* GameLayer::createScene(Vegolution* game)
{
    // Scene is an autoreleased object
    Scene* scene {Scene::createWithPhysics()};
    scene->getPhysicsWorld()->setAutoStep(false);
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    // Layer is an autoreleased object
    GameLayer* layer {GameLayer::create(game)};
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
    Terrain2D* terrain {factory_.createTerrain()};
    this->addChild(terrain, 2);
    // Get the parallax
    Parallax* parallax {factory_.createParallax()}; 
    this->addChild(parallax, 1);

    menuLayer_  = Layer::create();
    menuLayer_->setAnchorPoint(Vec2{0.5f, 0.5f});
    menuLayer_->setTag(1);
    log("Creating menu layer");
    // Get the body menu
    log("Getting body");
    BodyView* body {factory_.createBodyMenu()};
    menuLayer_->addChild(body, 6);
    // Get the shot menu
    log("Getting shot");
    ShotView* shot {factory_.createShotMenu()};
    menuLayer_->addChild(shot, 7);
    // Get the board
    board_ = factory_.createBoard();
    menuLayer_->addChild(board_, 8);
    
    // Create enemy spawning action
    CallFunc* spawn {CallFunc::create([this](){
        log("Spawning enemy");
        Enemy* enemy {factory_.createEnemy()};
        if (enemy == nullptr) return;
        enemy->setPositionX(centerX_ + actor_->getOffsetX() * 3);
        enemy->setPositionY(actor_->getPositionY());
        this->addChild(enemy, 3);
    })};
    DelayTime* delay {DelayTime::create(3.0f)};
    Sequence* sequence {Sequence::createWithTwoActions(delay, spawn)};
    RepeatForever* repeat {RepeatForever::create(sequence)};
    this->runAction(repeat);

    // Create the Game Controller
    GameController* controller {GameController::create(actor_, parallax)};
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

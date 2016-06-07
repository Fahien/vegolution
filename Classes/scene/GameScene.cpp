#include <audio/include/SimpleAudioEngine.h>
#include "GameScene.h"
#include "scene/MainScene.h"

USING_NS_CC;

GameScene::GameScene(DataManager& data, TextFactory& textFactory)
        : factory_{ data }
        , hudFactory_{ data, textFactory }
        , enemyFactory_{ data, factory_.getBullets() }
        , offsetX_{ Director::getInstance()->getVisibleSize().width / 4.0f }
        , pause_{ false }
{
    log("Constructing GameScene");
}

GameScene::~GameScene()
{
    log("Destructing GameScene");
	_eventDispatcher->removeEventListener(controller_);
}

GameScene* GameScene::create(DataManager& data, TextFactory& textFactory)
{
    GameScene* scene{ new(std::nothrow) GameScene{ data, textFactory }}; // Construct
    if (scene && scene->init(data, textFactory)) { scene->autorelease(); }  // Initialize
    else { CC_SAFE_DELETE(scene); }                        // Error
    return scene;
}

bool GameScene::init(DataManager& data, TextFactory& textFactory)
{
    // Super init first
    if (!Scene::initWithPhysics()) return false;

    _physicsWorld->setAutoStep(false);
    scheduleUpdateWithPriority(-8);
    setTag(5);

    // Create a layer
    Layer* layer{ Layer::create() };
    addChild(layer);

    // Get the main actor
    actor_ = factory_.createActor();
    layer->addChild(actor_, -1);

    // Get the terrain
    Terrain2D* terrain{ factory_.createTerrain() };
    layer->addChild(terrain);

    // Get the parallax
    Parallax* parallax{ factory_.createParallax() };
    layer->addChild(parallax, -2);

    log("Creating menu layer");
    menuLayer_ = Layer::create();
    addChild(menuLayer_);
    menuLayer_->setAnchorPoint(Vec2{ 0.5f, 0.5f });
    menuLayer_->setTag(1);

    // Get the score
    scoreText_ = hudFactory_.getScoreText();
    menuLayer_->addChild(scoreText_, -3);

    // Get the left gear
    ui::ImageView* leftGear{ hudFactory_.getLeftGear(actor_, textFactory.getVisibleSize()) };
    menuLayer_->addChild(leftGear, -1);

    // Get the right gear
    ui::ImageView* rightGear{ hudFactory_.getRightGear(actor_, textFactory.getVisibleSize()) };
    rightGear->addTouchEventListener(
            [ this ](Ref* sender, ui::Widget::TouchEventType type) {
                if (type == ui::Widget::TouchEventType::ENDED) {
                    pause_ = !pause_;
                    if (pause_) {
                        _director->pause();
                        CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
                        menuLayer_->addChild(hudFactory_.getQuitText(), -4);
                    }
                    else {
                        _director->resume();
                        CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
                        menuLayer_->removeChildByName("Quit", false);
                    }
                }
                return true;
            }
    );
    menuLayer_->addChild(rightGear, -2);

    // Get the board
    board_ = factory_.createBoard();
    menuLayer_->addChild(board_, 1);

    // Create enemy spawning action
    scheduleSpawning(actor_, enemyFactory_, factory_);

    // Add contact event listener with group PlayerBullet - Enemy
    listenPlayerBullet();
    // Add contact event listener with group EnemyBullet - Player
    listenEnemyBullet(data, textFactory);

    // Create the Game Controller
    controller_ = GameController::create(actor_, getContentSize());
    _eventDispatcher->addEventListenerWithFixedPriority(controller_, 1);

    return true;
}

void GameScene::willEnterForeground()
{
    pause_ = false;
    _director->resume();
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
    menuLayer_->removeChildByName("Quit", false);
}

// Update method
void GameScene::update(float delta)
{
    //if (!pause_) {
    // Step physics
    _physicsWorld->step(MAX(MIN(deltaMin, delta), deltaMax));
    //}
    // Get the right center X
    centerX_ = actor_->getPositionX() + actor_->getVehicle()->getPositionX() + actor_->getOffsetX();
    // Update the camera X
    _defaultCamera->setPositionX(centerX_);
    // Update the board X
    menuLayer_->setPositionX(centerX_ - actor_->getOffsetX() * 2.0f);
}

void GameScene::scheduleSpawning(MainActor* actor, EnemyFactory& enemyFactory, GameFactory& gameFactory)
{
    CallFunc* spawn{ CallFunc::create([ this, actor, &enemyFactory, &gameFactory ]() {
        Vec2 position{ actor->getVehicle()->getPositionX() + actor->getOffsetX() * 8.0f, 0.0f };
        Enemy* enemy{ enemyFactory.spawn(position) };
        if (enemy == nullptr) return;
        this->addChild(enemy, 1);
        enemy->resume();
        DelayTime* time{ DelayTime::create(32.0f) };
        CallFunc* despawn{ CallFunc::create([ enemy, &gameFactory, &enemyFactory ]() {
            gameFactory.createExplosion(enemy);
            enemyFactory.despawn(enemy);
        }) };
        Sequence* sequence{ Sequence::createWithTwoActions(time, despawn) };
        enemy->runAction(sequence);
    }) };
    DelayTime* delay{ DelayTime::create(3.0f) };
    Sequence* sequence{ Sequence::createWithTwoActions(delay, spawn) };
    RepeatForever* repeat{ RepeatForever::create(sequence) };
    runAction(repeat);
}

void GameScene::listenPlayerBullet()
{
    // Create a contact listener
    EventListenerPhysicsContactWithGroup* contactListener{ EventListenerPhysicsContactWithGroup::create(2) };
    // Create a callback
    contactListener->onContactBegin = [ this ](PhysicsContact& contact) {
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
        enemy->damage(bullet->getDamage());
        // If enemy is dead
        if (enemy->isDead()) {
            DelayTime* time{ DelayTime::create(0.125f) };
            CallFunc* remove{ CallFunc::create([ this, enemy ]() { enemyFactory_.despawn(enemy); }) };
            TintTo* white{ TintTo::create(0.0f, Color3B::WHITE) };
            enemy->runAction(Sequence::create(time, white, remove, nullptr));
            // Increase score
            std::string text{ StringUtils::format("%d", ++score_) };
            scoreText_->setString(text);
        }
        bullet->remove();
        return true;
    };
    _eventDispatcher->addEventListenerWithFixedPriority(contactListener, 2);
}

void GameScene::listenEnemyBullet(DataManager& data, TextFactory& textFactory)
{
    EventListenerPhysicsContactWithGroup* contactListener{ EventListenerPhysicsContactWithGroup::create(1) };
    contactListener->onContactBegin = [ this, &data, &textFactory ](PhysicsContact& contact) {
        Bullet* bullet{ nullptr };
        Node* nodeA{ contact.getShapeA()->getBody()->getNode() };
        Node* nodeB{ contact.getShapeB()->getBody()->getNode() };
        if (nodeA->getTag() == 8) { bullet = static_cast<Bullet*>(nodeA); }
        else { bullet = static_cast<Bullet*>(nodeB); }
        TintTo* toRed{ TintTo::create(0.125f, Color3B::RED) };
        TintTo* toWhite{ TintTo::create(0.125f, Color3B::WHITE) };
        actor_->getVehicle()->runAction(Sequence::createWithTwoActions(toRed, toWhite));
        factory_.createExplosion(bullet);
        actor_->setHealth(actor_->getHealth() - bullet->getDamage());
        if (actor_->getHealth() <= 0) {
            DelayTime* time{ DelayTime::create(0.125f) };
            CallFunc* remove{ CallFunc::create([ this, &data, &textFactory ]() {
                if (actor_->getHealth() <= 0) {
                    if (actor_->getVehicles().empty()) {
                        actor_->die(); // Die
                        _eventDispatcher->removeAllEventListeners(); // Remove controller controller
                        DelayTime* delay{ DelayTime::create(2.0f) }; // Wait a delay
                        CallFunc* func{ CallFunc::create([ this, &data, &textFactory ]() {
                            Scene* main{ MainScene::create(data, textFactory) };
                            _director->replaceScene(TransitionFade::create(0.5f, main, Color3B::BLACK));
                        }) }; // Return to main
                        Sequence* sequence{ Sequence::createWithTwoActions(delay, func) };
                        runAction(sequence);
                        return;
                    }
                    actor_->switchVehicle();
                    // Remove broken vehicle
                    actor_->getVehicles().pop_back();
                }
            }) };
            TintTo* white{ TintTo::create(0.0f, Color3B::WHITE) };
            actor_->getVehicle()->runAction(Sequence::create(time, white, remove, nullptr));
        }
        bullet->remove();
        return true;
    };
    _eventDispatcher->addEventListenerWithFixedPriority(contactListener, 1);
}

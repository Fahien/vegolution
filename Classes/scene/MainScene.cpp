#include "MainScene.h"
#include "factory/MainFactory.h"

USING_NS_CC;

MainScene* MainScene::create(Vegolution *game) {
    MainScene *scene{new(std::nothrow) MainScene{}}; // Construct
    if (scene && scene->init(game)) { scene->autorelease(); } // Initialize
    else { CC_SAFE_DELETE(scene); }
    return scene;
}

bool MainScene::init(Vegolution *game) {
    // Super init first
    if (!Scene::init()) return false;
    log("Initializing MainScene");

    // Play the background music
    game->getAudio().playMusic(soundtrack);

    // Create the MainFactory
    MainFactory factory {game};

    // Get the background
    this->addChild(factory.createBackground());

    // Get the menu
    this->addChild(factory.createMenu());

    // Get the board
    this->addChild(factory.createBoard());

    return true;
}
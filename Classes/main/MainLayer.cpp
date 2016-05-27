#include "MainLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

MainLayer::MainLayer(Vegolution* game)
: game_ {game}
, factory_ {game}
{
    log("Creating MainLayer");
}

MainLayer::~MainLayer()
{
    log("Destructing MainLayer");
}

MainLayer* MainLayer::create(Vegolution* game)
{
    MainLayer *ret {new (std::nothrow) MainLayer{game}};
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

Scene* MainLayer::createScene(Vegolution* game)
{
    // Scene is an autoreleased object
    Scene* scene {Scene::create()}; 
    // Layer is an autoreleased object
    MainLayer* layer {MainLayer::create(game)};
    // Add the layer as a child to the scene
    scene->addChild(layer);
    // Return the scene
    return scene;
}

bool MainLayer::init()
{
    // Super init first
    if (!Layer::init()) return false;

    // Get the background
    Sprite* background {factory_.createBackground()};
    this->addChild(background);
   
    // Get the menu
    ui::Layout* menu {factory_.createMenu()};
    this->addChild(menu);
    
    // Get the play text
    ui::Text* play {factory_.createPlayText()};
    menu->addChild(play);
   
    // Get the settings text
    ui::Text* settings{factory_.createSettingsText()};
    menu->addChild(settings);
    
    // Get the quit text
    ui::Text* quit {factory_.createQuitText()};
    menu->addChild(quit);
   
    // Get the board
    Sprite* board {factory_.createBoard()};
    this->addChild(board);

    return true;
}


void MainLayer::menuCloseCallback(Ref* pSender)
{
    game_->getDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

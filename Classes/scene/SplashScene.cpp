#include "SplashScene.h"

USING_NS_CC;

SplashScene* SplashScene::create(DataManager& data, TextFactory& textFactory)
{
    SplashScene* screen{ new(std::nothrow) SplashScene{ }}; // Construct
    if (screen && screen->init(data, textFactory)) { screen->autorelease(); } // Initialize
    else { CC_SAFE_DELETE(screen); }
    return screen;
}

bool SplashScene::init(DataManager& data, TextFactory& textFactory)
{
    // Super init first
    if (!Scene::init()) return false;

    // Create a layer
    Layer* layer{ Layer::create() };
    // Add the layer to the scene
    addChild(layer);

    // Initialize TextFactory
    textFactory.init(data);

    // Run Splash Sequence
    layer->runAction(factory_.createSequence(data, textFactory));

    return true;
}
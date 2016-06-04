#include "SettingsScene.h"
#include "factory/SettingsFactory.h"

USING_NS_CC;

SettingsScene *SettingsScene::create(Vegolution *game) {
    SettingsScene *scene{new(std::nothrow) SettingsScene{}}; // Construct
    if (scene && scene->init(game)) { scene->autorelease(); } // Initialize
    else { CC_SAFE_DELETE(scene); }
    return scene;
}

bool SettingsScene::init(Vegolution *game) {
    // Super init first
    if (!Scene::init()) return false;

    // Create a layer
    Layer* layer {Layer::create()};

    // Create the factory
    SettingsFactory factory {game->getDataManager()};
    // Get the settings background
    layer->addChild(factory.createBackground());
    // Get the menu from the factory
    layer->addChild(factory.createMenu());

    // Add the layer
    addChild(layer);

    return true;
}
#include "SettingsScene.h"
#include "factory/SettingsFactory.h"

USING_NS_CC;

SettingsScene::SettingsScene() {
    log("Constructing SettingsScene");
}

SettingsScene::~SettingsScene() {
    log("Destructing SettingsScene");
}

SettingsScene* SettingsScene::create(DataManager& data, TextFactory& textFactory) {
    SettingsScene* scene{ new(std::nothrow) SettingsScene{} }; // Construct
    if (scene && scene->init(data, textFactory)) { scene->autorelease(); } // Initialize
    else { CC_SAFE_DELETE(scene); }
    return scene;
}

bool SettingsScene::init(DataManager& data, TextFactory& textFactory) {
    // Super init first
    if (!Scene::init()) return false;
    log("Initializing SettingsScene");

    // Create a layer
    Layer* layer{ Layer::create() };

    // Create the factory
    SettingsFactory factory{ data, textFactory };
    // Get the settings background
    layer->addChild(factory.getBackground());
    // Get the menu from the factory
    layer->addChild(factory.getMenu());
    // Get the board
    layer->addChild(factory.getBoard());

    // Add the layer
    addChild(layer);

    return true;
}
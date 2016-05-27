#include "SettingsLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* SettingsLayer::createScene()
{
    // Scene is an autoreleased object
    Scene* scene {Scene::create()}; 
    // Layer is an autoreleased object
    SettingsLayer* layer {SettingsLayer::create()};
    // Add the layer as a child to the scene
    scene->addChild(layer);
    // Return the scene
    return scene;
}

bool SettingsLayer::init()
{
    // Super init first
    if (!Layer::init()) return false;

    return true;
}

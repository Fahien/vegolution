#include "MainScene.h"
#include "SimpleAudioEngine.h"
#include "factory/MainFactory.h"

USING_NS_CC;

MainScene::MainScene() {
    log("Constructing MainScene");
}

MainScene::~MainScene() {
    log("Destructing MainScene");
}

MainScene* MainScene::create(DataManager* data) {
    MainScene* scene{ new(std::nothrow) MainScene{} }; // Construct
    if (scene && scene->init(data)) { scene->autorelease(); } // Initialize
    else { CC_SAFE_DELETE(scene); }
    return scene;
}

bool MainScene::init(DataManager* data) {
    // Super init first
    if (!Scene::init()) return false;
    log("Initializing MainScene");

    // If audio is on
    if (data->getInteger("audio.status") == 1) {
        // Get audio instance
        CocosDenshion::SimpleAudioEngine *audio{CocosDenshion::SimpleAudioEngine::getInstance()};
        // If the music is not playing, play it and loop
        if (!audio->isBackgroundMusicPlaying()) {
            audio->preloadBackgroundMusic("audio/soundtrack.mp3");
            audio->playBackgroundMusic("audio/soundtrack.mp3", true);
        }
    }

    // Create a Layer
    Layer* layer { Layer::create() };
    // Create the MainFactory
    MainFactory factory{ data };
    // Get the background
    layer->addChild(factory.getBackground());
    // Get the menu
    layer->addChild(factory.getMenu());
    // Get the board
    layer->addChild(factory.getBoard());

    addChild(layer);
    return true;
}
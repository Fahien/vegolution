#include "SplashScene.h"

USING_NS_CC;

SplashScene *SplashScene::create(Vegolution *game) {
    SplashScene *screen{new(std::nothrow) SplashScene{}}; // Construct
    if (screen && screen->init(game)) { screen->autorelease(); } // Initialize
    else { CC_SAFE_DELETE(screen); }
    return screen;
}

bool SplashScene::init(Vegolution *game) {
    // Super init first
    if (!Scene::init()) return false;

    // Create a layer
    Layer *layer{Layer::create()};

    Size size{game->getDirector()->getVisibleSize()};
    Vec2 center{size.width / 2.0f, size.height / 2.0f};

    // Load the glow
    std::string glowName{"misc/rfp-glow.png"};
    Sprite *glow{Sprite::create(glowName)};
    glow->setPosition(center);
    FadeOut *fadeOut{FadeOut::create(1.0f)};
    FadeIn *fadeIn{FadeIn::create(1.0f)};
    Sequence *sequence{Sequence::createWithTwoActions(fadeOut, fadeIn)};
    RepeatForever *glowForever{RepeatForever::create(sequence)};
    glow->runAction(glowForever);
    layer->addChild(glow);

    // Load RFP
    std::string rfpName{"misc/rfp.png"};
    Sprite *rfp{Sprite::create(rfpName)};
    rfp->setPosition(center);
    layer->addChild(rfp);

    // Add the layer to the scene
    addChild(layer);

    // Scene logic
    CallFunc *initData{CallFunc::create([game]() { game->initDataManager(); })};
    DelayTime *delay{DelayTime::create(2.0f)};
    CallFunc *changeScene{CallFunc::create([game]() {
        Scene *main{MainLayer::createScene(game)};
        TransitionFade *transition{TransitionFade::create(0.5f, main, Color3B::BLACK)};
        game->getDirector()->replaceScene(transition);
    })};
    Sequence *sceneSequence{Sequence::create(initData, delay, changeScene, nullptr)};
    runAction(sceneSequence);

    return true;
}
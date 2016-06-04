#include "Vegolution.h"
#include "scene/SplashScene.h"

USING_NS_CC;

static Size designSize{570, 320};
static Size windowSize{1024, 544};

// Information about resources
typedef struct tagResource {
    // The size that this resource is designed for
    Size size;
    // If the screen size is more than this value, this resource is a valid choice
    Size useIfScreenOverSize;
    // The name of the directory containing resources of this type
    char directory[3];
} Resource;

// Define all our resource types and locations
static Resource largeResource{Size{1920, 1080}, Size{1024, 768}, "hd"};
static Resource mediumResource{Size{1024, 768}, Size{750, 544}, "md"};
static Resource smallResource{Size{480, 320}, Size{0, 0}, "sd"};

// Declare and array containing the resource descriptions, from largest to smallest
static std::array<Resource, 3> resources{{largeResource, mediumResource, smallResource}};

Vegolution::Vegolution()
        : director_{nullptr}, fileUtils_{nullptr}, audioEngine_{nullptr}, dataManager_{}, audio_{} {
    log("Creating Vegolution");
}

Vegolution::~Vegolution() {
    log("Destructing Vegolution");
    audio_.unload();
}

// If you want a different context, just modify the value of glContextAttrs
// it will takes effect on all platforms
void Vegolution::initGLContextAttrs() {
    // Set OpenGL context attributions, now can only set six attributions:
    // red, green, blue, alpha, depth, stencil
    GLContextAttrs glContextAttrs{8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

void Vegolution::initDataManager() {
    // Initialize the DataManager
    dataManager_.init(fileUtils_);
}

void Vegolution::initAudioFactory() {
    // Initialize the AudioFactory
    audio_.load(audioEngine_);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages() {
    return 0; //flag for packages manager
}

// Hook method for application did finish launching
bool Vegolution::applicationDidFinishLaunching() {
    // Get Director instance
    director_ = Director::getInstance();
    // Get FileUtils instance
    fileUtils_ = FileUtils::getInstance();
    // Get the SimpleAudioEngine instance
    audioEngine_ = CocosDenshion::SimpleAudioEngine::getInstance();

    // Initialize director
    GLView *glView{director_->getOpenGLView()};
    if (!glView) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glView = GLViewImpl::createWithRect("Vegolution", Rect{0, 0, windowSize.width, windowSize.height});
#else
        glView = GLViewImpl::create("Vegolution");
#endif
        director_->setOpenGLView(glView);
    }

    // Set FPS. the default value is 1.0/60 if you don't call this
    director_->setAnimationInterval(1.0f / 60.0f);

    // Set the design resolution
    glView->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::FIXED_HEIGHT);

    // Get the frame size
    Size frameSize{glView->getFrameSize()};
    log("Frame size is %.0fx%.0f", frameSize.width, frameSize.height);

    // Vector to build a list of resources paths
    std::vector<std::string> searchPaths;

    float scaleFactor{-1.0f};
    // Look through our resource definitions
    for (auto resource : resources) {
        // If the screen is wider or higher than the resolution of the resource
        if (frameSize.width > resource.useIfScreenOverSize.width) {
            // Add this directory to the search path
            searchPaths.push_back(resource.directory);
            log("Searching in %s", resource.directory);
            // If we haven't already determined the scale factor
            if (scaleFactor == -1) {
                scaleFactor = resource.size.height / designSize.height;
            }
            break;
        }
    }

    // Set scale factor and search paths
    director_->setContentScaleFactor(scaleFactor);
    fileUtils_->setSearchPaths(searchPaths);

    register_all_packages();

    // Create the main scene. it's an autorelease object
    Scene *scene{SplashScene::create(this)};

    // Run now!
    director_->runWithScene(scene);
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void Vegolution::applicationDidEnterBackground() {
    director_->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    audioEngine_->pauseBackgroundMusic();
}

// This function will be called when the app is active again
void Vegolution::applicationWillEnterForeground() {
    ;
    director_->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    audioEngine_->resumeBackgroundMusic();
}

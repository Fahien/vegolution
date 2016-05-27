#include "Vegolution.h"
#include "main/MainLayer.h"
#include <array>

USING_NS_CC;

//static Size designResolutionSize { 1200,  720};
static Size designResolutionSize { 570,  320};
static Size windowSize           { 570, 320};

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
static Resource largeResource  {Size{1920, 1080}, Size{1024, 768}, "hd"};
static Resource mediumResource {Size{1024,  768}, Size{ 750, 544}, "md"};
static Resource smallResource  {Size{ 480,  320}, Size{   0,   0}, "sd"};

// Declare and array containing the resource descriptions, from largest to smallest
static std::array<Resource, 3> resources {{largeResource, mediumResource, smallResource}};

Vegolution::Vegolution()
: director_    {nullptr}
, fileUtils_   {nullptr}
, dataManager_ {}
{
    log("Creating Vegolution");
}

Vegolution::~Vegolution() {
    log("Destructing Vegolution");
}

// If you want a different context, just modify the value of glContextAttrs
// it will takes effect on all platforms
void Vegolution::initGLContextAttrs()
{
    // Set OpenGL context attributions, now can only set six attributions:
    // red, green, blue, alpha, depth, stencil
    GLContextAttrs glContextAttrs {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool Vegolution::applicationDidFinishLaunching()
{
    director_ = Director::getInstance();
    fileUtils_ = FileUtils::getInstance();
    dataManager_.init(fileUtils_);

    // Initialize director
    GLView* glview {director_->getOpenGLView()};
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("Prototwo", Rect{0, 0, windowSize.width, windowSize.height});
#else
        glview = GLViewImpl::create("Prototwo");
#endif
        director_->setOpenGLView(glview);
    }

    // Turn on display FPS
    director_->setDisplayStats(false);

    // Set FPS. the default value is 1.0/60 if you don't call this
    director_->setAnimationInterval(1.0 / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_HEIGHT);
    auto frameSize = glview->getFrameSize();
	log("Frame size is %fx%f", frameSize.width, frameSize.height);

	// Vector to build a list of resources paths
    std::vector<std::string> searchPaths;

	float scaleFactor {-1.0f};
	
	// Look through our resource definitions
     for (auto resource : resources) {
        // If the screen is wider or higher than the resolution of the resource
        if (frameSize.width > resource.useIfScreenOverSize.width) {
            // Add this directory to the search path
            searchPaths.push_back(resource.directory);
            log("Searching in %s", resource.directory);
            // If we haven't already determined the scale factor
            if (scaleFactor == -1) {
                scaleFactor = resource.size.height / designResolutionSize.height;
            }
            break;
        }
    }

    // Set scale factor and search paths
    director_->setContentScaleFactor(scaleFactor);
    fileUtils_->setSearchPaths(searchPaths);

    register_all_packages();

    // Create a scene. it's an autorelease object
    Scene* scene {MainLayer::createScene(this)};

    // Run
    director_->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void Vegolution::applicationDidEnterBackground() {
    director_->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// This function will be called when the app is active again
void Vegolution::applicationWillEnterForeground() {
    director_->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

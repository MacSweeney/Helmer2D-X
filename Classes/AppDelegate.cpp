//Samuel MacSweeney
//K00200955

#include "AppDelegate.h"
#include "LevelOne.h"

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
	#include "audio/include/AudioEngine.h"
	using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
	#include "audio/include/SimpleAudioEngine.h"
	using namespace CocosDenshion;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1920, 1080);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{};

AppDelegate::~AppDelegate() 
{
	#if USE_AUDIO_ENGINE
		AudioEngine::end();
	#elif USE_SIMPLE_AUDIO_ENGINE
		SimpleAudioEngine::end();
	#endif
};

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};
    GLView::setGLContextAttrs(glContextAttrs);
};

static int register_all_packages()
{
    return 0;
};

bool AppDelegate::applicationDidFinishLaunching() 
{
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    if(!glview) 
	{
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
			glview = GLViewImpl::createWithFullScreen("Helmer2D");
		#else
			glview = GLViewImpl::create("Helmer2D");
		#endif

		director->setOpenGLView(glview);
    }

    director->setDisplayStats(true);
    director->setAnimationInterval(1.0f / 60);

    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();

    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }

    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }

    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    register_all_packages();

	auto LevelOneScene = LevelOne::createScene();

	director->runWithScene(LevelOneScene);

    return true;
};

void AppDelegate::applicationDidEnterBackground() 
{
    Director::getInstance()->stopAnimation();

	#if USE_AUDIO_ENGINE
		AudioEngine::pauseAll();
	#elif USE_SIMPLE_AUDIO_ENGINE
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		SimpleAudioEngine::getInstance()->pauseAllEffects();
	#endif
};

void AppDelegate::applicationWillEnterForeground() 
{
    Director::getInstance()->startAnimation();

	#if USE_AUDIO_ENGINE
		AudioEngine::resumeAll();
	#elif USE_SIMPLE_AUDIO_ENGINE
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		SimpleAudioEngine::getInstance()->resumeAllEffects();
	#endif
};

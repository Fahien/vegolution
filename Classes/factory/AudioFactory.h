#ifndef __AUDIO_FACTORY_H__
#define __AUDIO_FACTORY_H__

static const char *const bullet = "audio/bullet.ogg";

static const char *const explosion = "audio/explosion.ogg";

static const char *const gameover = "audio/gameover.ogg";

static const char *const hit = "audio/hit.ogg";

static const char *const laser = "audio/laser.ogg";

static const char *const spider = "audio/spider.ogg";

static const char *const shot = "audio/water.ogg";

static const char *const tank = "audio/tank.ogg";

static const char *const soundtrack = "audio/soundtrack.mp3";

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class AudioFactory {
public:
    AudioFactory();

    void load(CocosDenshion::SimpleAudioEngine* audio);
    void unload();

    void playEffect(const char *const effect);
    void playMusic(const char *const music);

private:
    CocosDenshion::SimpleAudioEngine* audio_;
};
#endif //__AUDIO_FACTORY_H__

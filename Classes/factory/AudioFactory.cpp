#include "AudioFactory.h"

USING_NS_CC;

AudioFactory::AudioFactory()
        : audio_{nullptr}
{}

void AudioFactory::load(CocosDenshion::SimpleAudioEngine *audio)
{
    log("Loading audio");
    audio_ = audio;
    audio_->preloadEffect(bullet);
    audio_->preloadEffect(explosion);
    audio_->preloadEffect(gameover);
    audio_->preloadEffect(hit);
    audio_->preloadEffect(laser);
    audio_->preloadEffect(spider);
    audio_->preloadEffect(shot);
    audio_->preloadEffect(tank);
    audio_->preloadBackgroundMusic(soundtrack);
}

void AudioFactory::unload()
{
    log("Unloading audio");
    audio_->unloadEffect(bullet);
    audio_->unloadEffect(explosion);
    audio_->unloadEffect(gameover);
    audio_->unloadEffect(hit);
    audio_->unloadEffect(laser);
    audio_->unloadEffect(spider);
    audio_->unloadEffect(shot);
    audio_->unloadEffect(tank);
    audio_->unloadEffect(soundtrack);
}

void AudioFactory::playEffect(const char *const effect) {
    audio_->playEffect(effect);
}

void AudioFactory::playMusic(const char *const music) {
    audio_->playBackgroundMusic(music);
}




#include "CompositeWave.h"
#include "cocos2d.h"

USING_NS_CC;

//---------------------------------refactored with composite---------------------------------//
void CompositeWave::startWave() {
    if (waves.empty()) {
        CCLOG("Warning: No waves to start in CompositeWave");
        return;
    }

    float delay = 0;
    auto scheduler = Director::getInstance()->getScheduler();
    
    // 递归启动所有子波次
    for (Wave* wave : waves) {
        scheduler->schedule([wave](float dt) {
            wave->startWave();
        }, this, 0, 0, delay, false, "wave_" + std::to_string(delay));
        delay += 10.0f;
    }
}

void CompositeWave::addWave(Wave* wave) {
    waves.push_back(wave);
}

void CompositeWave::removeWave(Wave* wave) {
    auto it = std::find(waves.begin(), waves.end(), wave);
    if (it != waves.end()) {
        waves.erase(it);
    }
}

Wave* CompositeWave::getWave(int index) {
    if (index >= 0 && index < waves.size()) {
        return waves[index];
    }
    return nullptr;
}

CompositeWave::~CompositeWave() {
    for (Wave* wave : waves) {
        delete wave;
    }
} 
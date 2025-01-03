#include "WaveManager.h"

//---------------------------------refactored with composite---------------------------------//

WaveManager* WaveManager::instance = nullptr;

WaveManager* WaveManager::getInstance() {
    if (!instance) {
        instance = new WaveManager();
    }
    return instance;
}

void WaveManager::addWave(Wave* wave) {
    waves.push_back(wave);
}

void WaveManager::startAllWaves() {
    float delay = 0;
    auto scheduler = Director::getInstance()->getScheduler();
    
    for (Wave* wave : waves) {
        // 为每个主波次设置延迟启动
        scheduler->schedule([wave](float dt) {
            wave->startWave();
        }, this, 0, 0, delay, false, "main_wave_" + std::to_string(delay));
        
        delay += 20.0f; // 每个主波次间隔20秒
    }
}

void WaveManager::cleanup() {
    for (Wave* wave : waves) {
        delete wave;
    }
    waves.clear();
} 
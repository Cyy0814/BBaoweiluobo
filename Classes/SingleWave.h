#ifndef __SINGLE_WAVE_H__
#define __SINGLE_WAVE_H__

#include "Wave.h"
#include <vector>

//---------------------------------refactored with composite---------------------------------//
class SingleWave : public Wave {
public:
    void startWave() override;
    void addMonster(Monster* monster) override;
    
    // 添加缺失的虚函数实现
    void addWave(Wave* wave) override {
        throw std::runtime_error("Leaf node cannot add wave");
    }
    
    void removeWave(Wave* wave) override {
        throw std::runtime_error("Leaf node cannot remove wave");
    }
    
    Wave* getWave(int index) override {
        throw std::runtime_error("Leaf node cannot get wave");
    }
    
    bool isComposite() const override { return false; }

    ~SingleWave() {
        auto scheduler = Director::getInstance()->getScheduler();
        scheduler->unschedule("spawn_monster", this);
    }

private:
    std::vector<Monster*> monsters;
};

#endif // __SINGLE_WAVE_H__ 
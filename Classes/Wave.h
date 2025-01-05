#ifndef __WAVE_H__
#define __WAVE_H__

#include "Monster.h"

//---------------------------------refactored with composite---------------------------------//
class Wave {
public:
    virtual ~Wave() {}
    
    // 核心操作
    virtual void startWave() = 0;
    
    // 组合操作
    virtual void addWave(Wave* wave) = 0;
    virtual void removeWave(Wave* wave) = 0;
    virtual Wave* getWave(int index) = 0;
    
    // 叶子操作
    virtual void addMonster(Monster* monster) = 0;
    
    // 安全检查
    virtual bool isComposite() const = 0;
    
protected:
    float spawnInterval = 2.0f;
};

#endif // __WAVE_H__ 
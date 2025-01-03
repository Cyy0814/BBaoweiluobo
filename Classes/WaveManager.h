#ifndef __WAVE_MANAGER_H__
#define __WAVE_MANAGER_H__

#include "Wave.h"
#include "SingleWave.h"
#include "CompositeWave.h"
#include <vector>

//---------------------------------refactored with composite---------------------------------//
class WaveManager {
public:
    static WaveManager* getInstance();
    void addWave(Wave* wave);
    void startAllWaves();
    void cleanup();
    
    // 添加析构函数
    ~WaveManager() {
        cleanup();
    }
    
private:
    WaveManager() {}
    static WaveManager* instance;
    std::vector<Wave*> waves;
};

#endif // __WAVE_MANAGER_H__ 
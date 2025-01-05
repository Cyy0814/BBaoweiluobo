#ifndef __COMPOSITE_WAVE_H__
#define __COMPOSITE_WAVE_H__

#include "Wave.h"
#include <vector>

//---------------------------------refactored with composite---------------------------------//
class CompositeWave : public Wave {
public:
    virtual ~CompositeWave();
    
    // 核心操作实现
    virtual void startWave() override;
    
    // 组合操作实现
    virtual void addWave(Wave* wave) override;
    virtual void removeWave(Wave* wave) override;
    virtual Wave* getWave(int index) override;
    
    // 叶子操作实现（组合节点不支持）
    virtual void addMonster(Monster* monster) override {
        throw std::runtime_error("Composite cannot add monster directly");
    }
    
    virtual bool isComposite() const override { return true; }

private:
    std::vector<Wave*> waves;
};

#endif // __COMPOSITE_WAVE_H__ 
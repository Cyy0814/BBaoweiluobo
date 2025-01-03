#ifndef __GOLD_COIN_VIEW_H__
#define __GOLD_COIN_VIEW_H__

#include "cocos2d.h"
#include "Observer.h"
#include "GoldCoin.h"

// Refactored with Observer Pattern
class GoldCoinView : public cocos2d::Node, public Observer {
private:
    cocos2d::Label* goldLabel;
    GoldCoin* goldCoin;

public:
    static GoldCoinView* create(GoldCoin* goldCoin);
    virtual bool init(GoldCoin* goldCoin);
    virtual ~GoldCoinView();
    
    // UI相关
    void setupUI();
    void updateGoldLabel(int value);
    
    // 观察者接口实现
    virtual void update(const std::string& message, int value) override;
    
    // 清理方法
    void cleanup() override;
    void removeFromParent() override;
};

#endif 
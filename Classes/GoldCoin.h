#ifndef __GOLD_COIN_H__
#define __GOLD_COIN_H__

#include "cocos2d.h"
#include "Subject.h"

// Refactored with Observer Pattern
class GoldCoin : public cocos2d::Node, public Subject {
private:
    int m_value;
    void notifyGoldChanged();

public:
    virtual bool init() override;
    
    // 获取/设置金币值
    int getValue() const { return m_value; }
    void setValue(int value);
    
    // 金币操作
    void updateGoldValue(int goldValue);


    // 击败怪物获得金币
    void earnGold(int amount);
    
    // 购买炮塔消耗金币
    bool purchaseTower(int cost);

    CREATE_FUNC(GoldCoin);
};

#endif
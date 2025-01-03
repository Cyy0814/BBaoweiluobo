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
    
    // ��ȡ/���ý��ֵ
    int getValue() const { return m_value; }
    void setValue(int value);
    
    // ��Ҳ���
    void updateGoldValue(int goldValue);


    // ���ܹ����ý��
    void earnGold(int amount);
    
    // �����������Ľ��
    bool purchaseTower(int cost);

    CREATE_FUNC(GoldCoin);
};

#endif
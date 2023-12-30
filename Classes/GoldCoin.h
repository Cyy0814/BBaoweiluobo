#ifndef __GOLD_COIN_H__
#define __GOLD_COIN_H__

#include "cocos2d.h"

USING_NS_CC;

class GoldCoin : public Node
{
public:

    int m_value;
    Label* m_label;

    virtual bool init() override;
  
    //���½��
    void updateGoldValue(int goldValue);


    // ���ܹ����ý��
    void earnGold(int amount);
    
    // �����������Ľ��
    bool purchaseTower(int cost);
    CREATE_FUNC(GoldCoin);

private:
    void updateGoldLabel();
};
#endif
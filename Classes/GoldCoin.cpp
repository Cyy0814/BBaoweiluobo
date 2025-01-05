#include "GoldCoin.h"

USING_NS_CC;

bool GoldCoin::init()
{
    if (!Node::init())
    {
        return false;
    }

    m_value = 0;  // 初始化金币数
    return true;
}

/*Refactored with Observer Pattern*/
void GoldCoin::notifyGoldChanged()
{
    notifyObservers("gold_changed", m_value);
}

void GoldCoin::setValue(int value)
{
    if (m_value != value)
    {
        m_value = value;
        notifyGoldChanged();
    }
}

void GoldCoin::updateGoldValue(int goldValue)
{
    setValue(goldValue);
}

void GoldCoin::earnGold(int amount)
{
    setValue(m_value + amount);
}

bool GoldCoin::purchaseTower(int cost)
{
    if (m_value >= cost)
    {
        setValue(m_value - cost);
        return true;
    }
    return false;
}
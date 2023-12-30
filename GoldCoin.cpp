#include"cocos2d.h"
#include"GameScene.h"
#include "Money.h"

USING_NS_CC;

bool GoldCoin::init()
{
    if (!Node::init())
    {
        return false;
    }

    m_value = 0;

    // ����������ʾ��������ı�ǩ
    m_label = Label::createWithTTF("300", "fonts/arial.ttf", 24);
    m_label->setPosition(Vec2(100, 100));
    addChild(m_label);

    return true;
}

//���ܹ���
void GoldCoin::earnGold(int amount)
{
    m_value += amount;
    updateGoldLabel();
}
//����
bool GoldCoin::purchaseTower(int cost)
{
    if (m_value >= cost)
    {
        m_value -= cost;
        updateGoldLabel();
        return true;
    }

    return false;
}
void GoldCoin::updateGoldValue(int goldValue)
{
    m_value = goldValue;
    this->m_label->setString(std::to_string(m_value));
}

void GoldCoin::updateGoldLabel()
{
    this->m_label->setString("Gold: " + std::to_string(m_value));
}
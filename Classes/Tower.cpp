#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "cocos/ui/UIImageView.h"
#include "Tower.h"


USING_NS_CC;

using namespace cocos2d::ui;
using namespace cocos2d;

Tower::Tower() : level(1), attackSpeed(1.0f), towerType(0)
{
    // ��ʼ������
}

//Tower::~Tower()
//{
//    // �������
//}
Tower* Tower::create(Vec2 position,int towerType)
{
    Tower* tower = new (std::nothrow) Tower();
    if (tower && tower->initWithFile("tower_base.png"))
    {
        tower->autorelease();
        tower->towerType = towerType;
        tower->setPosition(position);
        // �������ͳ�ʼ����ͬ������
        return tower;
    }
    CC_SAFE_DELETE(tower);
    return nullptr;
}

bool Tower::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    // ��ʼ����̨
    return true;
}
void Tower::attack()
{
    // ʵ�ֹ����߼�
    // ������漰������һ�� Projectile �࣬�����䷢�䵽Ŀ��
}

void Tower::upgrade()
{
    // ������̨���������
    //��ԭ�ȵ���̨�Ƴ�����������һ������̨
}

void Tower::remove()
{
    // �Ƴ���̨
    this->removeFromParentAndCleanup(true);
}
void Tower::showAttackRange() {

    // ��ʾ������Χ
} 
void Tower::showUpgradeAndRemoveButtons() {

    // ��ʾ�������Ƴ���ť
}
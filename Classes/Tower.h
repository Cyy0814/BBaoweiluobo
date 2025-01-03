#ifndef __TOWER_H__
#define __TOWER_H__

#include "cocos2d.h"
#include "Monster.h"
#include "ui/CocosGUI.h"
#include "BulletFlyweightFactory.h"
#include "Bullet.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;
using namespace cocos2d::ui;


class Tower : public cocos2d::Sprite
{
public:
    Tower();
    /*virtual ~Tower();//��sprite�����Ѿ�������������*/

    static Tower* create(Vec2 position,int towerType);//����һ��������ָ��λ�ü���ͼƬ

    virtual bool init() override;//�жϳ�ʼ���Ƿ�ɹ�
    virtual void attack();//������ʵ����һ��bullet��Ѱ·��������������ʧ���ٴ����ɡ���׼ʱҪ�ı�Ƕ�
    virtual void upgrade() ;//�������������Ե��������ı�ͼƬ
    virtual void remove() ;//�Ƴ�
    void showAttackRange(); // ��ʾ������Χ
    void showUpgradeAndRemoveButtons(); // ��ʾ�������Ƴ���ť
    void hideAttackRangeAndButtons();
protected:
    int level; // �ȼ�
    int towerType;
    std::vector<Monster*> monstersInRange; // �ڷ�Χ�ڵĹ�������
    int attackDamage; // �����˺�
    float attackRange; //������Χ
    float attackSpeed; // �����ٶ�
    float timeSinceLastAttack;

    cocos2d::ui::Button* upgradeButton; // ������ť
    cocos2d::ui::Button* removeButton;  // �Ƴ���ť
    Sprite* attackRange_;
};
#endif
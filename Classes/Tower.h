#ifndef __TOWER_H__
#define __TOWER_H__

#include "cocos2d.h"
#include "Monster.h"
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
    virtual void showAttackRange(); // ��ʾ������Χ
    virtual void showUpgradeAndRemoveButtons(); // ��ʾ�������Ƴ���ť
protected:
    int level;
    int towerType;
    std::vector<Monster*> monstersInRange; // �ڷ�Χ�ڵĹ�������
    float attackRange; //������Χ
    float attackSpeed; // �����ٶ�
    float timeSinceLastAttack;
};
#endif
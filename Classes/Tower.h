#ifndef __TOWER_H__
#define __TOWER_H__

#include "cocos2d.h"
#include "Monster.h"
#include "ui/CocosGUI.h"
#include "IAttackStrategy.h"
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
    /**
     * @brief 设置攻击策略
     * @param strategy 具体的攻击策略实现
     */
    void setAttackStrategy(IAttackStrategy* strategy) {
        attackStrategy = strategy;
    }
    virtual ~Tower() {
        if(attackStrategy) {
            delete attackStrategy;
            attackStrategy = nullptr;
        }
    }
protected:
    int level; // ȼ
    int towerType;
    std::vector<Monster*> monstersInRange; // ڷΧڵĹ
    int attackDamage; // ˺
    float attackRange; //Χ
    float attackSpeed; // ٶ
    float timeSinceLastAttack;

    cocos2d::ui::Button* upgradeButton; // ť
    cocos2d::ui::Button* removeButton;  // Ƴť
    Sprite* attackRange_;
    IAttackStrategy* attackStrategy;
};
#endif
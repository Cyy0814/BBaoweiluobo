// Bottle.h
#ifndef __Bottle_H__
#define __Bottle_H__

#include "cocos2d.h"
#include "Monster.h"
#include "Bullet.h"
#include "Tower.h"
#include "ui/CocosGUI.h"

class Bottle : public Tower {
public:
    static Bottle* create(const cocos2d::Vec2& position);

    // ��������߼�
    void update(float dt, std::vector<Monster*> monsters);
    bool isMonsterInRange(Monster* monster);
    void checkForMonstersInRange(std::vector<Monster*> monsters);
    void attack(Monster* target);

    void upgrade();
    void remove() override;
    void showAttackRange()override; // ��ʾ������Χ
    void showUpgradeAndRemoveButtons()override; // ��ʾ�������Ƴ���ť
    void hideAttackRangeAndButtons(); // �Ƴ�

private:
    cocos2d::ui::Button *upgradeButton; // ������ť
    cocos2d::ui::Button *removeButton;  // �Ƴ���ť
    Sprite* attackRange_;
};

#endif // __Bottle_H__

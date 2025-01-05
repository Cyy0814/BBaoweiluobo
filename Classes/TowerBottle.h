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

    // 攻击相关逻辑
    void update(float dt, std::vector<Monster*> monsters);
    bool isMonsterInRange(Monster* monster);
    void checkForMonstersInRange(std::vector<Monster*> monsters);
    void attack(Monster* target);

    void upgrade();
    void remove() override;
    void showAttackRange()override; // 显示攻击范围
    void showUpgradeAndRemoveButtons()override; // 显示升级和移除按钮
    void hideAttackRangeAndButtons(); // 移除

private:
    cocos2d::ui::Button *upgradeButton; // 升级按钮
    cocos2d::ui::Button *removeButton;  // 移除按钮
    Sprite* attackRange_;
};

#endif // __Bottle_H__

#ifndef __TOWER_H__
#define __TOWER_H__

#include "cocos2d.h"
#include "Monster.h"
#include "TowerState.h"
#include "ui/CocosGUI.h"
#include "IAttackStrategy.h"
#include "BulletFlyweightFactory.h"
#include "Bullet.h"
using namespace cocos2d;
using namespace cocos2d::ui;

class Tower : public cocos2d::Sprite {
protected:
    int level;
    int towerType;
    float attackRange;
    float attackSpeed;
    float timeSinceLastAttack;
    int attackDamage;
    TowerState* currentState;
    IAttackStrategy* attackStrategy;
    
    std::vector<Monster*> monstersInRange;
    cocos2d::ui::Button* upgradeButton;
    cocos2d::ui::Button* removeButton;
    cocos2d::Sprite* attackRange_;

public:
    Tower();
    virtual ~Tower();
    
    static Tower* create(cocos2d::Vec2 position, int towerType);
    virtual bool init() override;
    
    //---------------------------------refactored with state pattern---------------------------------//
    // 状态相关
    void changeState(TowerState* newState);
    TowerState* getCurrentState() const { return currentState; }
    
    // 攻击相关
    virtual void attack(Monster* target) = 0;
    float getAttackRange() const { return attackRange; }
    float getAttackInterval() const { return 1.0f / attackSpeed; }
    int getDamage() const { return attackDamage; }
    bool isMonsterInRange(Monster* monster);
    void checkForMonstersInRange(std::vector<Monster*> monsters);
    
    // 塔的功能
    virtual void upgrade() = 0;
    virtual void remove();
    void showAttackRange();
    void showUpgradeAndRemoveButtons();
    void hideAttackRangeAndButtons();
    
    // 更新函数
    virtual void update(float dt, std::vector<Monster*> monsters);

    //---------------------------------refactored with strategy pattern---------------------------------//
    // 攻击策略
    void setAttackStrategy(IAttackStrategy* strategy) {
        if (attackStrategy) {
            delete attackStrategy;
        }
        attackStrategy = strategy;
    }
};

#endif
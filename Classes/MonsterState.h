#ifndef __MONSTER_STATE_H__
#define __MONSTER_STATE_H__

#include "cocos2d.h"

class Monster;
// Refactored with State Pattern
// 怪物状态基类
class MonsterState {
public:
    virtual ~MonsterState() {}
    
    // 进入该状态时的行为
    virtual void enter(Monster* monster) = 0;
    
    // 状态更新
    virtual void update(Monster* monster, float dt) = 0;
    
    // 退出该状态时的行为
    virtual void exit(Monster* monster) = 0;
    
    // 处理受到伤害的行为
    virtual void handleDamage(Monster* monster, int damage) = 0;
};

// 正常行走状态
class WalkState : public MonsterState {
public:
    virtual void enter(Monster* monster) override;
    virtual void update(Monster* monster, float dt) override;
    virtual void exit(Monster* monster) override;
    virtual void handleDamage(Monster* monster, int damage) override;
};

// 减速状态
class SlowedState : public MonsterState {
private:
    float slowFactor;      // 减速系数
    float remainingTime;   // 减速剩余时间
    float originalSpeed;   // 原始速度

public:
    SlowedState(float factor = 0.5f, float duration = 3.0f) 
        : slowFactor(factor), remainingTime(duration), originalSpeed(0.0f) {}
    
    virtual void enter(Monster* monster) override;
    virtual void update(Monster* monster, float dt) override;
    virtual void exit(Monster* monster) override;
    virtual void handleDamage(Monster* monster, int damage) override;
};

// 死亡状态
class DeathState : public MonsterState {
public:
    virtual void enter(Monster* monster) override;
    virtual void update(Monster* monster, float dt) override;
    virtual void exit(Monster* monster) override;
    virtual void handleDamage(Monster* monster, int damage) override;
};

#endif
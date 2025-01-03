#ifndef __TOWER_STATE_H__
#define __TOWER_STATE_H__

#include "cocos2d.h"
#include <vector>

class Tower;
class Monster;

// Refactored with State Pattern
// 塔的状态基类
class TowerState {
public:
    virtual ~TowerState() {}
    
    // 进入该状态时的行为
    virtual void enter(Tower* tower) = 0;
    
    // 状态更新
    virtual void update(Tower* tower, float dt, std::vector<Monster*> monsters) = 0;
    
    // 退出该状态时的行为
    virtual void exit(Tower* tower) = 0;
};

// 待机状态
class IdleState : public TowerState {
public:
    virtual void enter(Tower* tower) override;
    virtual void update(Tower* tower, float dt, std::vector<Monster*> monsters) override;
    virtual void exit(Tower* tower) override;
};

// 攻击状态
class AttackState : public TowerState {
private:
    float attackTimer;
    
public:
    AttackState() : attackTimer(0.0f) {}
    virtual void enter(Tower* tower) override;
    virtual void update(Tower* tower, float dt, std::vector<Monster*> monsters) override;
    virtual void exit(Tower* tower) override;
};

#endif 
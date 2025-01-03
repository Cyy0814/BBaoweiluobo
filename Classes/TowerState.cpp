#include "TowerState.h"
#include "Tower.h"
#include "Monster.h"

// Refactored with State Pattern
// 待机状态实现
void IdleState::enter(Tower* tower) {
    // Do nothing , Maybe play animation
}

void IdleState::update(Tower* tower, float dt, std::vector<Monster*> monsters) {
    tower->checkForMonstersInRange(monsters);
    if (!tower->monstersInRange.empty()) {
        // 发现目标，切换到攻击状态
        tower->changeState(new AttackState());
    }
}

void IdleState::exit(Tower* tower) {
    // Do nothing
}

// 攻击状态实现
void AttackState::enter(Tower* tower) {
    attackTimer = 0.0f;
}

void AttackState::update(Tower* tower, float dt, std::vector<Monster*> monsters) {
    tower->checkForMonstersInRange(monsters);
    if (tower->monstersInRange.empty()) {
        // 目标消失，切换回待机状态
        tower->changeState(new IdleState());
        return;
    }

    // 更新攻击计时器
    attackTimer += dt;
    if (attackTimer >= tower->getAttackInterval()) {
        // 发动攻击
        tower->attack(tower->monstersInRange.front());
        attackTimer = 0.0f;
    }
}

void AttackState::exit(Tower* tower) {
    // 退出攻击状态的清理工作
    attackTimer = 0.0f;
} 
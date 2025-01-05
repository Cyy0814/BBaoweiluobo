#include "MonsterState.h"
#include "Monster.h"

//---------------------------------refactored with state pattern---------------------------------//
// WalkState实现
void WalkState::enter(Monster* monster) {
    // 设置正常速度
    monster->setMoveSpeed(monster->speed);  // 使用Monster类中定义的原始速度
}

void WalkState::update(Monster* monster, float dt) {
    // 行走状态不需要特殊更新
}

void WalkState::exit(Monster* monster) {
    // 清理工作（如有需要）
}

void WalkState::handleDamage(Monster* monster, int damage) {
    if (!monster->dead) {
        monster->_hitPoints -= damage;
        if (monster->_hitPoints <= 0) {
            monster->_hitPoints = 0;
            monster->changeState(new DeathState());
        } else {
            monster->showHitEffect();
        }
    }
}

// SlowedState实现
void SlowedState::enter(Monster* monster) {
    // 保存原始速度并减速
    originalSpeed = monster->getMoveSpeed();
    monster->setMoveSpeed(originalSpeed * slowFactor);
    
    // 设置减速效果
    monster->showSlowEffect();
}

void SlowedState::update(Monster* monster, float dt) {
    // 更新减速时间
    remainingTime -= dt;
    if (remainingTime <= 0) {
        // 减速时间结束，恢复正常状态
        monster->changeState(new WalkState());
    }
}

void SlowedState::exit(Monster* monster) {
    // 恢复原始速度
    monster->setMoveSpeed(originalSpeed);
    monster->hideSlowEffect();
}

void SlowedState::handleDamage(Monster* monster, int damage) {
    if (!monster->dead) {
        monster->_hitPoints -= damage;
        if (monster->_hitPoints <= 0) {
            monster->_hitPoints = 0;
            monster->changeState(new DeathState());
        } else {
            monster->showHitEffect();
        }
    }
}

// DeathState实现
void DeathState::enter(Monster* monster) {
    monster->dead = true;
}

void DeathState::update(Monster* monster, float dt) {
    monster->die();
}

void DeathState::exit(Monster* monster) {
    
}

void DeathState::handleDamage(Monster* monster, int damage) {
    // 死亡状态下不再处理伤害
} 
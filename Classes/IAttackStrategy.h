#ifndef __IATTACK_STRATEGY_H__
#define __IATTACK_STRATEGY_H__

#include "cocos2d.h"
#include "Monster.h"

/**
 * @brief 攻击策略的抽象接口类
 * 定义了防御塔攻击行为的抽象接口
 */
class IAttackStrategy {
public:
    /**
     * @brief 执行攻击行为
     * @param position 攻击发起的位置
     * @param target 攻击目标
     * @param damage 攻击伤害值
     */
    virtual void attack(cocos2d::Vec2 position, Monster* target, float damage) = 0;
    virtual ~IAttackStrategy() {}
};

/**
 * @brief 瓶子塔攻击策略
 * 实现普通直线弹道攻击
 */
class BottleAttackStrategy : public IAttackStrategy {
public:
    virtual void attack(cocos2d::Vec2 position, Monster* target, float damage) override;
};

/**
 * @brief 便便塔攻击策略  
 * 实现带减速效果的攻击
 */
class ShitAttackStrategy : public IAttackStrategy {
public:
    virtual void attack(cocos2d::Vec2 position, Monster* target, float damage) override;
};

#endif 
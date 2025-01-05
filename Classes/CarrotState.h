#ifndef __CARROT_STATE_H__
#define __CARROT_STATE_H__

#include "cocos2d.h"

class Carrot;

//---------------------------------refactored with state pattern---------------------------------//
// 萝卜状态基类
class CarrotState {
public:
    virtual ~CarrotState() {}
    
    // 获取该状态对应的纹理路径
    virtual std::string getTexturePath() const = 0;
    
    // 处理状态变化
    virtual void handleHealthChange(Carrot* carrot, int health) = 0;
    
    // 获取状态名称(用于调试)
    virtual std::string getStateName() const = 0;
};

// 满血状态 (10血)
class HealthyState : public CarrotState {
public:
    virtual std::string getTexturePath() const override { return "myCarrot/HP_MAX.PNG"; }
    virtual void handleHealthChange(Carrot* carrot, int health) override;
    virtual std::string getStateName() const override { return "Healthy"; }
};

// 轻伤状态 (9血)
class SlightlyDamagedState : public CarrotState {
public:
    virtual std::string getTexturePath() const override { return "myCarrot/HP_9.PNG"; }
    virtual void handleHealthChange(Carrot* carrot, int health) override;
    virtual std::string getStateName() const override { return "SlightlyDamaged"; }
};

// 中度受伤状态 (7-8血)
class ModeratelyDamagedState : public CarrotState {
public:
    virtual std::string getTexturePath() const override { return "myCarrot/HP_7-8.PNG"; }
    virtual void handleHealthChange(Carrot* carrot, int health) override;
    virtual std::string getStateName() const override { return "ModeratelyDamaged"; }
};

// 严重受伤状态 (5-6血)
class SeverelyDamagedState : public CarrotState {
public:
    virtual std::string getTexturePath() const override { return "myCarrot/HP_5-6.PNG"; }
    virtual void handleHealthChange(Carrot* carrot, int health) override;
    virtual std::string getStateName() const override { return "SeverelyDamaged"; }
};

// 危急状态 (4血)
class CriticalState : public CarrotState {
public:
    virtual std::string getTexturePath() const override { return "myCarrot/HP_4.PNG"; }
    virtual void handleHealthChange(Carrot* carrot, int health) override;
    virtual std::string getStateName() const override { return "Critical"; }
};

// 濒死状态 (3血)
class DyingState : public CarrotState {
public:
    virtual std::string getTexturePath() const override { return "myCarrot/HP_3.PNG"; }
    virtual void handleHealthChange(Carrot* carrot, int health) override;
    virtual std::string getStateName() const override { return "Dying"; }
};

// 垂死状态 (2血)
class NearDeathState : public CarrotState {
public:
    virtual std::string getTexturePath() const override { return "myCarrot/HP_2.PNG"; }
    virtual void handleHealthChange(Carrot* carrot, int health) override;
    virtual std::string getStateName() const override { return "NearDeath"; }
};

// 最后一口气状态 (1血)
class LastBreathState : public CarrotState {
public:
    virtual std::string getTexturePath() const override { return "myCarrot/HP_1.PNG"; }
    virtual void handleHealthChange(Carrot* carrot, int health) override;
    virtual std::string getStateName() const override { return "LastBreath"; }
};

#endif 
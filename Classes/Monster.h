#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "cocos2d.h"
#include "MonsterState.h"
#include "Global.h"
#include "ui/CocosGUI.h"

//---------------------------------refactored with state pattern---------------------------------//
class Monster : public cocos2d::Sprite {
    friend class MonsterState;
    friend class WalkState;
    friend class SlowedState;
    friend class DeathState;

public:
    static Monster* createWithType(int monsterType);

    float speed = 1000;
    bool dead = false;

    void moveOnPath(const std::vector<cocos2d::Vec2>& path);
    void getAttacked(int damage);
    void handleSlowEffect(int damage);
    void showHitEffect();
    void showSlowEffect();
    void hideSlowEffect();
    void dropCoins();
    void removeFromMonstersArray(Monster* monster);
    void setAnimation(const std::string& name);

    float getMoveSpeed() const { return speed; }
    void setMoveSpeed(float newSpeed) { speed = newSpeed; }

    CC_SYNTHESIZE_READONLY(int, _hitPoints, HitPoints);    // 使用 CC_SYNTHESIZE_READONLY 宏生成 getter 方法
    CC_SYNTHESIZE_READONLY(int, _monsterType, MonsterType);
    CC_SYNTHESIZE_READONLY(cocos2d::Label*, _hpLabel, HPLabel); // 血量值显示

    // 状态相关的方法声明
    void changeState(MonsterState* newState);
    MonsterState* getCurrentState() const { return currentState; }
    virtual void update(float dt) override;

protected:
    Monster(int monsterType, int hitPoints);
    virtual bool init() override;

private:
    bool isAlive = true;
    void updateHPLabel();
    void die();

    // 状态相关的成员变量
    MonsterState* currentState;
    cocos2d::Sprite* slowEffect = nullptr;

};

#endif
#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "cocos2d.h"

class Monster : public cocos2d::Sprite {
public:
    static Monster* createWithType(int monsterType);

    void moveOnPath(const std::vector<cocos2d::Vec2>& path);
    void getAttacked(int damage);
    void showHitEffect();
    void dropCoins();

    CC_SYNTHESIZE_READONLY(int, _hitPoints, HitPoints); // ʹ�� CC_SYNTHESIZE_READONLY �������� getter ����
    CC_SYNTHESIZE_READONLY(int, _monsterType, MonsterType);
    CC_SYNTHESIZE_READONLY(cocos2d::Label*, _hpLabel, HPLabel); // ����ֵ��ʾ

protected:
    Monster(int monsterType, int hitPoints);
    virtual bool init() override;

private:
    void updateHPLabel();
    void die(); // ����������ʱ����
};

#endif

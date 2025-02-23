#ifndef __Bottle_H__
#define __Bottle_H__

#include "cocos2d.h"
#include "Monster.h"
#include "BottleBullet.h"
#include "Tower.h"
#include "ui/CocosGUI.h"
#include "BulletFlyweightFactory.h"

class Bottle : public Tower {
public:
    static Bottle* create(const cocos2d::Vec2& position);

    // 攻击相关逻辑
    void update(float dt, std::vector<Monster*> monsters);
    void attack(Monster* target);

    void upgrade();
    void remove() override;

private:
    Sprite* base;
};

#endif
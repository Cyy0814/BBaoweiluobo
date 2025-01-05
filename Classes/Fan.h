#ifndef __FAN_H__
#define __FAN_H__

#include "cocos2d.h"
#include "Monster.h"
#include "FanBullet.h"
#include "Tower.h"
#include "ui/CocosGUI.h"

class Fan : public Tower {
public:
    static Fan* create(const cocos2d::Vec2& position);

    // 攻击相关逻辑
    void update(float dt, std::vector<Monster*> monsters);
    void attack(Monster* target);

    void upgrade();
    void remove() override;


private:
    cocos2d::Sprite* topSprite;
    cocos2d::Sprite* bottomSprite;

    FanBullet* fanbullet;
};
#endif

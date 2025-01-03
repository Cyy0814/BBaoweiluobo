#ifndef __Bottle_H__
#define __Bottle_H__

#include "cocos2d.h"
#include "Monster.h"
#include "BottleBullet.h"
#include "Tower.h"
#include "ui/CocosGUI.h"

class Bottle : public Tower {
public:
    static Bottle* create(const cocos2d::Vec2& position);

    // ��������߼�
    void update(float dt, std::vector<Monster*> monsters);
    void attack(Monster* target);

    void upgrade();
    void remove() override;

private:
    Bullet* bullet;
    Sprite* base;
};

#endif
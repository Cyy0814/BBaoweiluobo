// Bullet.h
#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"
#include "Monster.h"
#include "Flyweight.h"

//----------------------------refactored with flyweight----------------------------//
class Bullet : public cocos2d::Sprite, public Flyweight {
public:
    static Bullet* createWithTarget(Monster* target, const std::string& filename, float speed, int damage);
    void initOptions(Monster* target, float speed, int damage);
    
    virtual void moveToTarget() override;
    virtual void onHitMonster(Monster* monster) override;
    virtual void setTarget(Monster* target) override;

protected:
    Monster* target;
    float speed;
    int damage;
};

#endif // __BULLET_H__
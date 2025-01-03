#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "cocos2d.h"

class Monster : public cocos2d::Sprite {
public:
    static Monster* createWithType(int monsterType);

    float speed = 1000;
    float halfspeed = speed / 2;
    bool dead = false;

    void moveOnPath(const std::vector<cocos2d::Vec2>& path);
    void getAttacked(int damage);
    void showHitEffect();
    void dropCoins();
    void removeFromMonstersArray(Monster* monster);

    CC_SYNTHESIZE_READONLY(int, _hitPoints, HitPoints); // ʹ�� CC_SYNTHESIZE_READONLY �������� getter ����
    CC_SYNTHESIZE_READONLY(int, _monsterType, MonsterType);
    CC_SYNTHESIZE_READONLY(cocos2d::Label*, _hpLabel, HPLabel); // ����ֵ��ʾ

    void takeHalfspeed()
    {
        this->speed = halfspeed;
    }
    void beSloweddown()
    {
        auto bottomSprite = Sprite::create("Tower/Shit/ID2_10.PNG");
        addChild(bottomSprite);
        bottomSprite->setPosition(cocos2d::Vec2(40, -bottomSprite->getContentSize().height / 2 + 10));
    }

    void setMoveCallback(std::function<void()> callback) {
        moveCallback = callback;
    }
    
    void startMoving() {
        if (moveCallback) {
            moveCallback();
        } else {
            CCLOG("Warning: Monster move callback not set!");
        }
    }

protected:
    Monster(int monsterType, int hitPoints);
    virtual bool init() override;

private:
    bool isAlive = true;
    void updateHPLabel();
    void die(); // ����������ʱ����
    std::function<void()> moveCallback;
};

#endif
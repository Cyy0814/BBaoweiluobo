#include "IAttackStrategy.h"
#include "BottleBullet.h"
#include "ShitBullet.h" 
#include "FanBullet.h"

USING_NS_CC;

// 瓶子塔的攻击实现
void BottleAttackStrategy::attack(cocos2d::Vec2 position, Monster* target, float damage) {
    if (!target || !target->getParent()) return;
    
    auto bullet = Bullet::createWithTarget(target, "Tower/Bottle/ID1_0.PNG", 1000.0f, damage);
    if (bullet) {
        bullet->setPosition(position);
        target->getParent()->addChild(bullet);
    }
}

// 风扇塔的攻击实现
void FanAttackStrategy::attack(cocos2d::Vec2 position, Monster* target, float damage) {
    if (!target || !target->getParent()) {
        return;
    }
    
    // 创建并发射旋转弹药
    auto bullet = FanBullet::createWithTarget(target, "Tower/Fan/ID4_6.PNG", 1000.0f, damage);
    if (bullet) {
        bullet->setPosition(position);
        target->getParent()->addChild(bullet);
    }
}

// 便便塔的攻击实现
void ShitAttackStrategy::attack(cocos2d::Vec2 position, Monster* target, float damage) {
    if (!target || !target->getParent()) return;
    
    auto bullet = ShitBullet::createWithTarget(target, "Tower/Shit/ID2_43.PNG", 1000.0f, damage);
    if (bullet) {
        bullet->setPosition(position);
        target->getParent()->addChild(bullet);
        target->takeHalfspeed();
        target->beSloweddown();
    }
} 
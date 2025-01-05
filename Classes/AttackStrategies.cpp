#include "IAttackStrategy.h"
#include "BottleBullet.h"
#include "ShitBullet.h" 
#include "FanBullet.h"
#include "BulletFlyweightFactory.h"

USING_NS_CC;

//---------------------------------refactored with strategy pattern---------------------------------//
// 瓶子塔的攻击实现
void BottleAttackStrategy::attack(cocos2d::Vec2 position, Monster* target, float damage) {
    if (!target || !target->getParent()) return;
    
    //---------------------------------refactored with flyweight pattern---------------------------------//
    // 使用享元工厂获取子弹
    Flyweight* bullet = BulletFlyweightFactory::getBullet("Tower/Bottle/ID1_0.PNG", attackSpeed, attackDamage);
    
    // 设置目标并添加到场景
    Bullet* concreteBullet = dynamic_cast<Bullet*>(bullet);
    if (concreteBullet) {
        concreteBullet->setTarget(target);
        concreteBullet->setPosition(getPosition());
        this->getParent()->addChild(concreteBullet);
    }
}

// 风扇塔的攻击实现
void FanAttackStrategy::attack(cocos2d::Vec2 position, Monster* target, float damage) {
    if (!target || !target->getParent()) {
        return;
    }
    
    //---------------------------------refactored with flyweight pattern---------------------------------//
    // 使用享元工厂获取子弹
    Flyweight* bullet = BulletFlyweightFactory::getBullet("Tower/Fan/ID1_0.PNG", attackSpeed, attackDamage);
    
    // 设置目标并添加到场景
    Bullet* concreteBullet = dynamic_cast<Bullet*>(bullet);
    if (concreteBullet) {
        concreteBullet->setTarget(target);
        concreteBullet->setPosition(getPosition());
        this->getParent()->addChild(concreteBullet);
    }
}

// 便便塔的攻击实现
void ShitAttackStrategy::attack(cocos2d::Vec2 position, Monster* target, float damage) {
    if (!target || !target->getParent()) return;
    
    //---------------------------------refactored with flyweight pattern---------------------------------//
    // 使用享元工厂获取子弹
    Flyweight* bullet = BulletFlyweightFactory::getBullet("Tower/Shit/ID2_43.PNG", attackSpeed, attackDamage);
    
    // 设置目标并添加到场景
    Bullet* concreteBullet = dynamic_cast<Bullet*>(bullet);
    if (concreteBullet) {
        concreteBullet->setTarget(target);
        concreteBullet->setPosition(getPosition());
        this->getParent()->addChild(concreteBullet);
} 
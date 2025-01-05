// Bullet.cpp
#include "Bullet.h"

USING_NS_CC;

Bullet* Bullet::createWithTarget(Monster* target, const std::string& filename, float speed, int damage) {
    Bullet* bullet = new (std::nothrow) Bullet();
    if (bullet && bullet->initWithFile("Tower/Bottle/ID1_0.PNG")) {
        bullet->autorelease();
        bullet->initOptions(target, speed, damage);
        return bullet;
    }
    CC_SAFE_DELETE(bullet);
    return nullptr;
}

void Bullet::initOptions(Monster* target, float speed, int damage) {
    this->target = target;
    this->speed = speed;
    this->damage = damage;
    this->moveToTarget();
}

void Bullet::moveToTarget() {
    if (!target) {
        this->removeFromParent();
        return;
    }

    // 计算移动到目标的时间
    float distance = this->getPosition().distance(target->getPosition());
    float duration = distance / speed;

    // 创建移动动作
    auto moveAction = MoveTo::create(duration, target->getPosition());
    auto removeSelf = RemoveSelf::create();
    auto damageCallback = CallFunc::create([this]() {
        if (target) {
            target->getAttacked(damage); // 对怪物造成伤害
        }
        });

    // 创建动作序列
    auto sequence = Sequence::create(moveAction, damageCallback, removeSelf, nullptr);
    this->runAction(sequence);
}

/*Refactored with Flyweight Pattern*/
void Bullet::setTarget(Monster* target) {
    this->target = target;
}

void Bullet::onHitMonster(Monster* monster) {
    if (monster) {
        monster->getAttacked(damage);
    }
}

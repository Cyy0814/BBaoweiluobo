// Bullet.cpp
#include "BottleBullet.h"

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

    // 更新子弹的飞行方向
    auto updateFunc = [this](float dt) {
        if (target && this->getParent()) {
            // 计算新的方向和位置
            Vec2 direction = target->getPosition() - this->getPosition();
            float distance = direction.length();
            if (distance < 10.0f) { // 当子弹接近目标
                target->getAttacked(damage); // 对怪物造成伤害
                this->stopAllActions();
                this->removeFromParent();
            }
            else {
                // 更新子弹的方向
                direction.normalize();
                this->setPosition(this->getPosition() + direction * speed * dt);
            }
        }
        else {
            this->stopAllActions();
            this->removeFromParent();
        }
    };

    // 每帧更新子弹位置
    this->schedule(updateFunc, "bullet_update");
}

void Bullet::destroy() {
    // 停止所有动作
    this->stopAllActions();

    // 从父节点移除
    if (this->getParent() != nullptr) {
        this->removeFromParent();
    }
}
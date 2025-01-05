#include "ShitBullet.h"

USING_NS_CC;

ShitBullet* ShitBullet::createWithTarget(Monster* target, const std::string& filename, float speed, int damage) {
    ShitBullet* shitbullet = new (std::nothrow) ShitBullet();
    if (shitbullet && shitbullet->initWithFile(filename)) {
        shitbullet->autorelease();
        shitbullet->initOptions(target, speed, damage);
        return shitbullet;
    }
    CC_SAFE_DELETE(shitbullet);
    return nullptr;
}

void ShitBullet::initOptions(Monster* target, float speed, int damage) {
    this->target = target;
    this->speed = speed;
    this->damage = damage;
    this->moveToTarget();
}

void ShitBullet::moveToTarget() {
    if (!target || target->getHitPoints() <= 0) {
        this->removeFromParent();
        return;
    }

    // 更新子弹的飞行方向
    auto updateFunc = [this](float dt) {
        // 计算新的方向和位置
        Vec2 direction = target->getPosition() - this->getPosition();
        float distance = direction.length();
        // 子弹旋转
        if (distance < 10.0f) { // 当子弹接近目标

            /*Refactored with State Pattern*/
            target->handleSlowEffect(damage);
            
            this->stopAllActions();
            this->removeFromParent();
        }
        // 更新子弹的方向
        direction.normalize();
        this->setPosition(this->getPosition() + direction * speed * dt);
    };

    // 每帧 updateFunc 更新位置
    this->schedule(updateFunc, "bullet_update");
}
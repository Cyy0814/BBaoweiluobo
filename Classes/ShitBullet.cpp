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

    // 创建定时器，不断更新子弹的飞行方向
    auto updateFunc = [this](float dt) {
        if (!this->getParent()) {
            this->unschedule("bullet_update");
            return;
        }
        // 计算新的方向和位置
        Vec2 direction = target->getPosition() - this->getPosition();
        float distance = direction.length();
        // 子弹旋转
        if (distance < 10.0f) { // 如果子弹接近目标
            // target->getAttacked(damage); // 对怪物造成伤害
            /****1/2更新 怪物受攻击特效*****************************/
            // target->beSloweddown();

            // Refactored with State Pattern
            target->handleSlowEffect(damage);
            /*******************************************************/
            this->stopAllActions();
            this->removeFromParent();
        }
        else if (target) {
            // 更新子弹的方向
            direction.normalize();
            this->setPosition(this->getPosition() + direction * speed * dt);
        }
    };

    // 每帧调用 updateFunc 来更新子弹位置
    this->schedule(updateFunc, "bullet_update");
}
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

    // ������ʱ�������ϸ����ӵ��ķ��з���
    auto updateFunc = [this](float dt) {
        if (target && this->getParent()) {
            // �����µķ����λ��
            Vec2 direction = target->getPosition() - this->getPosition();
            float distance = direction.length();
            if (distance < 10.0f) { // ����ӵ��ӽ�Ŀ��
                target->getAttacked(damage); // �Թ�������˺�
                this->stopAllActions();
                this->removeFromParent();
            }
            else {
                direction.normalize();
                this->setPosition(this->getPosition() + direction * speed * dt);
            }
        }
        else {
            this->stopAllActions();
            this->removeFromParent();
        }
    };

    // ÿ֡���� updateFunc �������ӵ�λ��
    this->schedule(updateFunc, "bullet_update");
}

void Bullet::destroy() {
    // ֹͣ���ж���
    this->stopAllActions();

    // �Ӹ��ڵ��Ƴ�
    if (this->getParent() != nullptr) {
        this->removeFromParent();
    }
}
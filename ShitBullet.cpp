// Bullet.cpp
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
    if (!target || target->dead || target->getHitPoints() <= 0) {
        this->removeFromParent();
        return;
    }

    // �����ƶ���Ŀ���ʱ��
    float distance = this->getPosition().distance(target->getPosition());
    float duration = distance / speed;


    // �����ƶ�����
    auto moveAction = MoveTo::create(duration, target->getPosition());
    auto checkTargetValid = CallFunc::create([this]() {
        if (!target || target->getHitPoints() <= 0) {
            this->stopAllActions();
            this->removeFromParent();
        }
        });
    auto removeSelf = RemoveSelf::create();
    auto damageCallback = CallFunc::create([this]() {
        if (target && target->getHitPoints() > 0) {
            target->takeHalfspeed();
            target->beSloweddown();
            target->getAttacked(damage);
        }
        });


    // ���ж�������
    auto sequence = Sequence::create(moveAction, checkTargetValid, damageCallback, removeSelf, nullptr);
    this->runAction(sequence);
}

void ShitBullet::targetDied() {
    target = nullptr; // ��Ŀ������Ϊ nullptr
    // ���߲�ȡ�����ʵ����ж�
}
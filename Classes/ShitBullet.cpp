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

    // ������ʱ�������ϸ����ӵ��ķ��з���
    auto updateFunc = [this](float dt) {
        if (!this->getParent()) {
            this->unschedule("bullet_update");
            return;
        }
        // �����µķ����λ��
        Vec2 direction = target->getPosition() - this->getPosition();
        float distance = direction.length();
        // �ӵ���ת
        if (distance < 10.0f) { // ����ӵ��ӽ�Ŀ��
            // target->getAttacked(damage); // �Թ�������˺�
            /****1/2���� �����ܹ�����Ч*****************************/
            // target->beSloweddown();

            // Refactored with State Pattern
            target->handleSlowEffect(damage);
            /*******************************************************/
            this->stopAllActions();
            this->removeFromParent();
        }
        else if (target) {
            // �����ӵ��ķ���
            direction.normalize();
            this->setPosition(this->getPosition() + direction * speed * dt);
        }
    };

    // ÿ֡���� updateFunc �������ӵ�λ��
    this->schedule(updateFunc, "bullet_update");
}
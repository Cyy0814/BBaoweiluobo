#include "FanBullet.h"

USING_NS_CC;

FanBullet* FanBullet::createWithTarget(Monster* target, const std::string& filename, float speed, int damage) {
    FanBullet* fanbullet = new (std::nothrow) FanBullet();
    if (fanbullet && fanbullet->initWithFile("Tower/Fan/ID4_6.PNG")) {
        fanbullet->autorelease();
        fanbullet->initOptions(target, speed, damage);
        return fanbullet;
    }
    CC_SAFE_DELETE(fanbullet);
    return nullptr;
}

void FanBullet::initOptions(Monster* target, float speed, int damage) {
    this->target = target;
    this->speed = speed;
    this->damage = damage;
    this->moveToTarget();
}

void FanBullet::moveToTarget() {
    if (!target) {
        this->removeFromParent();
        return;
    }

    // 更新子弹的飞行方向和位置
    // 计算新的方向和位置
    // 子弹旋转
    // 每帧旋转5度，可以调整旋转速度
    // 当子弹接近目标
    // 对怪物造成伤害
    // 更新子弹的方向
    // 每帧更新子弹位置
    auto updateFunc = [this](float dt) {
        if (!this->getParent()) {
            this->unschedule("bullet_update");
            return;
        }
        // µķλ
        Vec2 direction = target->getPosition() - this->getPosition();
        float distance = direction.length();
        // �ӵ���ת
        this->setRotation(this->getRotation() + 5.0f); // ÿ֡��ת5�ȣ����Ե�����ת�ٶ�
        if (distance < 10.0f) { // ����ӵ��ӽ�Ŀ��
            target->getAttacked(damage); // �Թ�������˺�
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

//void Bullet::targetDied() {
//    target = nullptr; // ��Ŀ������Ϊ nullptr
//    // ���߲�ȡ�����ʵ����ж�
//}
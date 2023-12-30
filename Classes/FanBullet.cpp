// Bullet.cpp
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

    float distance = this->getPosition().getDistance(target->getPosition());
    

    float duration = distance / speed; // �����ƶ������ʱ��

    auto moveAction = MoveTo::create(duration, target->getPosition()); // ����ֱ���ƶ�����
    auto rotateAction = RotateBy::create(duration, 360.0f); // ��תһ�ܣ�ʱ��ͽǶȿ��Ը���ʵ���������

    auto combinedAction = Spawn::create(moveAction, rotateAction, nullptr);

    auto sequence = Sequence::create(combinedAction, CallFunc::create([this]() {
        this->removeFromParent();
        }), nullptr);

    this->runAction(sequence);

    
}

//void Bullet::targetDied() {
//    target = nullptr; // ��Ŀ������Ϊ nullptr
//    // ���߲�ȡ�����ʵ����ж�
//}
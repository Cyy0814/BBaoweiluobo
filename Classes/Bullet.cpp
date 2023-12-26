#include "BottleBullet.h"

Bullet::Bullet() : damage(0)
{
    // ��ʼ������
}

//Bullet::~Bullet()
//{
//    // �������
//}

Bullet* Bullet::create(const cocos2d::Vec2& position, const cocos2d::Vec2& direction)
{
    Bullet* bullet = new (std::nothrow) Bullet();
    
    if (bullet && bullet->initWithFile("Resources/Tower/Bottle/ID1_2.PNG"))
    {
        bullet->autorelease();
        bullet->setPosition(position);
        bullet->direction = direction;
        // ������Ҫ���н�һ���ĳ�ʼ��
        auto scene = cocos2d::Director::getInstance()->getRunningScene();
        scene->addChild(bullet);
        return bullet;
    }
    CC_SAFE_DELETE(bullet);
    return nullptr;
}

bool Bullet::init()
{
    if (!cocos2d::Sprite::init())
    {
        return false;
    }

    // �����ӵ���������г�ʼ������

    return true;
}

void Bullet::update(float delta)
{
    // �����ӵ����ٶȺͷ�������ӵ���λ��
    float speed = 100.0f; // ����ʵ����������ٶ�
    cocos2d::Vec2 newPosition = getPosition() + (direction * speed * delta);
    setPosition(newPosition);

    // ����ӵ���������ײ������ʵ����Ϸ�߼�����
    // ...

    // ����ӵ��Ƿ񳬳���Ϸ��������ǣ��������ӵ�����
    cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    if (newPosition.x < 0 || newPosition.x > visibleSize.width || newPosition.y < 0 || newPosition.y > visibleSize.height)
    {
        removeFromParentAndCleanup(true);
    }
}

void Bullet::setDamage(int damage)
{
    this->damage = damage;
}
#include "BottleBullet.h"

BottleBullet::BottleBullet() : damage(0),speed(10)
{
    // ��ʼ������
}

//BottleBullet::~Bullet()
//{
//    // �������
//}

BottleBullet* BottleBullet::create()
{
    //bottle&& bottle->levelToImagePathMap.count(bottle->level) > 0 && bottle->initWithFile())
    BottleBullet* bottlebullet = new (std::nothrow) BottleBullet();
    if (bottlebullet && bottlebullet->levelToImagePathMap.count(bottlebullet->level) > 0&& bottlebullet->initWithFile(bottlebullet->levelToImagePathMap.at(bottlebullet->level)))
    {
        bottlebullet->autorelease();
        
        
        // ������Ҫ���н�һ���ĳ�ʼ��
        return bottlebullet;
    }
    CC_SAFE_DELETE(bottlebullet);
    return nullptr;
}

bool BottleBullet::init()
{
    if (!cocos2d::Sprite::init())
    {
        return false;
    }
    return true;
}

void BottleBullet::update(float delta)
{
    // �����ӵ����ٶȺͷ�������ӵ���λ��
    // ����ʵ����������ٶ�
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

void BottleBullet::setDamage(int damage)
{
    this->damage = damage;
}
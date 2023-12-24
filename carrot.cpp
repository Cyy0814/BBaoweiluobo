#include "Carrot.h"

USING_NS_CC;

/*
auto carrotLayer = Layer::create();
this->addChild(carrotLayer);

auto aCarrot = Sprite::create("myCarrot/lovelyCarrot.png");
aCarrot->setPosition(Vec2(837, 450));
carrotLayer->addChild(aCarrot);

auto rotateAction = RepeatForever::create(
    Sequence::create(
        Spawn::create(
            RotateBy::create(1.5f, 360.0f),  // ������תһ��
            Blink::create(0.5f, 3),  // ��˸����
            nullptr
        ),
        DelayTime::create(6.0f),  // �ӳ�9�룬���־�ֹ״̬
        nullptr
    )
);

aCarrot->runAction(rotateAction);
*/


Carrot* Carrot::createCarrot()
{
    Carrot* pRet = new Carrot();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool Carrot::init()
{
    if (!Sprite::initWithFile("myCarrot/lovelyCarrot.png"))
    {
        return false;
    }

    // ��ʼ��Ѫ��Ϊ10
    m_hp = 10;

    return true;
}

void Carrot::decreaseHp(int value)
{
    m_hp -= value;
    if (m_hp <= 0)
    {
        // Ѫ��Ϊ0ʱ�Ĵ����߼�
        
        // ...
    }
}
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "cocos/ui/UIImageView.h"
#include "TowerBottle.h"
#include "BottleBullet.h"
#include "Monster.h"
USING_NS_CC;

using namespace cocos2d::ui;
using namespace cocos2d;

const int _tower_bottle = 1;


Bottle::Bottle() : level(1), attackPower(10), attackSpeed(1.0f), towerType(0),attackRange(100)
{
    // ��ʼ������
}



Bottle* Bottle::create()
{
    Bottle* bottle = new (std::nothrow) Bottle();

    
        if (bottle && (bottle->initWithFile("Tower/Bottle/ID1_22.PNG")))
        {
            bottle->autorelease();


            return bottle;
        }

    CC_SAFE_DELETE(bottle);
    return nullptr;
}


void Bottle::attack(cocos2d::Sprite* target)
{
    BottleBullet* bottlebullet = BottleBullet::create();
    if (bottlebullet) {
        // �����ӵ�����ʼλ��Ϊ����λ��
        bottlebullet->setPosition(this->getPosition());
      
        // ���ӵ���ӵ�������
        auto scene = cocos2d::Director::getInstance()->getRunningScene();
        scene->addChild(bottlebullet);
    }
    
}

void Bottle::upgrade()
{
    // ������̨���������
    level++;
    attackPower += 5; // ʾ����ÿ����������5�㹥����
}

//void Bottle::remove()
//{
//    // �Ƴ���̨
//    this->removeFromParent();
//}


void Bottle::setTowerImage(const std::string& imagePath) {
    auto texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(imagePath);
    setTexture(texture);
}

void Bottle::update(float deltaTime, std::vector<Monster*>& monsterList)
{
    // ��⹥����Χ���Ƿ��й���
    Monster* target = findTargetInAttackRange(monsterList);

    if (target)
    {
        // �Զ���׼����
        rotateTowerTowardsTarget(target);

        //Ӧ��������ȴʱ��
        attack(target);
    }
}

Monster* Bottle::findTargetInAttackRange(std::vector<Monster*> monsterList)
{
    // ���������б�����λ�ڹ�����Χ�ڵĹ���
    for (Monster* monster : monsterList)
    {
        float distance = (getPosition().distance(monster->getPosition()));

        if (distance <= attackRange)
        {
            return monster;
        }
    }

    return nullptr;
}

void Bottle::rotateTowerTowardsTarget(Monster* target)
{
    // ��������Ŀ�����֮��ĽǶ�
    Vec2 direction = target->getPosition() - getPosition();
    float angle = atan2(direction.y, direction.x);

    // ���Ƕ�Ӧ����������ת
    setRotation(-CC_RADIANS_TO_DEGREES(angle));
}
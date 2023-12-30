#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "cocos/ui/UIImageView.h"
#include "TowerBottle.h"
#include "Bullet.h"
USING_NS_CC;

using namespace cocos2d::ui;
using namespace cocos2d;

const int _tower_bottle = 1;


Bottle::Bottle() : level(1), attackPower(10), attackSpeed(1.0f), towerType(0)
{
    // ��ʼ������
}



Bottle* Bottle::create(Vec2 position)
{
    Bottle* bottle = new (std::nothrow) Bottle();
    if (bottle && bottle->levelToImagePathMap.count(bottle->level) > 0&& bottle->initWithFile(bottle->levelToImagePathMap.at(bottle->level)))
    {
        bottle->autorelease();
        //bottle->towerType = towerType;
        // �������ͳ�ʼ����ͬ������
        bottle->setPosition(position);
        auto scene = cocos2d::Director::getInstance()->getRunningScene();
        scene->addChild(bottle);
        return bottle;
    }
    
    CC_SAFE_DELETE(bottle);
    return nullptr;
}

//bool Bottle::init()
//{
  //  if (!Sprite::init())
    //{
      //  return false;
    //}
    // ��ʼ����̨
    //return true;
//}
void Bottle::attack()
{
   // Bullet* bullet = Bullet::create();
    //if (bullet) {
        // �����ӵ�����ʼλ��Ϊ����λ��
     //    bullet->setPosition(this->getPosition());

     //    // ������׼Ŀ��ķ�������
     //    cocos2d::Vec2 direction = target->getPosition() - this->getPosition();
     //    direction.normalize();

     //    // �����ӵ����ٶ�����
     //    float bulletSpeed = 500.0f;  // �ӵ��ٶ�
     //    cocos2d::Vec2 velocity = direction * bulletSpeed;

     //    // �����ӵ����ٶ�
     //    bullet->setVelocity(velocity);

     //    // ���ӵ���ӵ�������
     //    auto scene = cocos2d::Director::getInstance()->getRunningScene();
     //    scene->addChild(bullet);
     //}
     // ʵ�ֹ����߼�
     // ������漰������һ�� Projectile �࣬�����䷢�䵽Ŀ��
     // ������ܲ�Ӧ��ֱ��new bullet����ΪbulletӦ���Ƿ�����Ļ��࣬����ÿһ��tower��Ӧ���ж�Ӧ�ķ���������
    //}
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
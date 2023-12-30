// Bottle.cpp
#include "TowerBottle.h"
#include"Monster.h"

USING_NS_CC;

//Bottle::Bottle()
//{
//    /*attackRange = 20;
//    attackSpeed = 1;
//    timeSinceLastAttack = 0;*/
//
//
//}

Bottle* Bottle::create(const Vec2& position) {
    Bottle* bottle = new (std::nothrow) Bottle();
    if (bottle && bottle->initWithFile("Resources/Tower/Bottle/ID1_22.PNG")) { // ʹ����������ͼƬ
        bottle->autorelease();
        bottle->setPosition(position);
        bottle->attackRange = 100.0f; // ���ù�����Χ
        bottle->attackSpeed = 1.0f; // ���ù����ٶ�
        bottle->timeSinceLastAttack = 0;
        return bottle;
    }
    CC_SAFE_DELETE(bottle);
    return nullptr;
}

void Bottle::update(float dt, std::vector<Monster*> monsters) {
    timeSinceLastAttack += dt;
    if (timeSinceLastAttack >= attackSpeed) {
        checkForMonstersInRange(monsters);
        if (!monstersInRange.empty()) {
            attack(monstersInRange.front()); // ������һ������
            timeSinceLastAttack = 0;
        }
    }
}

bool Bottle::isMonsterInRange(Monster* monster) {
    return (getPosition().distance(monster->getPosition()) <= attackRange);
}

void Bottle::checkForMonstersInRange(std::vector<Monster*> monsters) {
    // ���� monsters �ǳ��������й�����б�
    monstersInRange.clear();
    if (monsters.size())
    {
        for (Monster* monster : monsters) {
            if (isMonsterInRange(monster)) {
                monstersInRange.push_back(monster);
            }
        }
    }
}

void Bottle::attack(Monster* target) {
    auto bullet = Bullet::createWithTarget(target, "Resource/Tower/Bottle/ID1_0.PNG", 1000.0f, 10);
    bullet->setPosition(getPosition());
    this->getParent()->addChild(bullet);
}

// shit.cpp
#include "Shit.h"
#include "ShitBullet.h"
#include"Monster.h"

USING_NS_CC;

Shit* Shit::create(const Vec2& position) {
    Shit* shit = new (std::nothrow) Shit();
    if (shit && shit->initWithFile("Tower/Shit/ID2_5.PNG")) { // ʹ����������ͼƬ
        shit->autorelease();
        shit->setPosition(position);
        shit->attackRange = 1000.0f; // ���ù�����Χ
        shit->attackSpeed = 1.0f; // ���ù����ٶ�
        shit->timeSinceLastAttack = 0;
        return shit;
    }
    CC_SAFE_DELETE(shit);
    return nullptr;
}

void Shit::update(float dt, std::vector<Monster*> monsters) {
    timeSinceLastAttack += dt;
    if (timeSinceLastAttack >= attackSpeed) {
        checkForMonstersInRange(monsters);
        if (!monstersInRange.empty()) {
            attack(monstersInRange.front()); // ������һ������
            timeSinceLastAttack = 0;
        }
    }
}

bool Shit::isMonsterInRange(Monster* monster) {
    return (getPosition().distance(monster->getPosition()) <= attackRange);
}

void Shit::checkForMonstersInRange(std::vector<Monster*> monsters) {
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

void Shit::attack(Monster* target) {
    auto shitbullet = ShitBullet::createWithTarget(target, "Tower/Shit/ID2_43.PNG", 1000.0f, 10);
    shitbullet->setPosition(getPosition());
    this->getParent()->addChild(shitbullet);
}


void Shit::upgrade()
{
    //���Եı仯



}
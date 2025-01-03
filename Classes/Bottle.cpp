#include "Bottle.h"
#include "BottleBullet.h"
#include "Monster.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Global.h"
using namespace cocos2d;
using namespace cocos2d::ui;

USING_NS_CC;

Bottle* Bottle::create(const Vec2& position) {
    Bottle* bottle = new (std::nothrow) Bottle();
    if (bottle && bottle->init()) {
        //1/2 ���Խ�base�Ƴɳ�Ա������ϣ���ﵽ��upgradeʱ�����ظ�����
        bottle->base = Sprite::create("Tower/Bottle/ID1_11.PNG");
        bottle->base->setPosition(bottle->getPosition().x + 15, bottle->getPosition().y + 25);
        
        bottle->addChild(bottle->base, -1);

        bottle->setTexture("Tower/Bottle/ID1_22.PNG"); // ��ʼ���
        bottle->autorelease();
        bottle->setPosition(position);

        bottle->attackDamage = 100;    // ���ù����˺�
        bottle->attackRange = 200.0f;  // ���ù�����Χ
        bottle->attackSpeed = 1000.0f; // ���ù����ٶ�
        bottle->timeSinceLastAttack = 0;
        bottles.push_back(bottle);
        return bottle;
    }
    CC_SAFE_DELETE(bottle);
    return nullptr;
}

void Bottle::upgrade()
{
    if (level < 3 && goldCoin->m_value > 70) {
        level++;

        // �������
        std::string textureName = "Tower/Bottle/Level" + std::to_string(level) + ".PNG";
        setTexture(textureName);
       // this->base->setPosition(this->getPosition().x + 15, this->getPosition().y + 25);
        // ս������
        attackSpeed += 400;
        attackDamage += 50;
        attackRange += 100;

        // ��Ǯ
        goldCoin->earnGold(-70);
    }
}

void Bottle::remove()
{
    if (level == 1)
        goldCoin->earnGold(80);
    else if(level==2)
        goldCoin->earnGold(120);
    else
        goldCoin->earnGold(180);

    // ɾ����Ч
    auto Delete = cocos2d::Sprite::create("Tower/Tower_Delete.PNG");
    Delete->setPosition(this->getPosition());
    this->getParent()->addChild(Delete);

    // ����һ���������Ƴ�ɾ����Ч
    auto fadeOut = cocos2d::FadeOut::create(0.5f); // ����ʱ����Ը�����Ҫ����
    auto removeExplosion = cocos2d::RemoveSelf::create();
    auto sequence = cocos2d::Sequence::create(fadeOut, removeExplosion, nullptr);
    Delete->runAction(sequence);
    /****1/2���� ָ����������*****************************/
    for (auto iter = bottles.begin(); iter != bottles.end();)
    {
        if (this == *iter)
        {
            iter = bottles.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    /****************************************/
    this->hideAttackRangeAndButtons();
    this->removeAllChildren();
    this->removeFromParentAndCleanup(true);
}

void Bottle::update(float dt, std::vector<Monster*> monsters) {
    timeSinceLastAttack += dt;
    if (timeSinceLastAttack >= 1) {
        checkForMonstersInRange(monsters);
        if (!monstersInRange.empty()) {
            cocos2d::Vec2 towerPosition = getPosition();
            cocos2d::Vec2 targetPosition = monstersInRange[0]->getPosition();

            // ������ָ��Ŀ�������
            cocos2d::Vec2 direction = targetPosition - towerPosition;
            direction.normalize();  // ��������һ��Ϊ��λ����

            // ������ת�Ƕ�
            float angle = CC_RADIANS_TO_DEGREES(atan2(direction.y, direction.x)) - 90;
            auto rotateAction = cocos2d::RotateBy::create(0.01f, angle);
            this->runAction(rotateAction);


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

//----------------------------refactored with flyweight----------------------------//
void Bottle::attack(Monster* target) {
    // 使用享元工厂获取子弹
    Flyweight* bullet = BulletFlyweightFactory::getBullet("Tower/Bottle/ID1_0.PNG", attackSpeed, attackDamage);
    
    // 设置目标并添加到场景
    Bullet* concreteBullet = dynamic_cast<Bullet*>(bullet);
    if (concreteBullet) {
        concreteBullet->setTarget(target);
        concreteBullet->setPosition(getPosition());
        this->getParent()->addChild(concreteBullet);
    }
}
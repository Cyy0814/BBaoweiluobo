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
        //1/2 尝试将base制成成员变量，希望达到在upgrade时不会重复出现
        bottle->base = Sprite::create("Tower/Bottle/ID1_11.PNG");
        bottle->base->setPosition(bottle->getPosition().x + 15, bottle->getPosition().y + 25);
        
        bottle->addChild(bottle->base, -1);

        bottle->setTexture("Tower/Bottle/ID1_22.PNG"); // 初始外观
        bottle->autorelease();
        bottle->setPosition(position);

        bottle->attackDamage = 100;    // 设置攻击伤害
        bottle->attackRange = 200.0f;  // 设置攻击范围
        bottle->attackSpeed = 1000.0f; // 设置攻击速度
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

        // 更新外观
        std::string textureName = "Tower/Bottle/Level" + std::to_string(level) + ".PNG";
        setTexture(textureName);
       // this->base->setPosition(this->getPosition().x + 15, this->getPosition().y + 25);
        // 战力提升
        attackSpeed += 400;
        attackDamage += 50;
        attackRange += 100;

        // 扣钱
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

    // 删除特效
    auto Delete = cocos2d::Sprite::create("Tower/Tower_Delete.PNG");
    Delete->setPosition(this->getPosition());
    this->getParent()->addChild(Delete);

    // 设置一个动作来移除删除特效
    auto fadeOut = cocos2d::FadeOut::create(0.5f); // 持续时间可以根据需要调整
    auto removeExplosion = cocos2d::RemoveSelf::create();
    auto sequence = cocos2d::Sequence::create(fadeOut, removeExplosion, nullptr);
    Delete->runAction(sequence);
    /****1/2更新 指针向量置零*****************************/
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

            // 计算塔指向目标的向量
            cocos2d::Vec2 direction = targetPosition - towerPosition;
            direction.normalize();  // 将向量归一化为单位向量

            // 计算旋转角度
            float angle = CC_RADIANS_TO_DEGREES(atan2(direction.y, direction.x)) - 90;
            auto rotateAction = cocos2d::RotateBy::create(0.01f, angle);
            this->runAction(rotateAction);


            attack(monstersInRange.front()); // 攻击第一个怪物
            timeSinceLastAttack = 0;
        }
    }
}

void Bottle::attack(Monster* target) {
    bullet = Bullet::createWithTarget(target, "Tower/Bottle/ID1_0.PNG", attackSpeed, attackDamage);
    bullet->setPosition(getPosition());
    this->getParent()->addChild(bullet);
}
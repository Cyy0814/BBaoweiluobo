#include "Bottle.h"
#include "BottleBullet.h"
#include "Monster.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Global.h"
#include "IAttackStrategy.h"
using namespace cocos2d;
using namespace cocos2d::ui;

USING_NS_CC;

Bottle* Bottle::create(const Vec2& position) {
    Bottle* bottle = new (std::nothrow) Bottle();
    if (bottle && bottle->init()) {
        //1/2 可以将base改成成员变量，这样就达到在upgrade时不重复创建
        bottle->base = Sprite::create("Tower/Bottle/ID1_11.PNG");
        bottle->base->setPosition(bottle->getPosition().x + 15, bottle->getPosition().y + 25);
        
        bottle->addChild(bottle->base, -1);

        bottle->setTexture("Tower/Bottle/ID1_22.PNG"); // 初始化塔
        bottle->autorelease();
        bottle->setPosition(position);

        bottle->attackDamage = 100;    // 设置攻击伤害
        bottle->attackRange = 200.0f;  // 设置攻击范围
        bottle->attackSpeed = 1000.0f; // 设置攻击速度
        bottle->timeSinceLastAttack = 0; // 设置上次攻击时间
        bottles.push_back(bottle);

        //---------------------------------refactored with dependency injection pattern---------------------------------//
        // 注入攻击策略
        bottle->setAttackStrategy(new BottleAttackStrategy());
        
        return bottle;
    }
    CC_SAFE_DELETE(bottle);
    return nullptr;
}

void Bottle::upgrade()
{
    if (level < 3 && goldCoin->m_value > 70) {
        level++;

        // 更新纹理
        std::string textureName = "Tower/Bottle/Level" + std::to_string(level) + ".PNG";
        setTexture(textureName);
       // this->base->setPosition(this->getPosition().x + 15, this->getPosition().y + 25);
        // 战斗属性
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

    // 动画时间可以根据需要调整
    auto fadeOut = cocos2d::FadeOut::create(0.5f);
    auto removeExplosion = cocos2d::RemoveSelf::create();
    auto sequence = cocos2d::Sequence::create(fadeOut, removeExplosion, nullptr);
    Delete->runAction(sequence);
    /**** 1/2清除 指针数组内容 *****************************/
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

            // 方向
            cocos2d::Vec2 direction = targetPosition - towerPosition;
            direction.normalize();  // 归一化

            // 旋转角度
            float angle = CC_RADIANS_TO_DEGREES(atan2(direction.y, direction.x)) - 90;
            auto rotateAction = cocos2d::RotateBy::create(0.01f, angle);
            this->runAction(rotateAction);


            attack(monstersInRange.front()); // 攻击第一个
            timeSinceLastAttack = 0;
        }
    }
}

//---------------------------------refactored with strategy pattern---------------------------------//
void Bottle::attack(Monster* target) {
    if(attackStrategy) {
        attackStrategy->attack(getPosition(), target, attackDamage);
    }
}
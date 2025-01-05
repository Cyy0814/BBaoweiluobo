#include "Fan.h"
#include "FanBullet.h"
#include "Monster.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "IAttackStrategy.h"
#include "Global.h"
USING_NS_CC;


Fan* Fan::create(const Vec2& position) {
    Fan* fan = new (std::nothrow) Fan();
    if (fan && fan->init()) { // 使用正确的图片
        auto base = Sprite::create("Tower/Fan/ID4_0.PNG");
        base->setPosition(fan->getPosition().x + 20, fan->getPosition().y + 10);
        fan->addChild(base, -1);

        fan->setTexture("Tower/Fan/Level1.PNG"); // 初始化塔
        fan->autorelease();
        fan->setPosition(position.x + 5, position.y + 10);

        fan->attackDamage = 100;    // 设置攻击伤害
        fan->attackRange = 200.0f;  // 设置攻击范围
        fan->attackSpeed = 1000.0f; // 设置攻击速度
        fan->timeSinceLastAttack = 0;
        fans.push_back(fan);

        /*Refactored with Dependency Injection Pattern*/
        // 注入攻击策略
        fan->setAttackStrategy(new FanAttackStrategy());

        return fan;
    }
    CC_SAFE_DELETE(fan);
    return nullptr;
}
void Fan::upgrade()
{
    if (level < 3 && goldCoin->m_value > 120) {
        level++;

        // 更新纹理
        std::string textureName = "Tower/Fan/Level" + std::to_string(level) + ".PNG";
        setTexture(textureName);

        // 战斗属性
        attackSpeed += 400;
        attackDamage += 150;
        attackRange += 100;

        // 扣钱
        goldCoin->earnGold(-120);
    }
}

void Fan::remove()
{
    if (level == 1)
        goldCoin->earnGold(80);
    else if (level == 2)
        goldCoin->earnGold(120);
    else
        goldCoin->earnGold(180);

    // 删除特效
    auto Delete = cocos2d::Sprite::create("Tower/Tower_Delete.PNG");
    Delete->setPosition(this->getPosition());
    this->getParent()->addChild(Delete);

    // 动画时间可以根据需要调整
    auto fadeOut = cocos2d::FadeOut::create(0.5f); // 动画时间可以根据需要调整
    auto removeExplosion = cocos2d::RemoveSelf::create();
    auto sequence = cocos2d::Sequence::create(fadeOut, removeExplosion, nullptr);
    Delete->runAction(sequence);
    /**** 1/2清除 指针数组内容 *****************************/
    for (auto iter = fans.begin(); iter != fans.end();)
    {
        if (this == *iter)
        {
            iter = fans.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    this->hideAttackRangeAndButtons();
    this->removeAllChildren();
    this->removeFromParentAndCleanup(true);
}

/*Refactored with Strategy Pattern*/
void Fan::attack(Monster* target) {
    if(attackStrategy) {
        attackStrategy->attack(getPosition(), target, attackDamage);
    }
}
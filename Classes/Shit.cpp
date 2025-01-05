#include "Shit.h"
#include "ShitBullet.h"
#include "Monster.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Global.h"
#include "IAttackStrategy.h"

USING_NS_CC;

Shit* Shit::create(const Vec2& position) {
    Shit* shit = new (std::nothrow) Shit();
    if (shit && shit->initWithFile("Tower/Shit/Level1.PNG")) {
        shit->autorelease();
        shit->setPosition(position);

        // 设置攻击伤害
        shit->attackDamage = 50;     
        // 设置攻击范围
        shit->attackRange = 200.0f;  
        // 设置攻击速度
        shit->attackSpeed = 1000.0f; 

        shit->timeSinceLastAttack = 0;
        shits.push_back(shit);

        
        // 注入攻击策略
        shit->setAttackStrategy(new ShitAttackStrategy());

        return shit;
    }
    CC_SAFE_DELETE(shit);
    return nullptr;
}

void Shit::upgrade()
{
    if (level < 3 && goldCoin->m_value > 120) {
        level++;

        // 更新纹理
        std::string textureName = "Tower/Shit/Level" + std::to_string(level) + ".PNG";
        setTexture(textureName);

        // 战斗属性
        attackSpeed += 400;
        attackDamage += 50;
        attackRange += 100;

        // 扣钱
        goldCoin->earnGold(-120);
    }
}

void Shit::remove()
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

    // 删除特效
    auto fadeOut = cocos2d::FadeOut::create(0.5f); // 删除时间，需要根据实际情况调整
    auto removeExplosion = cocos2d::RemoveSelf::create();
    auto sequence = cocos2d::Sequence::create(fadeOut, removeExplosion, nullptr);
    Delete->runAction(sequence);
    /**** 1/2清除 指针数组内容 *****************************/
    for (auto iter = shits.begin(); iter != shits.end();)
    {
        if (this == *iter)
        {
            iter = shits.erase(iter);
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
void Shit::attack(Monster* target) {
    if(attackStrategy) {
        attackStrategy->attack(getPosition(), target, attackDamage);
    }
}
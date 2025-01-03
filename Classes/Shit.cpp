#include "Shit.h"
#include "ShitBullet.h"
#include "Monster.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Global.h"

USING_NS_CC;

Shit* Shit::create(const Vec2& position) {
    Shit* shit = new (std::nothrow) Shit();
    if (shit && shit->initWithFile("Tower/Shit/Level1.PNG")) {
        shit->autorelease();
        shit->setPosition(position);

        shit->attackDamage = 50;     // ���ù����˺�
        shit->attackRange = 200.0f;  // ���ù�����Χ
        shit->attackSpeed = 1000.0f; // ���ù����ٶ�
        shit->timeSinceLastAttack = 0;
        shits.push_back(shit);
        return shit;
    }
    CC_SAFE_DELETE(shit);
    return nullptr;
}

void Shit::upgrade()
{
    if (level < 3 && goldCoin->m_value > 120) {
        level++;

        // �������
        std::string textureName = "Tower/Shit/Level" + std::to_string(level) + ".PNG";
        setTexture(textureName);

        // ս������
        attackSpeed += 400;
        attackDamage += 50;
        attackRange += 100;

        // ��Ǯ
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

void Shit::update(float dt, std::vector<Monster*> monsters) {
    timeSinceLastAttack += dt;
    if (timeSinceLastAttack >= 1) {
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

//----------------------------refactored with flyweight----------------------------//
void Shit::attack(Monster* target) {
    // 使用享元工厂获取子弹
    Flyweight* bullet = BulletFlyweightFactory::getBullet("Tower/Shit/ID2_43.PNG", attackSpeed, attackDamage);
    
    // 设置目标并添加到场景
    Bullet* concreteBullet = dynamic_cast<Bullet*>(bullet);
    if (concreteBullet) {
        concreteBullet->setTarget(target);
        concreteBullet->setPosition(getPosition());
        this->getParent()->addChild(concreteBullet);
    }
}
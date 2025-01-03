#include "Fan.h"
#include "FanBullet.h"
#include "Monster.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Global.h"
USING_NS_CC;


Fan* Fan::create(const Vec2& position) {
    Fan* fan = new (std::nothrow) Fan();
    if (fan && fan->init()) { // ʹ����������ͼƬ
        auto base = Sprite::create("Tower/Fan/ID4_0.PNG");
        base->setPosition(fan->getPosition().x + 20, fan->getPosition().y + 10);
        fan->addChild(base, -1);

        fan->setTexture("Tower/Fan/Level1.PNG"); // ��ʼ���
        fan->autorelease();
        fan->setPosition(position.x + 5, position.y + 10);

        fan->attackDamage = 100;    // ���ù����˺�
        fan->attackRange = 200.0f;  // ���ù�����Χ
        fan->attackSpeed = 1000.0f; // ���ù����ٶ�
        fan->timeSinceLastAttack = 0;
        fans.push_back(fan);

        return fan;
    }
    CC_SAFE_DELETE(fan);
    return nullptr;
}
void Fan::upgrade()
{
    if (level < 3 && goldCoin->m_value > 120) {
        level++;

        // �������
        std::string textureName = "Tower/Fan/Level" + std::to_string(level) + ".PNG";
        setTexture(textureName);

        // ս������
        attackSpeed += 400;
        attackDamage += 150;
        attackRange += 100;

        // ��Ǯ
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


void Fan::update(float dt, std::vector<Monster*> monsters) {
    timeSinceLastAttack += dt;
    if (timeSinceLastAttack >= 1) {
        checkForMonstersInRange(monsters);
        if (!monstersInRange.empty()) {
            attack(monstersInRange.front()); // ������һ������
            timeSinceLastAttack = 0;
        }
    }
}

bool Fan::isMonsterInRange(Monster* monster) {
    return (getPosition().distance(monster->getPosition()) <= attackRange);
}

void Fan::checkForMonstersInRange(std::vector<Monster*> monsters) {
    // ���� monsters �ǳ��������й�����б�
    monstersInRange.clear();
    if (monsters.size())
    {
        for (Monster*& monster : monsters) {
            if (isMonsterInRange(monster)) {
                monstersInRange.push_back(monster);
            }
        }
    }
}

//----------------------------refactored with flyweight----------------------------//
void Fan::attack(Monster* target) {
    // 使用享元工厂获取子弹
    Flyweight* bullet = BulletFlyweightFactory::getBullet("Tower/Fan/ID1_0.PNG", attackSpeed, attackDamage);
    
    // 设置目标并添加到场景
    Bullet* concreteBullet = dynamic_cast<Bullet*>(bullet);
    if (concreteBullet) {
        concreteBullet->setTarget(target);
        concreteBullet->setPosition(getPosition());
        this->getParent()->addChild(concreteBullet);
    }
}
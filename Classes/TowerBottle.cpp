// Bottle.cpp
#include "TowerBottle.h"
#include "Bullet.h"
#include "Monster.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Global.h"
using namespace cocos2d;
using namespace cocos2d::ui;

USING_NS_CC;

Bottle* Bottle::create(const Vec2& position) {
    Bottle* bottle = new (std::nothrow) Bottle();
    if (bottle && bottle->initWithFile("Tower/Bottle/ID1_22.PNG")) { // ʹ����������ͼƬ
        bottle->setTexture("Tower/Bottle/ID1_22.PNG"); // ��ʼ���
        bottle->autorelease();
        bottle->setPosition(position);
        bottle->attackRange = 1000.0f; // ���ù�����Χ
        bottle->attackSpeed = 1.0f; // ���ù����ٶ�
        bottle->timeSinceLastAttack = 0;
        bottles.push_back(bottle);
        return bottle;
    }
    CC_SAFE_DELETE(bottle);
    return nullptr;
}

void Bottle::showAttackRange()  {
    attackRange_ = Sprite::create("GameScene/Tower/AttackRange.PNG");
    attackRange_->setPosition(Vec2(10, 30));
    this->addChild(attackRange_);

}
void Bottle::showUpgradeAndRemoveButtons()  {
    // ����������ť
    upgradeButton = cocos2d::ui::Button::create("GameScene/Tower/Btn_CanUpLevel.png");
    upgradeButton->setPosition(Vec2(10, 110)); // ����λ��
    upgradeButton->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            this->upgrade();
        }
        });
    this->addChild(upgradeButton);

    // �����Ƴ���ť
    removeButton = cocos2d::ui::Button::create("GameScene/Tower/Btn_SellTower.png");
    removeButton->setPosition(Vec2(10, -50)); // ����λ��
    removeButton->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            this->remove();
        }
        });
    this->addChild(removeButton);

    // ����һ��ȫ�ִ���������
    auto globalListener = EventListenerTouchOneByOne::create();
    globalListener->setSwallowTouches(false); // �����ɴ����¼�
    globalListener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 touchLocation = touch->getLocation();

        // ��ⴥ�����Ƿ�����̨��������ť���Ƴ���ť��
        if (!this->getBoundingBox().containsPoint(touchLocation) &&
            !this->upgradeButton->getBoundingBox().containsPoint(touchLocation) &&
            !this->removeButton->getBoundingBox().containsPoint(touchLocation)) {
            // �������̨�Ͱ�ť֮�⣬���ع�����Χ�Ͱ�ť
            this->hideAttackRangeAndButtons();
            return true;
        }
        return false; // �����¼���������
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(globalListener, this);
}

void Bottle::hideAttackRangeAndButtons() {
    // ���ع�����Χָʾ
    if (attackRange) attackRange_->setVisible(false);

    // �����������Ƴ���ť
    if (upgradeButton) upgradeButton->setVisible(false);
    if (removeButton) removeButton->setVisible(false);
}

void Bottle::upgrade()
{
    // ���ӹ���������Χ�ȣ�������Ϸ������
    if (level < 3) {
        level++;
        // ������������ۺ�����
    }
    std::string textureName = "Tower/Bottle/Level" + std::to_string(level) + ".PNG";
    setTexture(textureName);

    // ��߹�������������Χ�򹥻��ٶ�
     // ʾ�����ӹ�����
    // �������Ӹ����ս���������߼�

}


void Bottle::remove()
{
    if (level == 1)
        goldCoin->earnGold(80);
    else if(level==2)
        goldCoin->earnGold(120);
    else
        goldCoin->earnGold(180);
    this->removeFromParentAndCleanup(true);
}

void Bottle::update(float dt, std::vector<Monster*> monsters) {
    timeSinceLastAttack += dt;
    if (timeSinceLastAttack >= attackSpeed) {
        checkForMonstersInRange(monsters);
        if (!monstersInRange.empty()) {
            cocos2d::Vec2 towerPosition = getPosition();
            cocos2d::Vec2 targetPosition = monstersInRange[0]->getPosition();

            // ������ָ��Ŀ�������
            cocos2d::Vec2 direction = targetPosition - towerPosition;
            direction.normalize();  // ��������һ��Ϊ��λ����

            // ������ת�Ƕ�
            float angle = CC_RADIANS_TO_DEGREES(atan2f(direction.y, direction.x));

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

void Bottle::attack(Monster* target) {
    auto bullet = Bullet::createWithTarget(target, "Tower/Bottle/ID1_0.PNG", 1000.0f, 100);
    bullet->setPosition(getPosition());
    this->getParent()->addChild(bullet);
}
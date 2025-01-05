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
    if (bottle && bottle->initWithFile("Tower/Bottle/ID1_22.PNG")) { // 使用正确的图片
        bottle->setTexture("Tower/Bottle/ID1_22.PNG"); // 初始化塔
        bottle->autorelease();
        bottle->setPosition(position);
        bottle->attackRange = 1000.0f; // 设置攻击范围
        bottle->attackSpeed = 1.0f; // 设置攻击速度
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
    // 创建升级按钮
    upgradeButton = cocos2d::ui::Button::create("GameScene/Tower/Btn_CanUpLevel.png");
    upgradeButton->setPosition(Vec2(10, 110)); // 设置位置
    upgradeButton->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            this->upgrade();
        }
        });
    this->addChild(upgradeButton);

    // 创建移除按钮
    removeButton = cocos2d::ui::Button::create("GameScene/Tower/Btn_SellTower.png");
    removeButton->setPosition(Vec2(10, -50)); // 设置位置
    removeButton->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            this->remove();
        }
        });
    this->addChild(removeButton);

    // 创建一个全局触摸监听器
    auto globalListener = EventListenerTouchOneByOne::create();
    globalListener->setSwallowTouches(false); // 不吞噬事件
    globalListener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 touchLocation = touch->getLocation();

        // 检测触摸点是否在防御塔或升级按钮或移除按钮上
        if (!this->getBoundingBox().containsPoint(touchLocation) &&
            !this->upgradeButton->getBoundingBox().containsPoint(touchLocation) &&
            !this->removeButton->getBoundingBox().containsPoint(touchLocation)) {
            // 如果点击在防御塔和按钮之外，隐藏攻击范围和按钮
            this->hideAttackRangeAndButtons();
            return true;
        }
        return false; // 如果点击在防御塔和按钮之外，隐藏攻击范围和按钮
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(globalListener, this);
}

void Bottle::hideAttackRangeAndButtons() {
    // 隐藏攻击范围
    if (attackRange) attackRange_->setVisible(false);

    // 隐藏升级按钮和移除按钮
    if (upgradeButton) upgradeButton->setVisible(false);
    if (removeButton) removeButton->setVisible(false);
}

void Bottle::upgrade()
{
    // 升级防御塔
    if (level < 3) {
        level++;
        // 显示升级后的防御塔
    }
    std::string textureName = "Tower/Bottle/Level" + std::to_string(level) + ".PNG";
    setTexture(textureName);

    // 显示升级后的攻击范围和攻击速度

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

            // 计算攻击方向
            cocos2d::Vec2 direction = targetPosition - towerPosition;
            direction.normalize();  // 将方向向量归一化

            // 计算旋转角度
            float angle = CC_RADIANS_TO_DEGREES(atan2f(direction.y, direction.x));

            auto rotateAction = cocos2d::RotateBy::create(0.01f, angle);
            this->runAction(rotateAction);


            attack(monstersInRange.front()); // 攻击第一个目标
            timeSinceLastAttack = 0;
        }
    }
}

bool Bottle::isMonsterInRange(Monster* monster) {
    return (getPosition().distance(monster->getPosition()) <= attackRange);
}

void Bottle::checkForMonstersInRange(std::vector<Monster*> monsters) {
    // 清除所有在攻击范围内的怪物
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
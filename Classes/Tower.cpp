#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "cocos/ui/UIImageView.h"
#include "Tower.h"


USING_NS_CC;

using namespace cocos2d::ui;
using namespace cocos2d;

Tower::Tower() : level(1), attackSpeed(1.0f), towerType(0), attackDamage(50)
{
    // 初始化其他成员
    attackRange_ = nullptr;
    upgradeButton = nullptr;
    removeButton = nullptr;
}

Tower* Tower::create(Vec2 position,int towerType)
{
    Tower* tower = new (std::nothrow) Tower();
    if (tower && tower->initWithFile("tower_base.png"))
    {
        tower->autorelease();
        tower->towerType = towerType;
        tower->setPosition(position);
        // 初始化其他成员
        return tower;
    }
    CC_SAFE_DELETE(tower);
    return nullptr;
}

bool Tower::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    // 初始化塔
    return true;
}

//----------------------------refactored with flyweight----------------------------//
void Tower::attack()
{
    // 使用享元工厂获取默认子弹
    Flyweight* bullet = BulletFlyweightFactory::getBullet("Tower/Bottle/ID1_0.PNG", attackSpeed, attackDamage);
    
    // 设置目标并添加到场景
    Bullet* concreteBullet = dynamic_cast<Bullet*>(bullet);
    if (concreteBullet && !monstersInRange.empty()) {
        Monster* target = monstersInRange.front();
        concreteBullet->setTarget(target);
        concreteBullet->setPosition(this->getPosition());
        this->getParent()->addChild(concreteBullet);
    }
}

void Tower::upgrade()
{
    // 塔
    //原塔升级为一级塔
}

void Tower::remove()
{
    // 删除塔
    this->removeFromParentAndCleanup(true);
}

void Tower::showAttackRange() {
    attackRange_ = Sprite::create("GameScene/Tower/AttackRange.PNG");
    attackRange_->setPosition(this->getPosition());
    this->getParent()->addChild(attackRange_);

}
void Tower::showUpgradeAndRemoveButtons() {
    // 
    upgradeButton = cocos2d::ui::Button::create("GameScene/Tower/Btn_CanUpLevel.png");
    upgradeButton->setPosition(Vec2(this->getPosition().x, this->getPosition().y + 85)); // 
    upgradeButton->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            this->upgrade();
        }
        });
    this->getParent()->addChild(upgradeButton);

    // 
    removeButton = cocos2d::ui::Button::create("GameScene/Tower/Btn_SellTower.png");
    removeButton->setPosition(Vec2(this->getPosition().x, this->getPosition().y - 85)); // 
    removeButton->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            this->remove();
        }
        });
    this->getParent()->addChild(removeButton);

    // 一键触发
    auto globalListener = EventListenerTouchOneByOne::create();
    globalListener->setSwallowTouches(false); // 忽略点击
    globalListener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 touchLocation = touch->getLocation();

        // 判断是否点击到塔或按钮
        if (!this->getBoundingBox().containsPoint(touchLocation) &&
            !this->upgradeButton->getBoundingBox().containsPoint(touchLocation) &&
            !this->removeButton->getBoundingBox().containsPoint(touchLocation)) {
            // 塔和按钮外，其他地方点击
            this->hideAttackRangeAndButtons();
            return true;
        }
        return false; // 未点击
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(globalListener, this);
}

void Tower::hideAttackRangeAndButtons() {
    // 
    if (attackRange_) attackRange_->setVisible(false);

    // 
    if (upgradeButton) upgradeButton->setVisible(false);
    if (removeButton) removeButton->setVisible(false);
}
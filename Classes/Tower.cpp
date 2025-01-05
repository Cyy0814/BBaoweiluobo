#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "cocos/ui/UIImageView.h"
#include "Tower.h"

//---------------------------------refactored with state pattern---------------------------------//
USING_NS_CC;

using namespace cocos2d::ui;
using namespace cocos2d;

Tower::Tower() : level(1), attackSpeed(1.0f), towerType(0), attackDamage(50), currentState(nullptr) {
    // 初始化其他成员
    attackRange_ = nullptr;
    upgradeButton = nullptr;
    removeButton = nullptr;
}

Tower::~Tower() {
    if (currentState) {
        delete currentState;
        currentState = nullptr;
    }
}

Tower* Tower::create(Vec2 position, int towerType) {
    Tower* tower = new (std::nothrow) Tower();
    if (tower && tower->initWithFile("tower_base.png")) {
        tower->autorelease();
        tower->towerType = towerType;
        tower->setPosition(position);
        // 初始化其他成员
        return tower;
    }
    CC_SAFE_DELETE(tower);
    return nullptr;
}

bool Tower::init() {
    if (!Sprite::init()) {
        return false;
    }
    
    //---------------------------------refactored with state pattern---------------------------------//
    // 设置初始状态为空闲
    // 启动update
    currentState = new IdleState();
    currentState->enter(this);
    
    // 由当前状态处理更新
    this->scheduleUpdate();
    
    return true;
}

//---------------------------------refactored with state pattern---------------------------------//
void Tower::changeState(TowerState* newState) {
    if (currentState) {
        currentState->exit(this);
        delete currentState;
    }
    currentState = newState;
    if (currentState) {
        currentState->enter(this);
    }
}

void Tower::update(float dt, std::vector<Monster*> monsters) {
    timeSinceLastAttack += dt;
    
    // 由当前状态处理更新
    if (currentState) {
        currentState->update(this, dt, monsters);
    }
}

void Tower::attack() {
    // 攻击策略
}

void Tower::upgrade() {
    // 防御塔升级逻辑
    // 更新防御塔属性和外观
}

void Tower::remove() {
    // 移除防御塔
    this->removeFromParentAndCleanup(true);
}

void Tower::showAttackRange() {
    attackRange_ = Sprite::create("GameScene/Tower/AttackRange.PNG");
    attackRange_->setPosition(this->getPosition());
    this->getParent()->addChild(attackRange_);
}

void Tower::showUpgradeAndRemoveButtons() {
    // 创建升级按钮
    upgradeButton = cocos2d::ui::Button::create("GameScene/Tower/Btn_CanUpLevel.png");
    upgradeButton->setPosition(Vec2(this->getPosition().x, this->getPosition().y + 85)); // 设置位置
    upgradeButton->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            this->upgrade();
        }
    });
    this->getParent()->addChild(upgradeButton);

    // 创建移除按钮
    removeButton = cocos2d::ui::Button::create("GameScene/Tower/Btn_SellTower.png");
    removeButton->setPosition(Vec2(this->getPosition().x, this->getPosition().y - 85)); // 设置位置
    removeButton->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            this->remove();
        }
    });
    this->getParent()->addChild(removeButton);

    // 创建一个全局触摸监听器
    auto globalListener = EventListenerTouchOneByOne::create();
    globalListener->setSwallowTouches(false); // 不吞噬事件
    globalListener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 touchLocation = touch->getLocation();

        // 检测触摸点是否在防御塔或按钮范围内
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

void Tower::hideAttackRangeAndButtons() {
    // 隐藏攻击范围
    if (attackRange_) attackRange_->setVisible(false);

    // 隐藏升级按钮和移除按钮
    if (upgradeButton) upgradeButton->setVisible(false);
    if (removeButton) removeButton->setVisible(false);
}

bool Tower::isMonsterInRange(Monster* monster) {
    return (getPosition().distance(monster->getPosition()) <= attackRange);
}

void Tower::checkForMonstersInRange(std::vector<Monster*> monsters) {
    // 清空怪物列表
    monstersInRange.clear();
    if (monsters.size()) {
        for (Monster* monster : monsters) {
            if (isMonsterInRange(monster)) {
                monstersInRange.push_back(monster);
            }
        }
    }
}
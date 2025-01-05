#include "PickTower.h"
#include "Bottle.h"
#include "cocos2d.h"
#include "Level1.h"
#include "Level2.h"
#include "GoldCoin.h"
#include "Global.h"
#include "Tower.h"
using namespace cocos2d;

USING_NS_CC;

PickTower* PickTower::createWithPositions(const std::vector<cocos2d::Vec2>& positions) {
    PickTower* ptr = new (std::nothrow) PickTower();
    if (ptr && ptr->initWithPositions(positions)) {
        ptr->autorelease();
        return ptr;
    }
    CC_SAFE_DELETE(ptr);
    return nullptr;
}

bool PickTower::initWithPositions(const std::vector<cocos2d::Vec2>& positions) {
    if (!Node::init()) {
        return false;
    }

    // 保存传入的位置
    Positions = positions;

    // 初始化选择菜单（这只是简化的实现，可能需要更精美的UI和动画）
    menu = Node::create();
    auto click = Sprite::create("GameScene/click.png");
    tower1Item = Sprite::create("Tower/Bottle/CanClick1.PNG"); // 塔1图标
    tower2Item = Sprite::create("Tower/Fan/CanClick1.PNG");    // 塔2图标
    tower3Item = Sprite::create("Tower/Shit/CanClick1.PNG");   // 塔3图标
    click->setPosition(Director::getInstance()->getVisibleOrigin() / 2); // 闪烁

    // 添加到menu
    menu->addChild(click);
    menu->addChild(tower1Item);
    menu->addChild(tower2Item);
    menu->addChild(tower3Item);

    // 添加点击位置的监听器
    auto listener = EventListenerTouchOneByOne::create();

    // 为每个防御塔选项添加触摸事件监听器
    auto createTowerBottleListener = EventListenerTouchOneByOne::create();
    createTowerBottleListener->setSwallowTouches(true); // 吞噬事件

    // 隐藏菜单
    menu->setVisible(false);

    tower1Item->setPosition(Vec2(-80, 80));
    tower2Item->setPosition(Vec2(0, 80));
    tower3Item->setPosition(Vec2(80, 80));

    // 添加到menu
    this->addChild(menu);

    // 添加到event dispatcher
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // 为每个防御塔选项添加触摸事件监听器
    _eventDispatcher->addEventListenerWithSceneGraphPriority(createTowerBottleListener, tower1Item);

    auto createTowerFanListener = EventListenerTouchOneByOne::create();
    createTowerFanListener->setSwallowTouches(true); // 吞噬事件
    _eventDispatcher->addEventListenerWithSceneGraphPriority(createTowerFanListener, tower2Item);

    auto createTowerShitListener = EventListenerTouchOneByOne::create();
    createTowerShitListener->setSwallowTouches(true); // 吞噬事件
    _eventDispatcher->addEventListenerWithSceneGraphPriority(createTowerShitListener, tower3Item);

    return true;
}

bool PickTower::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    cocos2d::Vec2 location = touch->getLocation();

    for (const auto& pos : Positions) {
        if (isPointNearLocation(location, pos)) {
            selectedPosition = pos;
            showMenuAtPosition(pos);
            return true;
        }
        else {
            menu->setVisible(false);
        }
    }
    return false;
}

void PickTower::showMenuAtPosition(const cocos2d::Vec2& position) {
    // 设置菜单的位置
    menu->setPosition(position);
    menu->setVisible(true);
}

// 创建TowerBottle
void PickTower::createTowerBottle(const cocos2d::Vec2& position) {
    auto towerBottle = Bottle::create(position);
    this->addChild(towerBottle);
    addTowerTouchListener(towerBottle);
}

// 创建TowerFan
void PickTower::createTowerFan(const cocos2d::Vec2& position) {
    auto towerFan = Fan::create(position);
    this->addChild(towerFan);
    addTowerTouchListener(towerFan);
}

// 创建TowerShit
void PickTower::createTowerShit(const cocos2d::Vec2& position) {
    auto towerShit = Shit::create(position);
    this->addChild(towerShit);
    addTowerTouchListener(towerShit);
}

// 添加触摸事件监听器
void PickTower::addTowerTouchListener(Tower* tower) {
    auto towerListener = EventListenerTouchOneByOne::create();
    towerListener->setSwallowTouches(true);
    towerListener->onTouchBegan = [tower](Touch* touch, Event* event) {
        if (tower->getBoundingBox().containsPoint(touch->getLocation())) {
           tower->showAttackRange();
           tower->showUpgradeAndRemoveButtons();
            return true;
        }
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(towerListener, tower);
}

bool PickTower::isPointNearLocation(const cocos2d::Vec2& point, const cocos2d::Vec2& location) {
    // 计算距离
    return point.distance(location) < 50.0f; // 50像素为有效范围
}
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

    // ���洫���λ��
    Positions = positions;

    // ��ʼ��ѡ��˵�������ֻ�Ǽ򻯵�ʵ�֣�������Ҫ�������UI���������
    menu = Node::create();
    auto click = Sprite::create("GameScene/click.png");
    tower1Item = Sprite::create("Tower/Bottle/CanClick1.PNG"); // ��1��ͼ��
    tower2Item = Sprite::create("Tower/Fan/CanClick1.PNG");    // ��2��ͼ��
    tower3Item = Sprite::create("Tower/Shit/CanClick1.PNG");    // ��3��ͼ��
    click->setPosition(Director::getInstance()->getVisibleOrigin() / 2); // �����˸
    // ��ӵ�menu
    menu->addChild(click);
    menu->addChild(tower1Item);
    menu->addChild(tower2Item);
    menu->addChild(tower3Item);

    menu->setVisible(false);
    this->addChild(menu);

    tower1Item->setPosition(Vec2(-80, 80));
    tower2Item->setPosition(Vec2(0, 80));
    tower3Item->setPosition(Vec2(80, 80));

    // ��ӵ��λ�õļ�����
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(PickTower::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // Ϊÿ������ѡ����Ӵ����¼�������
    auto createTowerBottleListener = EventListenerTouchOneByOne::create();
    createTowerBottleListener->setSwallowTouches(true); // ���ɴ����¼�
    createTowerBottleListener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 touchLocation = touch->getLocation();
        Vec2 localLocation = tower1Item->getParent()->convertToNodeSpace(touchLocation);

        if (tower1Item->getBoundingBox().containsPoint(localLocation)) {

            createTowerBottle(selectedPosition);
            goldCoin->purchaseTower(100);

            menu->setVisible(false); // ���ز˵�
            return true;
        }
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(createTowerBottleListener, tower1Item);

    auto createTowerFanListener = EventListenerTouchOneByOne::create();
    createTowerFanListener->setSwallowTouches(true); // ���ɴ����¼�
    createTowerFanListener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 touchLocation = touch->getLocation();
        Vec2 localLocation = tower2Item->getParent()->convertToNodeSpace(touchLocation);

        if (tower2Item->getBoundingBox().containsPoint(localLocation)) {

            createTowerFan(selectedPosition);
            goldCoin->purchaseTower(160);

            menu->setVisible(false); // ���ز˵�
            return true;
        }
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(createTowerFanListener, tower2Item);

    // Ϊÿ������ѡ����Ӵ����¼�������
    auto createTowerShitListener = EventListenerTouchOneByOne::create();
    createTowerShitListener->setSwallowTouches(true); // ���ɴ����¼�
    createTowerShitListener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 touchLocation = touch->getLocation();
        Vec2 localLocation = tower3Item->getParent()->convertToNodeSpace(touchLocation);

        if (tower3Item->getBoundingBox().containsPoint(localLocation)) {

            createTowerShit(selectedPosition);
            goldCoin->purchaseTower(120);

            menu->setVisible(false); // ���ز˵�
            return true;
        }
        return false;
    };
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

    // ��ʾ�˵�������λ��
    menu->setPosition(position);
    menu->setVisible(true); 
}

// ���� TowerBottle ��������Ӵ����¼�������
void PickTower::createTowerBottle(const cocos2d::Vec2& position) {
    auto towerBottle = Bottle::create(position);
    this->addChild(towerBottle);
    addTowerTouchListener(towerBottle);
}

// ���� TowerFan ��������Ӵ����¼�������
void PickTower::createTowerFan(const cocos2d::Vec2& position) {
    auto towerFan = Fan::create(position);
    this->addChild(towerFan);
    addTowerTouchListener(towerFan);
}

// ���� TowerShit ��������Ӵ����¼�������
void PickTower::createTowerShit(const cocos2d::Vec2& position) {
    auto towerShit = Shit::create(position);
    this->addChild(towerShit);
    addTowerTouchListener(towerShit);
}

// Ϊ������Ӵ����¼���������ͨ�÷���
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
    // �򵥵ľ����⣬���Ը�����Ҫ������ֵ
    return point.distance(location) < 50.0f; // ����50����Ϊ�����Ч����
}
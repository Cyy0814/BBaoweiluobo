#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "cocos/ui/UIImageView.h"
#include "Tower.h"

// Refactored with State Pattern
USING_NS_CC;

using namespace cocos2d::ui;
using namespace cocos2d;

Tower::Tower() : level(1), attackSpeed(1.0f), towerType(0), currentState(nullptr) {
}

Tower::~Tower() {
    if (currentState) {
        delete currentState;
        currentState = nullptr;
    }
}

bool Tower::init() {
    if (!Sprite::init()) {
        return false;
    }
    
    // ���ó�ʼ״̬Ϊ����
    currentState = new IdleState();
    currentState->enter(this);
    
    // ����update
    this->scheduleUpdate();
    
    return true;
}

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
    
    // �õ�ǰ״̬�������
    if (currentState) {
        currentState->update(this, dt, monsters);
    }
}

void Tower::attack()
{
    // ?????????
    // ??????�p????????? Projectile ?????????????
}

void Tower::upgrade()
{
    // ????????????????
    //??????????????????????????????
}

void Tower::remove()
{
    // ??????
    this->removeFromParentAndCleanup(true);
}

void Tower::showAttackRange() {
    attackRange_ = Sprite::create("GameScene/Tower/AttackRange.PNG");
    attackRange_->setPosition(this->getPosition());
    this->getParent()->addChild(attackRange_);

}
void Tower::showUpgradeAndRemoveButtons() {
    // ???????????
    upgradeButton = cocos2d::ui::Button::create("GameScene/Tower/Btn_CanUpLevel.png");
    upgradeButton->setPosition(Vec2(this->getPosition().x, this->getPosition().y + 85)); // ????��??
    upgradeButton->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            this->upgrade();
        }
        });
    this->getParent()->addChild(upgradeButton);

    // ??????????
    removeButton = cocos2d::ui::Button::create("GameScene/Tower/Btn_SellTower.png");
    removeButton->setPosition(Vec2(this->getPosition().x, this->getPosition().y - 85)); // ????��??
    removeButton->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            this->remove();
        }
        });
    this->getParent()->addChild(removeButton);

    // ???????????????????
    auto globalListener = EventListenerTouchOneByOne::create();
    globalListener->setSwallowTouches(false); // ????????????
    globalListener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 touchLocation = touch->getLocation();

        // ??????????????????????????????????
        if (!this->getBoundingBox().containsPoint(touchLocation) &&
            !this->upgradeButton->getBoundingBox().containsPoint(touchLocation) &&
            !this->removeButton->getBoundingBox().containsPoint(touchLocation)) {
            // ????????????????????????��????
            this->hideAttackRangeAndButtons();
            return true;
        }
        return false; // ???????????????
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(globalListener, this);
}

void Tower::hideAttackRangeAndButtons() {
    // ?????????��??
    if (attackRange) attackRange_->setVisible(false);

    // ????????????????
    if (upgradeButton) upgradeButton->setVisible(false);
    if (removeButton) removeButton->setVisible(false);
}

bool Tower::isMonsterInRange(Monster* monster) {
    return (getPosition().distance(monster->getPosition()) <= attackRange);
}

void Tower::checkForMonstersInRange(std::vector<Monster*> monsters) {
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
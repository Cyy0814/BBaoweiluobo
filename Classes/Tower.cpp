#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "cocos/ui/UIImageView.h"
#include "Tower.h"


USING_NS_CC;

using namespace cocos2d::ui;
using namespace cocos2d;

Tower::Tower() : level(1), attackSpeed(1.0f), towerType(0)
{
    // ��ʼ������
}

Tower* Tower::create(Vec2 position,int towerType)
{
    Tower* tower = new (std::nothrow) Tower();
    if (tower && tower->initWithFile("tower_base.png"))
    {
        tower->autorelease();
        tower->towerType = towerType;
        tower->setPosition(position);
        // �������ͳ�ʼ����ͬ������
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

    // ��ʼ����̨
    return true;
}
void Tower::attack()
{
    // ʵ�ֹ����߼�
    // ������漰������һ�� Projectile �࣬�����䷢�䵽Ŀ��
}

void Tower::upgrade()
{
    // ������̨���������
    //��ԭ�ȵ���̨�Ƴ�����������һ������̨
}

void Tower::remove()
{
    // �Ƴ���̨
    this->removeFromParentAndCleanup(true);
}

void Tower::showAttackRange() {
    attackRange_ = Sprite::create("GameScene/Tower/AttackRange.PNG");
    attackRange_->setPosition(this->getPosition());
    this->getParent()->addChild(attackRange_);

}
void Tower::showUpgradeAndRemoveButtons() {
    // ����������ť
    upgradeButton = cocos2d::ui::Button::create("GameScene/Tower/Btn_CanUpLevel.png");
    upgradeButton->setPosition(Vec2(this->getPosition().x, this->getPosition().y + 85)); // ����λ��
    upgradeButton->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            this->upgrade();
        }
        });
    this->getParent()->addChild(upgradeButton);

    // �����Ƴ���ť
    removeButton = cocos2d::ui::Button::create("GameScene/Tower/Btn_SellTower.png");
    removeButton->setPosition(Vec2(this->getPosition().x, this->getPosition().y - 85)); // ����λ��
    removeButton->addTouchEventListener([this](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            this->remove();
        }
        });
    this->getParent()->addChild(removeButton);

    // ���һ��ȫ�ִ���������
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

void Tower::hideAttackRangeAndButtons() {
    // ���ع�����Χָʾ
    if (attackRange) attackRange_->setVisible(false);

    // �����������Ƴ���ť
    if (upgradeButton) upgradeButton->setVisible(false);
    if (removeButton) removeButton->setVisible(false);
}
#include "Global.h"
using namespace cocos2d;
USING_NS_CC;

Monster* Monster::createWithType(int monsterType) {
    int hitPoints = 100; // Ĭ������ֵ�����ݹ������Ϳ��Բ�ͬ
    // ���ݹ����������ò�ͬ������ֵ
    switch (monsterType) {
        case 1:
            hitPoints = 100;
            break;
        case 2:
            hitPoints = 150;
            break;
        case 3:
            hitPoints = 200;
            break;
        default:
            break;
    }

    Monster* monster = new (std::nothrow) Monster(monsterType, hitPoints);
    if (monster && monster->init()) {
        monster->autorelease();
        return monster;
    }
    CC_SAFE_DELETE(monster);
    return nullptr;
}

Monster::Monster(int monsterType, int hitPoints)
    : _monsterType(monsterType), _hitPoints(hitPoints) {
}

bool Monster::init() {
    if (!Sprite::init()) {
        return false;
    }

    // �������ͳ�ʼ����������
    std::string filename = "Monster/monster" + std::to_string(_monsterType) + ".png";
    this->initWithFile(filename);

    // ������ʾ����ֵ�ı�ǩ
    //_hpLabel = Label::createWithSystemFont(std::to_string(_hitPoints), "Arial", 12);
    //_hpLabel->setPosition(Vec2(this->getContentSize() / 2));
    //this->addChild(_hpLabel);

    return true;
}

void Monster::moveOnPath(const std::vector<Vec2>& path) {
    if (path.empty()) return;

    // ������������
    Vector<FiniteTimeAction*> actions;

    for (size_t i = 1; i < path.size(); ++i) {
        // ���㵱ǰ�㵽��һ������ƶ�ʱ�䣬������������ٶ��ǹ̶���
        float moveDuration = 0.4f; // ÿ��·���ƶ������ʱ�䣬�����Ը��ݹ�����ٶȽ��е���

        // �����ƶ�����һ����Ķ���
        auto moveAction = MoveTo::create(moveDuration, path[i]);
        actions.pushBack(moveAction);
    }
    // ���һ���ص����������ƶ����ʱ����
    auto callbackAction = cocos2d::CallFunc::create([this]() {

        globalCarrot->decreaseHealth(); // �����ܲ���Ѫ��

        this->removeFromParent(); // �Ƴ�����
        // ����������������������룬���������Ϸ״̬
        });
    actions.pushBack(callbackAction);

    // �������ж���
    auto sequence = Sequence::create(actions);

    // ���ж���
    this->runAction(sequence);
}

void Monster::getAttacked(int damage) {
    _hitPoints -= damage;
    updateHPLabel();

    if (_hitPoints <= 0) {
        die();
    }
    else {
        showHitEffect();
    }
}

void Monster::showHitEffect() {
    // ʵ�ֱ���������Ч������˸���ɫ����
}

void Monster::dropCoins() {
    // ʵ�ֽ�ҵ�����߼�
}

void Monster::updateHPLabel() {
    _hpLabel->setString(std::to_string(_hitPoints));
}

void Monster::die() {
    dropCoins();
    this->removeFromParent();
}
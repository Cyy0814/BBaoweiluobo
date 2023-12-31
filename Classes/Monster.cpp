#include<Monster.h>
#include<Global.h>
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;
USING_NS_CC;

Monster* Monster::createWithType(int monsterType) {
    int hitPoints = 100; // Ĭ������ֵ�����ݹ������Ϳ��Բ�ͬ
    // ���ݹ����������ò�ͬ������ֵ
    switch (monsterType) {
        case 1:
            hitPoints = 150;
            break;
        case 2:
            hitPoints = 200;
            break;
        case 3:
            hitPoints = 300;
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

void Monster::moveOnPath(const std::vector<cocos2d::Vec2>& path) {
    if (path.empty()) return;

    // ������������
    Vector<FiniteTimeAction*> actions;

    for (size_t i = 1; i < path.size(); ++i) {
        // ÿ��·���ƶ������ʱ�䣬�����Ը��ݹ�����ٶȽ��е���
        float moveDuration = path[i].length() / speed;

        // �����ƶ�����һ����Ķ���
        auto moveAction = MoveTo::create(moveDuration, path[i]);
        actions.pushBack(moveAction);
    }
    // ���һ���ص����������ƶ����ʱ����
    auto callbackAction = cocos2d::CallFunc::create([this]() {

        if (this->_hitPoints > 0)
            globalCarrot->decreaseHealth();// �����ܲ���Ѫ��
        removeFromMonstersArray(this);
        this->removeFromParent(); // �Ƴ�����
        // ����������������������룬���������Ϸ״̬
        });
    actions.pushBack(callbackAction);

    // �������ж���
    auto sequence = Sequence::create(actions);

    // ���ж���
    runAction(sequence);
}

void Monster::getAttacked(int damage) {
    //updateHPLabel();
    if (!isAlive) return; // ��������Ѿ�������ֱ�ӷ���

    _hitPoints -= damage;

    if (_hitPoints <= 0) {
        _hitPoints = 0;
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
    // ����30���
    goldCoin->earnGold(30);
}

/*
void Monster::updateHPLabel() {
    _hpLabel->setString(std::to_string(_hitPoints));
}
*/

void Monster::removeFromMonstersArray(Monster* monster)
{
    auto iter = std::find(monsters.begin(), monsters.end(), monster);
    if (iter != monsters.end())
    {
        monsters.erase(iter);
    }
}

void Monster::die() {
    // ��ը��Ч
    auto explosion = cocos2d::Sprite::create("Monster/explosion.PNG");
    explosion->setPosition(this->getPosition());
    this->getParent()->addChild(explosion);

    // ����һ���������Ƴ���ը��Ч
    auto fadeOut = cocos2d::FadeOut::create(0.5f); // ����ʱ����Ը�����Ҫ����
    auto removeExplosion = cocos2d::RemoveSelf::create();
    auto sequence = cocos2d::Sequence::create(fadeOut, removeExplosion, nullptr);
    explosion->runAction(sequence);

    dropCoins();
    removeFromMonstersArray(this);
    this->setVisible(false);
    //this->removeFromParent();
}
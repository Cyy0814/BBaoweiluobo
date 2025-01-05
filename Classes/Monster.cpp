#include<Monster.h>
#include<Global.h>
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;
USING_NS_CC;

/*Refactored with State Pattern*/
Monster* Monster::createWithType(int monsterType) {
    int hitPoints = 100;
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
    : _monsterType(monsterType), _hitPoints(hitPoints), currentState(nullptr) {
}

bool Monster::init() {
    if (!Sprite::init()) {
        return false;
    }

    // 设置怪物图片
    std::string filename = "Monster/monster" + std::to_string(_monsterType) + ".png";
    this->initWithFile(filename);

    /*Refactored with State Pattern*/
    // 初始化为行走状态
    currentState = new WalkState();
    currentState->enter(this);

    // 启用update
    scheduleUpdate();

    return true;
}

/*Refactored with State Pattern*/
void Monster::changeState(MonsterState* newState) {
    if (currentState) {
        currentState->exit(this);
        delete currentState;
    }
    currentState = newState;
    if (currentState) {
        currentState->enter(this);
    }
}

void Monster::update(float dt) {
    if (currentState) {
        currentState->update(this, dt);
    }
}

void Monster::moveOnPath(const std::vector<cocos2d::Vec2>& path) {
    if (path.empty()) return;

    Vector<FiniteTimeAction*> actions;

    for (size_t i = 1; i < path.size(); ++i) {
        float moveDuration = path[i].length() / speed;
        auto moveAction = MoveTo::create(moveDuration, path[i]);
        actions.pushBack(moveAction);
    }
    auto callbackAction = CallFunc::create([this]() {
        if (this->_hitPoints > 0)
            globalCarrot->decreaseHealth();
        removeFromMonstersArray(this);
        this->removeFromParent();
    });
    actions.pushBack(callbackAction);

    auto sequence = Sequence::create(actions);
    runAction(sequence);
}

void Monster::getAttacked(int damage) {
    if (!isAlive) return;

    if (currentState) {
        currentState->handleDamage(this, damage);
    } else {
        // 原有的伤害处理逻辑作为后备
        _hitPoints -= damage;
        if (_hitPoints <= 0) {
            _hitPoints = 0;
            die();
        } else {
            showHitEffect();
        }
    }
}

void Monster::showHitEffect() {
    // TODO: 实现受击特效
}

void Monster::setAnimation(const std::string& name) {
    // TODO: 实现动画切换逻辑
    // 这里需要根据具体的动画系统来实现
}

void Monster::showSlowEffect() {
    if (!slowEffect) {
        slowEffect = Sprite::create("Tower/Shit/ID2_10.PNG");
        if (slowEffect) {
            addChild(slowEffect);
            slowEffect->setPosition(Vec2(40, -slowEffect->getContentSize().height / 2 + 10));
        }
    }
    slowEffect->setVisible(true);
}

void Monster::hideSlowEffect() {
    if (slowEffect) {
        slowEffect->setVisible(false);
    }
}

void Monster::dropCoins() {
    // 掉落30金币
    goldCoin->earnGold(30);
}

void Monster::removeFromMonstersArray(Monster* monster)
{
    auto iter = std::find(monsters.begin(), monsters.end(), monster);
    if (iter != monsters.end())
    {
        monsters.erase(iter);
    }
}

void Monster::die() {
    // 爆炸特效
    auto explosion = cocos2d::Sprite::create("Monster/explosion.PNG");
    explosion->setPosition(this->getPosition());
    this->getParent()->addChild(explosion);

    // 设置一个动作来移除爆炸特效
    auto fadeOut = cocos2d::FadeOut::create(0.5f);
    auto removeExplosion = cocos2d::RemoveSelf::create();
    auto sequence = cocos2d::Sequence::create(fadeOut, removeExplosion, nullptr);
    explosion->runAction(sequence);

    dropCoins();
    removeFromMonstersArray(this);
    this->setVisible(false);
}

/*Refactored with State Pattern*/
// 新增：处理减速效果
void Monster::handleSlowEffect(int damage) {
    if (!isAlive) return;

    // 处理伤害
    if (currentState) {
        currentState->handleDamage(this, damage);
    } else {
        _hitPoints -= damage;
        if (_hitPoints <= 0) {
            _hitPoints = 0;
            die();
            return;
        }
    }

    // 如果当前不是减速状态，则切换到减速状态
    if (!dynamic_cast<SlowedState*>(currentState)) {
        changeState(new SlowedState());
    }
}
#include "Global.h"
using namespace cocos2d;
using namespace cocos2d::ui;
USING_NS_CC;

//---------------------------------refactored with state pattern---------------------------------//
bool Carrot::init() {
    if (!Node::init()) {
        return false;
    }

    health = MAX_HEALTH;
    currentState = new HealthyState();
    return true;
}

//---------------------------------refactored with observer pattern---------------------------------//
Carrot::~Carrot() {
    if (currentState) {
        delete currentState;
        currentState = nullptr;
    }
}

void Carrot::setHealth(int value) {
    if (value != health) {
        health = value;
        if (health < 0) health = 0;
        if (health > MAX_HEALTH) health = MAX_HEALTH;
        
        // 通知观察者健康发生变化
        notifyObservers("health_changed", health);
        
        // 处理健康变化
        if (currentState) {
            currentState->handleHealthChange(this, health);
        }
    }
}

void Carrot::decreaseHealth() {
    setHealth(health - 1);
}

//---------------------------------refactored with state pattern---------------------------------//
void Carrot::changeState(CarrotState* newState) {
    if (currentState) {
        delete currentState;
    }
    currentState = newState;
    
    // 通知观察者状态发生变化
    notifyObservers("state_changed", health);
}

std::string Carrot::getCurrentTexturePath() const {
    return currentState ? currentState->getTexturePath() : "myCarrot/HP_MAX.PNG";
}
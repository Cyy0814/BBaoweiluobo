#include "Global.h"
using namespace cocos2d;
using namespace cocos2d::ui;
USING_NS_CC;

// Refactored with State Pattern
bool Carrot::init() {
    if (!Node::init()) {
        return false;
    }

    health = MAX_HEALTH;
    currentState = new HealthyState();
    return true;
}

// Refactored with Observer Pattern
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
        
        // ֪ͨ���й۲��߽���ֵ�����仯
        notifyObservers("health_changed", health);
        
        // �õ�ǰ״̬����Ѫ���仯
        if (currentState) {
            currentState->handleHealthChange(this, health);
        }
    }
}

void Carrot::decreaseHealth() {
    setHealth(health - 1);
}

void Carrot::changeState(CarrotState* newState) {
    if (currentState) {
        delete currentState;
    }
    currentState = newState;
    
    // ֪ͨ�۲���״̬�����仯
    notifyObservers("state_changed", health);
}

std::string Carrot::getCurrentTexturePath() const {
    return currentState ? currentState->getTexturePath() : "myCarrot/HP_MAX.PNG";
}
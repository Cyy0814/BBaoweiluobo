#ifndef __CARROT_H__
#define __CARROT_H__

#include "cocos2d.h"
#include "Subject.h"
#include "CarrotState.h"

//---------------------------------refactored with observer pattern---------------------------------//
class Carrot : public cocos2d::Node, public Subject {
private:
    int health;
    CarrotState* currentState;

public:
    virtual bool init() override;
    virtual ~Carrot();
    
    // HP相关
    static const int MAX_HEALTH = 10;
    int getHealth() const { return health; }
    void setHealth(int value);
    void decreaseHealth();
    
    //---------------------------------refactored with state pattern---------------------------------//
    // 状态相关
    void changeState(CarrotState* newState);
    CarrotState* getCurrentState() const { return currentState; }
    std::string getCurrentTexturePath() const;

    CREATE_FUNC(Carrot);
};

#endif

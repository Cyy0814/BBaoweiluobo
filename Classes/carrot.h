#ifndef __CARROT_H__
#define __CARROT_H__

#include "cocos2d.h"
#include "Subject.h"
#include "CarrotState.h"

// Refactored with Observer Pattern
class Carrot : public cocos2d::Node, public Subject {
private:
    int health;
    CarrotState* currentState;

public:
    virtual bool init() override;
    virtual ~Carrot();
    
    // HP���
    static const int MAX_HEALTH = 10;
    int getHealth() const { return health; }
    void setHealth(int value);
    void decreaseHealth();
    
    // ״̬���
    void changeState(CarrotState* newState);
    CarrotState* getCurrentState() const { return currentState; }
    std::string getCurrentTexturePath() const;

    CREATE_FUNC(Carrot);
};

#endif

#ifndef __FLYWEIGHT_H__
#define __FLYWEIGHT_H__

#include "Monster.h"

//---------------------------------refactored with flyweight pattern---------------------------------//
class Flyweight {
public:
    virtual void moveToTarget() = 0;
    virtual void onHitMonster(Monster* monster) = 0;
    virtual void setTarget(Monster* target) = 0;
    virtual ~Flyweight() {}
};

#endif // __FLYWEIGHT_H__ 
#ifndef __SHIT_H__
#define __SHIT_H__

#include "cocos2d.h"
#include "Monster.h"
#include "shitBullet.h"
#include "Tower.h"
#include "ui/CocosGUI.h"

class Shit : public Tower {
public:
    static Shit* create(const cocos2d::Vec2& position);

    void upgrade();
    void remove() override;

    // ��������߼�
    void update(float dt, std::vector<Monster*> monsters);
    void attack(Monster* target);

private:

    ShitBullet* shitbullet;
};
#endif 


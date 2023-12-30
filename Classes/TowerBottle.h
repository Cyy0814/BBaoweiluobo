#ifndef __TOWER_BOTTLE_H__
#define __TOWER_BOTTLE_H__

#include "Tower.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;
class Bottle : public Tower
{
public:
    Bottle();//���캯������ʼ����Ա����
    //virtual ~Bottle();//����
    
    static Bottle* create(Vec2 position);//������������������һ����

    /*bool init() override; */
    void attack();
    void upgrade();
    //void remove();
    void setTowerImage(const std::string& imagePath);
protected:
    int level;
    int attackPower;
    float attackSpeed;
    DrawNode* attackRangeIndicator;
    // �������Ժͷ���...

    int towerType;
    unordered_map<int, std::string> levelToImagePathMap =
    {
        {1,"Resources/Tower/Bottle/ID1_22.PNG"},
        {2,"Resources/Tower/Bottle/ID1_15.PNG"},
        {3,"Resources/Tower/Bottle/ID1_5.PNG"}
    };
};
#endif
#ifndef __PICKTOWER_H__
#define __PICKTOWER_H__
#include "Level1.h"
#include "cocos2d.h"
#include "Bottle.h"
#include "Tower.h"// ����������ص�ͷ�ļ�

class PickTower : public cocos2d::Node {
public:
    static PickTower* createWithPositions(const std::vector<cocos2d::Vec2>& positions);
    bool initWithPositions(const std::vector<cocos2d::Vec2>& positions);
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void showMenuAtPosition(const cocos2d::Vec2& position);
    void createTowerBottle(const cocos2d::Vec2& position);
    void createTowerShit(const cocos2d::Vec2& position);
    void createTowerFan(const cocos2d::Vec2& position);
    void addTowerTouchListener(Tower* tower);
    bool isPointNearLocation(const cocos2d::Vec2& point, const cocos2d::Vec2& location);

private:
    Sprite* tower1Item;
    Sprite* tower2Item;
    Sprite* tower3Item;
    cocos2d::Node* menu;                  // 选择菜单
    cocos2d::Vec2 selectedPosition;       // 当前选择的位置
    std::vector<cocos2d::Vec2> Positions; // 可点击的位置
};


#endif
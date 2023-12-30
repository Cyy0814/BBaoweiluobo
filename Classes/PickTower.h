#ifndef __PICKTOWER_H__
#define __PICKTOWER_H__
#include"Level1.h"
#include "cocos2d.h"

class PickTower : public cocos2d::Sprite
{
public:
    cocos2d::Sprite* startSprite; // ȷ��������
    cocos2d::Sprite* tower1;  // ȷ�������� tower1
    cocos2d::Sprite* tower2;  // ȷ�������� tower2
    static PickTower* create() {
        PickTower* pRet = new(std::nothrow) PickTower();
        if (pRet && pRet->init()) {
            pRet->autorelease();
            return pRet;
        }
        else {
            delete pRet;
            return nullptr;
        }
    }
    virtual bool init();
    void setInitPositions(const std::vector<Vec2>& positions) {
        initPositions = positions;
    }
private:
    std::vector<cocos2d::Sprite*> gridSprites; // �洢��������ͼ��
    std::vector<cocos2d::Vec2> initPositions;  // �洢���г�ʼλ��
};

#endif
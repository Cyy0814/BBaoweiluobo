#ifndef __TOWER_H__
#define __TOWER_H__

#include "cocos2d.h"
using namespace cocos2d;
using namespace cocos2d::ui;

class Tower : public cocos2d::Sprite
{
public:
    Tower();
    /*virtual ~Tower();//��sprite�����Ѿ�������������*/

    static Tower* create(Vec2 position,int towerType);//����һ��������ָ��λ�ü���ͼƬ

    virtual bool init() override;//�жϳ�ʼ���Ƿ�ɹ�
    virtual void attack();//������ʵ����һ��bullet��Ѱ·��������������ʧ���ٴ����ɡ���׼ʱҪ�ı�Ƕ�
    virtual void upgrade() ;//�������������Ե��������ı�ͼƬ
    virtual void remove() ;//�Ƴ�
    virtual void onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);//���ʱչʾ��Χ
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void setTowerImage(const std::string& imagePath);
protected:
    int level;
    int attackPower;
    int towerType;
    float attackSpeed;
    float attackRange;
    

};
#endif
#ifndef __CARROT_H__
#define __CARROT_H__

#include "cocos2d.h"

class Carrot : public cocos2d::Sprite
{
public:
    static Carrot* createCarrot();

    virtual bool init();

    // ���Ѫ�����Ժ���ط���
    CC_SYNTHESIZE(int, m_hp, Hp);  // Ѫ������

    void decreaseHp(int value);  // ����Ѫ���ķ���

    CREATE_FUNC(Carrot);
};

#endif
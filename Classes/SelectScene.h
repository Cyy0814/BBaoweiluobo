#ifndef __SELECT_SCENE_H__
#define __SELECT_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;
using namespace cocos2d::ui;

enum LevelTags {
    // �ؿ���ǩ
    TAG_LEVEL_1 = 100,
    TAG_LEVEL_2,
    TAG_LEVEL_3,
    TAG_LEVEL_4,
    TAG_LEVEL_5,
    // ҳ���ǩ
    TAG_LEVEL_IMAGE = 200,
    // ������ǩ
    TAG_LOCK_IMAGE = 210,
    // ��ɱ�ǩ
    TAG_FINISH_IMAGE = 220,
    // ��ť��ǩ
    TAG_START_BUTTON = 300,
    // ��ͼ��ǩ
    TAG_PAGE_VIEW
};

class SelectScene : public cocos2d::Scene
{
private:
    // ����Ƿ�����
    bool isLock = false; // ��ӹؿ��Ƿ���ɵĽӿ�

    // ����Ƿ���ɹؿ�
    bool isFinish = false; // ��ӹؿ��Ƿ���ɵĽӿ�
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void scroll();

    void updateLevelState(int pageIndex);

    void gotoGameScene(Ref* sender);

    CREATE_FUNC(SelectScene);
};



#endif
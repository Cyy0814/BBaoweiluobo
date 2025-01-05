#ifndef __SELECT_SCENE_H__
#define __SELECT_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;
using namespace cocos2d::ui;

enum LevelTags {
    // 关卡标签
    TAG_LEVEL_1 = 100,
    TAG_LEVEL_2,
    TAG_LEVEL_3,
    TAG_LEVEL_4,
    TAG_LEVEL_5,
    // 页面标签
    TAG_LEVEL_IMAGE = 200,
    // 锁定标签
    TAG_LOCK_IMAGE = 210,
    // 完成标签
    TAG_FINISH_IMAGE = 220,
    // 按钮标签
    TAG_START_BUTTON = 300,
    // 视图标签
    TAG_PAGE_VIEW
};

class SelectScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void scroll();

    void updateLevelState(int pageIndex);

    void gotoGameScene(Ref* sender);

    CREATE_FUNC(SelectScene);
};
#endif
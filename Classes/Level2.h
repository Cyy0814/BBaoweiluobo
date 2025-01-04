#ifndef __LEVEL2_SCENE_H__
#define __LEVEL2_SCENE_H__

#include "BaseLevelScene.h"

class Level2Scene : public BaseLevelScene {
public:
    static Scene* createScene();
    virtual bool init() override;
    
    // 实现基类的纯虚函数
    virtual void initPath(GameMap* gameMap) override;
    virtual const char* getBgImagePath() override { return "Level/Level2_bg.png"; }
    virtual int getTotalWaves() override { return 15; }

    CREATE_FUNC(Level2Scene);
};

#endif
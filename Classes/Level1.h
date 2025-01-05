#ifndef __LEVEL1_SCENE_H__
#define __LEVEL1_SCENE_H__

#include "BaseLevelScene.h"

class Level1Scene : public BaseLevelScene {
public:
    static Scene* createScene();
    virtual bool init() override;
    
    // 基类纯虚函数的实现
    virtual void initPath(GameMap* gameMap) override;
    virtual const char* getBgImagePath() override { return "Level/Level1_bg.png"; }
    virtual int getTotalWaves() override { return 15; }
    virtual void setupWaves() override;  // 实现波数设置

    CREATE_FUNC(Level1Scene);
};

#endif
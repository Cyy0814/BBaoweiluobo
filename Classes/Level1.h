#ifndef __LEVEL1_SCENE_H__
#define __LEVEL1_SCENE_H__

#include "BaseLevelScene.h"

/*Refactored with Factory Pattern*/
/**
 * @brief 第一关场景类
 * 
 * 继承自BaseLevelScene,实现第一关特有的设置
 */
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
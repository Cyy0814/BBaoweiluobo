#ifndef __LEVEL2_SCENE_H__
#define __LEVEL2_SCENE_H__

#include "BaseLevelScene.h"

/*Refactored with Factory Pattern*/
/**
 * @brief 第二关场景类
 * 
 * 继承自BaseLevelScene,实现第二关特有的设置
 */
class Level2Scene : public BaseLevelScene {
public:
    static Scene* createScene();
    virtual bool init() override;
    
    // 实现基类的纯虚函数
    virtual void initPath(GameMap* gameMap) override;
    virtual const char* getBgImagePath() override { return "Level/Level2_bg.png"; }
    virtual int getTotalWaves() override { return 15; }
    virtual void setupWaves() override;  // 实现波数设置

    CREATE_FUNC(Level2Scene);
};

#endif
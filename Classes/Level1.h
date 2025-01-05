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
    /** @brief 创建场景静态方法 */
    static Scene* createScene();

    /** @brief 初始化方法 */
    virtual bool init() override;
    
    // 实现基类的纯虚函数
    /** @brief 设置第一关的路径 */
    virtual void initPath(GameMap* gameMap) override;

    /** @brief 获取第一关背景图片 */
    virtual const char* getBgImagePath() override { return "Level/Level1_bg.png"; }

    /** @brief 获取第一关总波数 */
    virtual int getTotalWaves() override { return 15; }

    CREATE_FUNC(Level1Scene);
};

#endif
#ifndef __BASE_LEVEL_SCENE_H__
#define __BASE_LEVEL_SCENE_H__

#include "cocos2d.h"
#include "Monster.h"
#include "GameScene.h"
#include "WaveManager.h"

using namespace cocos2d;
using namespace std;

//---------------------------------refactored with factory pattern---------------------------------//
/**
 * @brief 基础关卡场景类
 * 
 * 这是所有关卡场景的基类,包含了关卡的通用功能和接口
 */
class BaseLevelScene : public Scene {
public:
    virtual bool init() override;
    
    // 纯虚函数,子类必须实现
    /**
     * @brief 初始化关卡路径
     * @param gameMap 游戏地图对象
     * 每个关卡需要实现自己的路径设置
     */
    virtual void initPath(GameMap* gameMap) = 0;

    /**
     * @brief 获取关卡背景图片路径
     * @return 背景图片的资源路径
     */
    virtual const char* getBgImagePath() = 0;

    /**
     * @brief 获取关卡总波数
     * @return 怪物波数
     */
    virtual int getTotalWaves() = 0;
    
    // 通用功能实现
    /**
     * @brief 开始下一波怪物
     * @param dt 时间间隔
     */
    void startNextWave(float dt);

    /**
     * @brief 生成一波怪物
     * @param waveIndex 当前波数索引
     */
    void spawnMonsters(int waveIndex);

    /** @brief 结束游戏 */
    void endGame();

    /** @brief 更新函数 */
    void update(float dt);

    /**
     * @brief 获取路径点
     * @param gameMap 游戏地图对象
     */
    void getPath(GameMap* gameMap);

protected:
    cocos2d::Label* waveLabel;  // 波数显示标签
    int currentWave;            // 当前波数
    vector<Vec2> path;          // 怪物移动路径
    vector<Monster*> monsters;
    vector<Bottle*> bottles;
    vector<Fan*> fans;
    vector<Shit*> shits;
    WaveManager* waveManager;  // 波数管理器
    
    // 新增虚函数
    virtual void setupWaves() = 0;  // 设置波数的纯虚函数
};

#endif 
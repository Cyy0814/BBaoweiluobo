#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include <vector>
using namespace cocos2d;
using namespace std;

class GameScene : public Scene
{
private:
    LayerColor* maskLayer;      // 遮罩,用于处理暂停时对地图的操作
    bool isGamePaused = false;  // 游戏暂停状态
public:
    static Scene* createScene();

    virtual bool init();

    void options();

    void createCountdownAnimation();

    CREATE_FUNC(GameScene);
};

class GameMap : public Layer
{
public:
    // 初始化网格
    virtual bool init();

    // 关卡
    int level;

    // 网格大小 12X8
    static const int GRID_WIDTH = 12;
    static const int GRID_HEIGHT = 8;

    // 网格结构
    struct Grid {
        int x, y;
    };

    // 网格数组
    vector<vector<bool>> gridMap;

    // 存储路径
    vector<Grid> path;

    // 转换函数：网格坐标转换为像素坐标
    Vec2 gridToPixel(int x, int y);

    // 反转换：像素坐标转换为网格坐标
    Grid pixelToGrid(float x, float y);

    // 设置网格初始化并设置路径
    void setupGrid();
    void addPathPoint(Grid g);

    // 打印可放置防御塔的位置
    void printStartSprite();

    CREATE_FUNC(GameMap);
};

#endif
#ifndef __LEVEL1_SCENE_H__
#define __LEVEL1_SCENE_H__

#include "cocos2d.h"
#include "GameScene.h"
#include "Monster.h"
#include "Carrot.h"
#include "CarrotView.h"
#include "GoldCoin.h"
#include "GoldCoinView.h"

using namespace cocos2d;
using namespace std;

class Level1Scene : public Scene
{
public:
    static Scene* createScene();

    struct Grid {
        int x, y;
    };

    virtual bool init()override;
    void startNextWave(float dt);
    void spawnMonsters(int waveIndex);
    void endGame();

    /*表示波数*/
    cocos2d::Label* waveLabel; // 用于显示波数的标签
    int currentWave;           // 当前波数
    const int totalWaves = 15; // 总波数
    // 存储路径
    vector<Vec2> path;

    void getPath(GameMap* gamemap);

    void update(float dt);

    CREATE_FUNC(Level1Scene);
};
#endif
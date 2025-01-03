#ifndef __LEVEL2_SCENE_H__
#define __LEVEL2_SCENE_H__
#include "Monster.h"
#include "GameScene.h"
#include "Monster.h"
#include "Carrot.h"
#include "CarrotView.h"
#include "GoldCoin.h"
#include "GoldCoinView.h"

using namespace cocos2d;
using namespace std;

class Level2Scene : public Scene
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

    /*��ʾ����*/
    cocos2d::Label* waveLabel; // ������ʾ�����ı�ǩ
    int currentWave;           // ��ǰ����
    const int totalWaves = 15; // �ܲ���
    // �洢·��
    vector<Vec2> path;

    void getPath(GameMap* gamemap);

    void update(float dt);

    CREATE_FUNC(Level2Scene);
};
#endif
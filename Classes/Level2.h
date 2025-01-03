#ifndef __LEVEL2_SCENE_H__
#define __LEVEL2_SCENE_H__
#include "Monster.h"
#include "GameScene.h"
#include "cocos2d.h"
#include "WaveManager.h"

using namespace cocos2d;
using namespace std;

class Level2Scene : public Scene
{
public:
    static Scene* createScene();

    struct Grid {
        int x, y;
    };

    virtual bool init() override;
    void setupWaves();
    void endGame();
    void getPath(GameMap* gamemap);
    void update(float dt);

    cocos2d::Label* waveLabel;
    const int totalWaves = 15;
    vector<Vec2> path;

    CREATE_FUNC(Level2Scene);

private:
    WaveManager* waveManager;
};
#endif
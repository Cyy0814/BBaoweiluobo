#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include <vector>
using namespace cocos2d;
using namespace std;

class GameScene : public Scene
{
private:
    LayerColor* maskLayer;      // �ɰ�,�����������Ϊ��������ͣʱ�Ե�ͼ�Ĳ���
    bool isGamePaused = false;  // ������ͣ״̬
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
    // ��ʼ������
    virtual bool init();

    // ���������С 12X8
    static const int GRID_WIDTH = 12;
    static const int GRID_HEIGHT = 8;

    // ���ӽṹ
    struct Grid {
        int x, y;
    };

    // ��������
    vector<vector<bool>> gridMap;

    // �洢·��
    vector<Grid> path;

    // ת������������������ת��Ϊ��������
    Vec2 gridToPixel(int x, int y);

    // ��ת������������������ת��Ϊ��������
    Grid pixelToGrid(float x, float y);

    // ��������ʼ�������·��
    void setupGrid();
    void addPathPoint(int x, int y);

    // ��ӡ������̨��λ��
    void printStartSprite();

    CREATE_FUNC(GameMap);
};

#endif
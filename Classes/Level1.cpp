#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include <string.h>
#include "ui/CocosGUI.h"
#include "SelectScene.h"
#include"PickTower.h"
#include "Level1.h"
#include"Global.h"
#include "Monster.h"
#include <vector>
using namespace cocos2d;
using namespace cocos2d::ui;
PickTower* pickTower;
USING_NS_CC;

Scene* Level1Scene::createScene()
{
    return Level1Scene::create();
}

bool Level1Scene::init()
{
    /* ��ʼ������ */
    if (!Scene::init())
    {
        return false;
    }

    /* ��ȡ��Ļ��С */
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /* ���ñ��� */
    auto bg = Sprite::create("Level_1/Level_1_1_bg.png");
    bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(bg);

    /* ������Ϸ���� */
    this->addChild(GameScene::create());

    /* ��ӡ������̨��λ�� */
    auto gamemap = GameMap::create();
    this->addChild(gamemap);
    //*****************************//
   
    // ��ʼ��·���Ͳ���
    currentWave = 0;
    getPath(gamemap);

    globalCarrot = Carrot::create();
    globalCarrot->setPosition(path[path.size() - 1]);
    this->addChild(globalCarrot);
    // ��ʼ��һ��
    this->schedule(CC_SCHEDULE_SELECTOR(Level1Scene::startNextWave), 6.0f);

    return true;
}


void Level1Scene::getPath(GameMap* gamemap)
{
    // ����·������������
    for (const auto& grid : gamemap->path)
    {
        path.push_back(gamemap->gridToPixel(grid.x, grid.y));
    }
}





void Level1Scene::startNextWave(float dt) {
    if (currentWave >= totalWaves) {
        // ���в�����ɣ���Ϸ����
        unschedule(CC_SCHEDULE_SELECTOR(Level1Scene::startNextWave));
        endGame();
        return;
    }

    // ��ʼ��һ�����������
    spawnMonsters(currentWave);

    // ׼����һ��
    currentWave++;
}

void Level1Scene::spawnMonsters(int waveIndex) {
    // ���ݲ��ξ������ɹ�������ͺ�����
    int monsterType = (waveIndex % 3) + 1; // ������ÿ���л�һ�ֹ�������
    int monsterCount = 4 + (waveIndex % 3); // ������ÿ������������5��7֮��

    for (int i = 0; i < monsterCount; ++i) {
        // �������ﲢ����λ�õ�·�����
        auto monster = Monster::createWithType(monsterType);
        monster->setPosition(path.front()); // ����·���ĵ�һ���������
        this->addChild(monster);
        monster->setVisible(false);
        monsters.push_back(monster);

        // ���ù����ĳ���ʱ����
        this->scheduleOnce([this, monster](float dt) {
            monster->setVisible(true);
            monster->moveOnPath(this->path);
            }, i * 1.0f, "spawn_monster_" + std::to_string(i)); // ÿ��0.5�����һֻ����
    }
}

void Level1Scene::endGame() {
    // ������Ϸ�����߼�
    globalCarrot = nullptr;
    log("Game Over!");
}
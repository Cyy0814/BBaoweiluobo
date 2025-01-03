#include "SimpleAudioEngine.h"
#include <string.h>
#include "Global.h"
#include <vector>
#include "ui/CocosGUI.h"
#include "WelcomeScene.h"
#include "SelectScene.h"
#include "Monster.h"
#include "GameScene.h"
#include "Level2.h"
#include "Carrot.h"
#include "PickTower.h"
#include "GoldCoin.h"
#include "Bottle.h"
#include "Fan.h"
#include "Shit.h"
#include "WaveManager.h"
using namespace cocos2d;
using namespace cocos2d::ui;

USING_NS_CC;

Scene* Level2Scene::createScene()
{
    return Level2Scene::create();
}

bool Level2Scene::init()
{
    /* 初始化 */
    if (!Scene::init())
    {
        return false;
    }

    /* 获取屏幕大小 */
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /* 添加背景 */
    auto bg = Sprite::create("Level/Level2_bg.png");
    bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(bg);

    /* 添加游戏场景 */
    auto gamescene = GameScene::create();
    this->addChild(gamescene);

    /* 添加游戏地图 */
    auto gamemap = GameMap::create();
    this->addChild(gamemap);

    /* 初始化路径和波次 */
    getPath(gamemap);

    /* 初始化波次显示标签 */
    waveLabel = Label::createWithTTF("0  " + std::to_string(totalWaves), "fonts/arial.ttf", 28);
    waveLabel->setPosition(Vec2(visibleSize.width / 2 - 75, visibleSize.height - 35));
    waveLabel->setColor(Color3B::WHITE);
    this->addChild(waveLabel);

    /* 倒计时动画 */
    gamescene->createCountdownAnimation();

    //---------------------------------refactored with composite---------------------------------//
    /* 设置并开始波次 */
    waveManager = WaveManager::getInstance();
    setupWaves();
    
    // 6秒后开始第一波
    this->scheduleOnce([this](float dt) {
        waveManager->startAllWaves();
    }, 6.0f, "start_waves");

    this->scheduleUpdate();
    return true;
}

//---------------------------------refactored with composite---------------------------------//
void Level2Scene::setupWaves()
{
    for (int waveIndex = 0; waveIndex < totalWaves; ++waveIndex) {
        // 创建组合波次
        auto compositeWave = new CompositeWave();
        
        // Level2 的特殊波次设置：每波包含3个子波次，怪物更强
        for (int subWaveIndex = 0; subWaveIndex < 3; ++subWaveIndex) {
            auto singleWave = new SingleWave();
            
            // 创建该波次的怪物（Level2 的怪物更强）
            int monsterType = ((waveIndex + 1) % 3) + 1;
            int monsterCount = 5 + (waveIndex % 4); // 比 Level1 多一个怪物
            
            for (int i = 0; i < monsterCount; ++i) {
                auto monster = Monster::createWithType(monsterType);
                monster->setPosition(path.front());
                this->addChild(monster);
                monster->setVisible(false);
                
                // 设置怪物的路径
                monster->setMoveCallback([this, monster]() {
                    monster->moveOnPath(this->path);
                });
                
                singleWave->addMonster(monster);
            }
            
            // 将单一波次添加到组合波次
            compositeWave->addWave(singleWave);
        }
        
        // 将组合波次添加到管理器
        waveManager->addWave(compositeWave);
    }
}

void Level2Scene::getPath(GameMap* gamemap)
{
    // 路径遍历
    for (const auto& grid : gamemap->path)
    {
        path.push_back(gamemap->gridToPixel(grid.x, grid.y));
    }
}

void Level2Scene::endGame() {
    // 清理波次管理器
    waveManager->cleanup();
    
    // 清理游戏对象
    monsters.clear();
    bottles.clear();
    fans.clear();
    shits.clear();
    
    // 清理游戏对象
    globalCarrot = nullptr;
}

void Level2Scene::update(float dt) {
    // 遍历所有游戏对象
    if (monsters.size())
    {
        for (auto bottle : bottles) {
            bottle->update(dt, monsters);
        }
        for (auto fan : fans) {
            fan->update(dt, monsters);
        }
        for (auto shit : shits) {
            shit->update(dt, monsters);
        }
    }
}
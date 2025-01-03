#include "SimpleAudioEngine.h"
#include <string.h>
#include "Global.h"
#include <vector>
#include "ui/CocosGUI.h"
#include "WelcomeScene.h"
#include "SelectScene.h"
#include "Monster.h"
#include "GameScene.h"
#include "Level1.h"
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

Scene* Level1Scene::createScene()
{
    return Level1Scene::create();
}

bool Level1Scene::init()
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
    auto bg = Sprite::create("Level/Level1_bg.png");
    bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(bg);

    /* 添加游戏场景 */
    auto gamescene = GameScene::create();
    this->addChild(gamescene);

    /* 添加游戏地图 */
    auto gamemap = GameMap::create();
    this->addChild(gamemap);

    /* 初始化路径 */
    vector<GameMap::Grid>path_ = {
    { 1,5 },
    { 1,4 },
    { 1,3 },
    { 1,2 },
    { 2,2 },
    { 3,2 },
    { 4,2 },
    { 4,3 },
    { 5,3 },
    { 6,3 },
    { 7,3 },
    { 7,2 },
    { 8,2 },
    { 9,2 },
    { 10,2 },
    { 10,3 },
    { 10,4 },
    { 10,5 } };
    for (int i = 0; i < path_.size(); i++) {
        gamemap->addPathPoint(path_[i]);
    }

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

void Level1Scene::getPath(GameMap* gamemap)
{
    // 路径遍历
    for (const auto& grid : gamemap->path)
    {
        path.push_back(gamemap->gridToPixel(grid.x, grid.y));
    }
}

//---------------------------------refactored with composite---------------------------------//
void Level1Scene::setupWaves()
{
    for (int waveIndex = 0; waveIndex < totalWaves; ++waveIndex) {
        // 创建组合波次
        auto compositeWave = new CompositeWave();
        
        // 根据波次索引创建不同类型的怪物波次
        for (int subWaveIndex = 0; subWaveIndex < 2; ++subWaveIndex) {
            auto singleWave = new SingleWave();
            
            // 创建该波次的怪物
            int monsterType = (waveIndex % 3) + 1;
            int monsterCount = 4 + (waveIndex % 3);
            
            for (int i = 0; i < monsterCount; ++i) {
                auto monster = Monster::createWithType(monsterType);
                monster->setPosition(path.front());
                this->addChild(monster);
                monster->setVisible(false);
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

void Level1Scene::endGame() {
    // 清理波次管理器
    waveManager->cleanup();
    
    // 清理游戏对象
    monsters.clear();
    bottles.clear();
    fans.clear();
    shits.clear();
    /* 显示胜利或失败提示 */
    // 胜利
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 150));
    this->addChild(maskLayer, 6);

    // 显示提示
    ImageView* tips;
    if (globalCarrot->health <= 0) {
        tips = ImageView::create("GameScene/lose.png");
    }
    else {
        tips = ImageView::create("GameScene/win.png");
    }
    
    tips->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    maskLayer->addChild(tips, 2);

    // 重新开始按钮
    auto restartButton = ui::Button::create("GameScene/again_normal.png", "GameScene/again_selected.png");
    restartButton->setPosition(Vec2(210, 185));
    tips->addChild(restartButton);
    restartButton->addClickEventListener([=](Ref*) {
        Director::getInstance()->resume();
        Director::getInstance()->getScheduler()->setTimeScale(1);

        // 重新开始游戏
        auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 0));  // 清除遮罩层
        this->addChild(maskLayer);
        float duration = 0.1f; // 动画持续时间
        float targetY = visibleSize.height; // 目标位置的Y坐标

        auto moveUp = MoveTo::create(duration, Vec2(0, targetY));
        auto callback = CallFunc::create([]() {
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, Level1Scene::create(), Color3B::BLACK)); // 淡入淡出效果
            });
        auto sequence = Sequence::create(moveUp, callback, nullptr);
        maskLayer->runAction(sequence);
        });

    // 返回按钮
    auto returnButton = ui::Button::create("GameScene/return_normal.png", "GameScene/return_selected.png");
    returnButton->setPosition(Vec2(210, 90));
    tips->addChild(returnButton);
    returnButton->addClickEventListener([=](Ref*) {
        Director::getInstance()->resume();
        Director::getInstance()->getScheduler()->setTimeScale(1);

        // 返回选择场景
        auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 0));  // 清除遮罩层
        this->addChild(maskLayer);
        float duration = 0.1f; // 动画持续时间
        float targetY = visibleSize.height; // 目标位置的Y坐标

        auto moveUp = MoveTo::create(duration, Vec2(0, targetY));
        auto callback = CallFunc::create([]() {
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, SelectScene::create(), Color3B::BLACK)); // 淡入淡出效果
            });
        auto sequence = Sequence::create(moveUp, callback, nullptr);
        maskLayer->runAction(sequence);
        });

    /* 游戏暂停逻辑 */
    // 游戏暂停
    Director::getInstance()->pause();

    // 确保玩家可以触摸到遮罩层，否则无法触发事件
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](cocos2d::Touch* touch, cocos2d::Event* event) {
        return true; // 允许触发事件
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, maskLayer);

    /* 全局变量重置 */
    globalCarrot = nullptr;
}

void Level1Scene::update(float dt) {
    // 更新所有怪物
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
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
using namespace cocos2d;
using namespace cocos2d::ui;

USING_NS_CC;

Scene* Level1Scene::createScene()
{
    return Level1Scene::create();
}

bool Level1Scene::init()
{
    /* 初始化场景 */
    if (!Scene::init())
    {
        return false;
    }

    /* 获取屏幕大小 */
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /* 设置背景 */
    auto bg = Sprite::create("Level/Level1_bg.png");
    bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(bg);

    /* 设置游戏画面 */
    auto gamescene = GameScene::create();
    this->addChild(gamescene);

    /* 打印放置炮台的位置 */
    auto gamemap = GameMap::create();
    this->addChild(gamemap);

    /* 初始化网格路径 */
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

    /* 初始化像素路径和波次 */
    currentWave = 0;
    getPath(gamemap);

    /* 倒计时后进行设置 */
    auto delay = DelayTime::create(3.0f);
    auto createCarrot = CallFunc::create([this]() {
        /* 设置起点 */
        auto startPoint = Sprite::create("Level/startPoint.PNG");
        startPoint->setPosition(path[0]);
        this->addChild(startPoint);
        });
    auto sequence = Sequence::create(delay, createCarrot, nullptr);
    runAction(sequence);

    /* 初始化萝卜 */
    globalCarrot = Carrot::create();
    globalCarrot->setPosition(path[path.size() - 1]);
    globalCarrotView = CarrotView::create(globalCarrot);
    this->addChild(globalCarrot);
    this->addChild(globalCarrotView);

    /* 创建点击动作 */
    std::vector<Vec2> positions; // 存储可以放置炮台的位置
    for (int y = 0; y < gamemap->GRID_HEIGHT - 1; ++y) {
        for (int x = 0; x < gamemap->GRID_WIDTH; ++x) {
            if (gamemap->gridMap[y][x]) {
                // 将位置添加到 positions 向量
                positions.push_back(gamemap->gridToPixel(x, y));
            }
        }
    }
    // 创建并初始化 PickTower
    auto pickTower = PickTower::createWithPositions(positions);
    this->addChild(pickTower);

    /* 初始化波数显示标签 */
    currentWave = 0;
    waveLabel = Label::createWithTTF(std::to_string(currentWave) + "  " + std::to_string(totalWaves), "fonts/arial.ttf", 28);
    waveLabel->setPosition(Vec2(visibleSize.width / 2 - 75, visibleSize.height - 35));
    waveLabel->setColor(Color3B::WHITE); // 设置颜色为白色
    this->addChild(waveLabel);

    /* 倒计时动画 */
    gamescene->createCountdownAnimation();

    /* 开始第一波 */
    this->schedule(CC_SCHEDULE_SELECTOR(Level1Scene::startNextWave), 6.0f);

    this->scheduleUpdate();
    return true;
}

void Level1Scene::getPath(GameMap* gamemap)
{
    // 计算路径的像素坐标
    for (const auto& grid : gamemap->path)
    {
        path.push_back(gamemap->gridToPixel(grid.x, grid.y));
    }
}

void Level1Scene::startNextWave(float dt) {
    if (currentWave >= totalWaves) {
        // 所有波次完成，游戏结束
        unschedule(CC_SCHEDULE_SELECTOR(Level1Scene::startNextWave));
        isLevel1Finish = true;
        endGame();
        return;
    }

    // 开始下一波怪物的生成
    spawnMonsters(currentWave);

    // 准备下一波
    currentWave++;
    waveLabel->setString(std::to_string(currentWave) + "  " + std::to_string(totalWaves));
}

void Level1Scene::spawnMonsters(int waveIndex) {
    // 根据波次决定生成怪物的类型和数量
    int monsterType = (waveIndex % 3) + 1; // 举例：每波切换一种怪物类型
    int monsterCount = 4 + (waveIndex % 3); // 举例：每波怪物数量在5到7之间

    for (int i = 0; i < monsterCount; ++i) {
        // 创建怪物并设置位置到路径起点
        auto monster = Monster::createWithType(monsterType);
        monster->setPosition(path.front()); // 假设路径的第一个点是起点
        this->addChild(monster);
        monster->setVisible(false);
        monsters.push_back(monster);
        
        // 设置怪物间的出现时间间隔
        this->scheduleOnce([this, monster](float dt) {
            // 萝卜血量先为0，则游戏结束
            if (globalCarrot->health <= 0) {
                monster->cleanup();
                endGame();
                return;
            }
            // 出现特效
            auto appear = cocos2d::Sprite::create("Monster/appear.PNG");
            appear->setPosition(path.front());
            this->addChild(appear);

            // 设置一个动作来移除出现特效
            auto fadeOut = cocos2d::FadeOut::create(0.5f);
            auto removeExplosion = cocos2d::RemoveSelf::create();
            auto sequence = cocos2d::Sequence::create(fadeOut, removeExplosion, nullptr);
            appear->runAction(sequence);

            monster->setVisible(true);
            monster->moveOnPath(this->path);
            
            }, i * 1.0f, "spawn_monster_" + std::to_string(i)); // 每隔1.0秒出现一只怪物
    }
}

void Level1Scene::endGame() {
    // 处理游戏结束逻辑
    /********1/2 更新 置零*/
    monsters.clear();
    bottles.clear();
    fans.clear();
    shits.clear();
    /* 出现获胜提示框 */
    // 蒙版层
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 150));
    this->addChild(maskLayer, 6);

    // 创建提示框
    ImageView* tips;
    if (globalCarrot->health <= 0) {
        tips = ImageView::create("GameScene/lose.png");
    }
    else {
        tips = ImageView::create("GameScene/win.png");
    }
    
    tips->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    maskLayer->addChild(tips, 2);

    // 以下按钮都添加在 tips 层上 
    // 再试一次按钮
    auto restartButton = ui::Button::create("GameScene/again_normal.png", "GameScene/again_selected.png");
    restartButton->setPosition(Vec2(210, 185));
    tips->addChild(restartButton);
    restartButton->addClickEventListener([=](Ref*) {
        Director::getInstance()->resume();
        Director::getInstance()->getScheduler()->setTimeScale(1);

        // 重新开始游戏
        auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 0));  // 透明的遮罩层
        this->addChild(maskLayer);
        float duration = 0.1f; // 动画的持续时间
        float targetY = visibleSize.height; // 目标位置的Y坐标

        auto moveUp = MoveTo::create(duration, Vec2(0, targetY));
        auto callback = CallFunc::create([]() {
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, Level1Scene::create(), Color3B::BLACK)); // 切换到新场景
            });
        auto sequence = Sequence::create(moveUp, callback, nullptr);
        maskLayer->runAction(sequence);
        });

    // 选择关卡按钮
    auto returnButton = ui::Button::create("GameScene/return_normal.png", "GameScene/return_selected.png");
    returnButton->setPosition(Vec2(210, 90));
    tips->addChild(returnButton);
    returnButton->addClickEventListener([=](Ref*) {
        Director::getInstance()->resume();
        Director::getInstance()->getScheduler()->setTimeScale(1);

        // 返回选择界面
        auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 0));  // 透明的遮罩层
        this->addChild(maskLayer);
        float duration = 0.1f; // 动画的持续时间
        float targetY = visibleSize.height; // 目标位置的Y坐标

        auto moveUp = MoveTo::create(duration, Vec2(0, targetY));
        auto callback = CallFunc::create([]() {
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, SelectScene::create(), Color3B::BLACK)); // 切换到新场景
            });
        auto sequence = Sequence::create(moveUp, callback, nullptr);
        maskLayer->runAction(sequence);
        });

    /* 游戏暂停逻辑 */
    // 游戏暂停
    Director::getInstance()->pause();

    // 确保用户不能点击暂停菜单后面的元素，可以通过添加一个监听器到暂停菜单层来吞噬所有点击事件
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](cocos2d::Touch* touch, cocos2d::Event* event) {
        return true; // 吞噬触摸事件
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, maskLayer);

    /* 让萝卜的指针归零 */
    globalCarrot = nullptr;
}

void Level1Scene::update(float dt) {
    // 更新场景中的所有炮塔
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
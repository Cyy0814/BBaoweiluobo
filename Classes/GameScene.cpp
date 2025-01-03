#include "SimpleAudioEngine.h"
#include <string.h>
#include "Global.h"
#include "GameScene.h"
#include "GoldCoin.h"
#include "GoldCoinView.h"
#include "SelectScene.h"
#include "Level1.h"
using namespace cocos2d;
using namespace cocos2d::ui;

USING_NS_CC;
int level = 1;

/* -------------------- 地图公共场景搭建 ---------------------- */
Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    /* 初始化场景 */
    if (!Scene::init())
    {
        return false;
    }

    /* 获取屏幕大小 */
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /* 设置菜单栏 */
    auto menu = Sprite::create("GameScene/menu.PNG");
    menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 38));
    this->addChild(menu);

    /* 设置剩余波数 */
    auto gameover = Sprite::create("GameScene/gameover.PNG");
    gameover->setPosition(Vec2(visibleSize.width / 2 - 80, visibleSize.height - 33));
    this->addChild(gameover);

    /* 显示金币数量 */
    // 创建 GoldCoin 对象
    goldCoin = GoldCoin::create();
    goldCoinView = GoldCoinView::create(goldCoin);

    if (goldCoin && goldCoin->init())
    {
        // 将 GoldCoin 和 GoldCoinView 添加到场景中
        this->addChild(goldCoin);
        this->addChild(goldCoinView);

        // 假设当前金币数量为 500
        int currentGold = 500;

        // 更新金币数量显示
        goldCoin->updateGoldValue(currentGold);
    }

    /* 倍速按钮 */
    auto speedButton = Button::create("GameScene/speed_button.PNG");
    speedButton->setPosition(Vec2(visibleSize.width / 2 + 150, visibleSize.height - 38));
    this->addChild(speedButton, 1);
    speedButton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED)
        {
            static bool isDoubleSpeed = false;
            isDoubleSpeed = !isDoubleSpeed; // 切换速度状态

            float speed = isDoubleSpeed ? 2.0f : 1.0f; // 根据状态选择速度
            Director::getInstance()->getScheduler()->setTimeScale(speed);

            // 更新按钮的图案
            std::string buttonImage = isDoubleSpeed ? "GameScene/speed_button_selected.PNG" : "GameScene/speed_button.PNG";
            speedButton->loadTextureNormal(buttonImage);
        }
        }); 

    /* 暂停按钮 */
    auto pauseButton = Button::create("GameScene/pause_button.PNG");
    pauseButton->setPosition(Vec2(visibleSize.width / 2 + 300, visibleSize.height - 38));
    this->addChild(pauseButton);
    pauseButton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            // 切换暂停状态
            isGamePaused = !isGamePaused; 
            if (isGamePaused) {
                Director::getInstance()->pause(); // 暂停游戏
                // 切换到恢复图标
                pauseButton->loadTextures("GameScene/pause_button_selected.PNG", "GameScene/pause_button_selected_light.PNG");
            }
            else {
                Director::getInstance()->resume(); // 恢复游戏
                // 切换到暂停图标
                pauseButton->loadTextures("GameScene/pause_button.PNG", "GameScene/pause_button.PNG");
            }
        }
        });
    /* 选项按钮 */
    GameScene::options();

    return true;
}

// 实现选项按钮下的菜单
void GameScene::options()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    /* 选项按钮 */
    auto choiceButton = Button::create("GameScene/choice_button.PNG");
    choiceButton->setPosition(Vec2(visibleSize.width / 2 + 400, visibleSize.height - 38));
    this->addChild(choiceButton, 1);
    choiceButton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED)
        {
            /* 点击后出现提示框 */
            // 类蒙版层
            maskLayer = LayerColor::create(Color4B(0, 0, 0, 150));
            this->getParent()->addChild(maskLayer, 6);

            // 创建提示框
            auto options = ImageView::create("GameScene/options.png");
            options->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
            maskLayer->addChild(options, 2);

            // 以下按钮都添加在 options 层上 
            // 重新开始按钮
            auto restartButton = ui::Button::create("GameScene/restart_normal.png", "GameScene/restart_selected.png");
            restartButton->setPosition(Vec2(210, 185));
            options->addChild(restartButton);
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
            options->addChild(returnButton);
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

            // 继续游戏按钮
            auto resumeButton = Button::create("GameScene/resume_normal.png", "GameScene/resume_selected.png");
            resumeButton->setPosition(Vec2(210, 278));
            options->addChild(resumeButton);
            resumeButton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
                if (type == Widget::TouchEventType::ENDED)
                {
                    // 恢复游戏
                    Director::getInstance()->resume();
                    // 移除暂停菜单
                    options->removeFromParent();
                    // 移除蒙版层
                    maskLayer->removeFromParent();
                }
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
            Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this->maskLayer);
        }
        });
}

// 创建倒计时动画的函数
void GameScene::createCountdownAnimation()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    // 创建类内蒙版层
    maskLayer = LayerColor::create(Color4B(0, 0, 0, 150));
    this->getParent()->addChild(maskLayer, 5); // 确保蒙版层的层级高于其他元素

    // 确保用户不能点击倒计时后面的元素，可以通过添加一个监听器到暂停菜单层来吞噬所有点击事件
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](cocos2d::Touch* touch, cocos2d::Event* event) {
        return true; // 吞噬触摸事件
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this->maskLayer);

    // 创建倒计时圆环
    auto circlebase = Sprite::create("GameScene/circlebase.PNG");
    circlebase->setPosition(visibleSize / 2);
    maskLayer->addChild(circlebase);
    auto circleSprite = Sprite::create("GameScene/circle.PNG");
    circleSprite->setPosition(Vec2(visibleSize.width / 2 + 5, visibleSize.height / 2));
    maskLayer->addChild(circleSprite);

    // 初始化倒计时数字
    auto countdownSprite = Sprite::create("GameScene/3.PNG"); // 替换为您的初始倒计时数字图片
    countdownSprite->setPosition(circleSprite->getPosition());
    maskLayer->addChild(countdownSprite);

    // 圆环每秒钟完成一次循环
    circleSprite->runAction(RepeatForever::create(RotateBy::create(1.0f, 360)));

    // 倒计时开始，每秒更新一次数字
    int countdownNumber = 3; // 从3开始倒计时
    auto scheduler = Director::getInstance()->getScheduler();
    scheduler->schedule([=](float dt) mutable {
        countdownNumber--;
        // 更新倒计时数字精灵的纹理
        std::string frameName = "GameScene/" + std::to_string(countdownNumber) + ".PNG";
        countdownSprite->setTexture(frameName);

        // 当倒计时结束时
        if (countdownNumber <= 0) {
            scheduler->unschedule("countdown", this); // 取消倒计时调度
            countdownSprite->setVisible(false); // 隐藏数字精灵
            // 倒计时结束后
            maskLayer->removeFromParent();
        }
      
        }, this, 1.0f, false, "countdown");
}

/* -------------------- 对地图网格化实现 ---------------------- */
bool GameMap::init()
{
    if (!Layer::init()) {
        return false;
    }
    setupGrid(); // 设置网格

    // 设置倒计时以匹配动画
    int countdownNumber = 3; // 从3开始倒计时
    auto scheduler = Director::getInstance()->getScheduler();
    scheduler->schedule([=](float dt) mutable {
        countdownNumber--;
        if (countdownNumber <= 0) {
            // 打印框格
            GameMap::printStartSprite();

        }
        }, this, 1.0f, false, "countdown");

    return true;
}

void GameMap::setupGrid() {
    gridMap.resize(GRID_HEIGHT, vector<bool>(GRID_WIDTH));

    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            // 初始化每个格子
            if (y == 7)
                gridMap[y][x] = false;
            else
                gridMap[y][x] = true;
        }
    }
}

void GameMap::addPathPoint(Grid g) {
    // 确保坐标在地图范围内
    if (g.x >= 0 && g.x < GRID_WIDTH && g.y >= 0 && g.y < GRID_HEIGHT) {
        path.push_back(g);
        gridMap[g.y][g.x] = false;
    }
}

Vec2 GameMap::gridToPixel(int x, int y) {
    float pixelX = x * (this->getContentSize().width / GRID_WIDTH) + (this->getContentSize().width / GRID_WIDTH) / 2;
    float pixelY = y * (this->getContentSize().height / GRID_HEIGHT) + (this->getContentSize().height / GRID_HEIGHT) / 2;
    return Vec2(pixelX, pixelY);
}

GameMap::Grid GameMap::pixelToGrid(float x, float y) {
    int gridX = static_cast<int>(x / (this->getContentSize().width / GRID_WIDTH));
    int gridY = static_cast<int>(y / (this->getContentSize().height / GRID_HEIGHT));
    return { gridX, gridY };
}

void GameMap::printStartSprite()
{
    // 创建可以放置炮台的位置
    for (int y = 0; y < GRID_HEIGHT - 1; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            if (gridMap[y][x]) {
                // 打印格子
                auto StartSprite = ImageView::create("GameScene/StartSprite.png");
                StartSprite->setPosition(gridToPixel(x, y));
                this->addChild(StartSprite);
            }
        }
    }

    // 创建一个闪烁动作
    auto blinkAction = cocos2d::Blink::create(2, 4);

    // 创建一个回调动作，在闪烁结束后移除图层
    auto removeLayer = CallFunc::create([this]() {
        this->removeFromParent();
        });

    // 创建一个动作序列，先闪烁，后移除
    auto sequence = Sequence::create(blinkAction, removeLayer, nullptr);

    // 运行动作序列
    this->runAction(sequence);

}
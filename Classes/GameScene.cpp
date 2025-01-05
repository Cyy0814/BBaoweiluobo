#include <string.h>
#include "Global.h"
#include "GameScene.h"
#include "GoldCoin.h"
#include "GoldCoinView.h"
#include "SelectScene.h"
#include "Level1.h"
#include "LevelFactory.h"
using namespace cocos2d;
using namespace cocos2d::ui;

USING_NS_CC;
int level = 1;

/* -------------------- 场景 ---------------------- */
Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    /* 初始化 */
    if (!Scene::init())
    {
        return false;
    }

    /* 获取屏幕大小 */
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /* 设置菜单 */
    auto menu = Sprite::create("GameScene/menu.PNG");
    menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 38));
    this->addChild(menu);

    /* 剩余生命 */
    auto gameover = Sprite::create("GameScene/gameover.PNG");
    gameover->setPosition(Vec2(visibleSize.width / 2 - 80, visibleSize.height - 33));
    this->addChild(gameover);

    /* 金币显示 */
    //将 GoldCoin 和 GoldCoinView 添加到场景中
    goldCoin = GoldCoin::create();
    goldCoinView = GoldCoinView::create(goldCoin);

    if (goldCoin && goldCoin->init())
    {
        //  GoldCoin 添加到场景中
        this->addChild(goldCoin);
        this->addChild(goldCoinView);

        // 当前金币为 500
        int currentGold = 500;

        // 更新显示
        goldCoin->updateGoldValue(currentGold);
    }

    /* 速度按钮 */
    auto speedButton = Button::create("GameScene/speed_button.PNG");
    speedButton->setPosition(Vec2(visibleSize.width / 2 + 150, visibleSize.height - 38));
    this->addChild(speedButton, 1);
    speedButton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED)
        {
            static bool isDoubleSpeed = false;
            isDoubleSpeed = !isDoubleSpeed; // 切换加速状态

            float speed = isDoubleSpeed ? 2.0f : 1.0f; // 根据状态选择速度
            Director::getInstance()->getScheduler()->setTimeScale(speed);

            // 切换按钮图标
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
                // 切换恢复图标
                pauseButton->loadTextures("GameScene/pause_button_selected.PNG", "GameScene/pause_button_selected_light.PNG");
            }
            else {
                Director::getInstance()->resume(); // 恢复游戏
                // 切换暂停图标
                pauseButton->loadTextures("GameScene/pause_button.PNG", "GameScene/pause_button.PNG");
            }
        }
        });
    /* 选项按钮 */
    GameScene::options();

    return true;
}

// 选项按钮的回调函数
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
            /* 创建遮罩层 */
            // 创建遮罩层
            maskLayer = LayerColor::create(Color4B(0, 0, 0, 150));
            this->getParent()->addChild(maskLayer, 6);

            // 显示选项界面
            auto options = ImageView::create("GameScene/options.png");
            options->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
            maskLayer->addChild(options, 2);

            // 选项按钮
            // 重启按钮
            auto restartButton = ui::Button::create("GameScene/restart_normal.png", "GameScene/restart_selected.png");
            restartButton->setPosition(Vec2(210, 185));
            options->addChild(restartButton);
            restartButton->addClickEventListener([=](Ref*) {

                //---------------------------------refactored with factory pattern---------------------------------//

                auto level = LevelFactory::createLevel(currentLevel);
                if (level) {
                    Director::getInstance()->replaceScene(
                        TransitionFade::create(0.5f, level, Color3B::BLACK)
                    );
                }
                
            });

            // 返回按钮
            auto returnButton = ui::Button::create("GameScene/return_normal.png", "GameScene/return_selected.png");
            returnButton->setPosition(Vec2(210, 90));
            options->addChild(returnButton);
            returnButton->addClickEventListener([=](Ref*) {
                Director::getInstance()->resume();
                Director::getInstance()->getScheduler()->setTimeScale(1);

                // 创建遮罩层
                auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 0));  // 创建遮罩层
                this->addChild(maskLayer);
                float duration = 0.1f; // 持续时间
                float targetY = visibleSize.height; // 目标位置Y

                auto moveUp = MoveTo::create(duration, Vec2(0, targetY));
                auto callback = CallFunc::create([]() {
                    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, SelectScene::create(), Color3B::BLACK)); // 切换场景
                    });
                auto sequence = Sequence::create(moveUp, callback, nullptr);
                maskLayer->runAction(sequence);
                });

            // 恢复按钮
            auto resumeButton = Button::create("GameScene/resume_normal.png", "GameScene/resume_selected.png");
            resumeButton->setPosition(Vec2(210, 278));
            options->addChild(resumeButton);
            resumeButton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
                if (type == Widget::TouchEventType::ENDED)
                {
                    // 恢复游戏
                    Director::getInstance()->resume();
                    // 移除选项界面
                    options->removeFromParent();
                    // 移除遮罩层
                    maskLayer->removeFromParent();
                }
                });

            /* 暂停游戏 */
            // 暂停游戏
            Director::getInstance()->pause();

            // 确保所有事件监听器都已添加
            auto listener = EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);
            listener->onTouchBegan = [](cocos2d::Touch* touch, cocos2d::Event* event) {
                return true; // 返回true表示事件已处理
            };
            Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this->maskLayer);
        }
        });
}

// 创建倒计时动画
void GameScene::createCountdownAnimation()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    // 创建遮罩层
    maskLayer = LayerColor::create(Color4B(0, 0, 0, 150));
    this->getParent()->addChild(maskLayer, 5); // 确保遮罩层已添加

    // 确保所有事件监听器都已添加
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](cocos2d::Touch* touch, cocos2d::Event* event) {
        return true; // 返回true表示事件已处理
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this->maskLayer);

    // 创建圆圈
    auto circlebase = Sprite::create("GameScene/circlebase.PNG");
    circlebase->setPosition(visibleSize / 2);
    maskLayer->addChild(circlebase);
    auto circleSprite = Sprite::create("GameScene/circle.PNG");
    circleSprite->setPosition(Vec2(visibleSize.width / 2 + 5, visibleSize.height / 2));
    maskLayer->addChild(circleSprite);

    // 创建倒计时数字
    auto countdownSprite = Sprite::create("GameScene/3.PNG"); // 替换为实际的倒计时图片
    countdownSprite->setPosition(circleSprite->getPosition());
    maskLayer->addChild(countdownSprite);

    // 圆圈旋转
    circleSprite->runAction(RepeatForever::create(RotateBy::create(1.0f, 360)));

    // 倒计时每秒更新
    int countdownNumber = 3; // 3秒倒计时
    auto scheduler = Director::getInstance()->getScheduler();
    scheduler->schedule([=](float dt) mutable {
        countdownNumber--;
        // 更新倒计时数字
        std::string frameName = "GameScene/" + std::to_string(countdownNumber) + ".PNG";
        countdownSprite->setTexture(frameName);

        // 倒计时结束
        if (countdownNumber <= 0) {
            scheduler->unschedule("countdown", this); // 移除倒计时
            countdownSprite->setVisible(false); // 隐藏倒计时数字
            // 移除遮罩层
            maskLayer->removeFromParent();
        }
      
        }, this, 1.0f, false, "countdown");
}

/* -------------------- 游戏地图 ---------------------- */
bool GameMap::init()
{
    if (!Layer::init()) {
        return false;
    }
    setupGrid(); // 

    // 倒计时
    int countdownNumber = 3; // 3秒倒计时
    auto scheduler = Director::getInstance()->getScheduler();
    scheduler->schedule([=](float dt) mutable {
        countdownNumber--;
        if (countdownNumber <= 0) {
            // 打印开始精灵
            GameMap::printStartSprite();

        }
        }, this, 1.0f, false, "countdown");

    return true;
}

void GameMap::setupGrid() {
    gridMap.resize(GRID_HEIGHT, vector<bool>(GRID_WIDTH));

    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            // 初始化
            if (y == 7)
                gridMap[y][x] = false;
            else
                gridMap[y][x] = true;
        }
    }
}

void GameMap::addPathPoint(Grid g) {
    // 确保点在地图范围内
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
    // 打印开始精灵
    for (int y = 0; y < GRID_HEIGHT - 1; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            if (gridMap[y][x]) {
                // 打印
                auto StartSprite = ImageView::create("GameScene/StartSprite.png");
                StartSprite->setPosition(gridToPixel(x, y));
                this->addChild(StartSprite);
            }
        }
    }

    // 闪烁效果
    auto blinkAction = cocos2d::Blink::create(2, 4);

    // 移除遮罩层
    auto removeLayer = CallFunc::create([this]() {
        this->removeFromParent();
        });

    // 创建闪烁效果
    auto sequence = Sequence::create(blinkAction, removeLayer, nullptr);

    // 执行
    this->runAction(sequence);

}

//---------------------------------refactored with factory pattern---------------------------------//
void GameScene::restartLevel() {
    // 使用工厂重新创建当前关卡
    auto level = LevelFactory::createLevel(currentLevel);
    if (level) {
        Director::getInstance()->replaceScene(
            TransitionFade::create(0.5f, level, Color3B::BLACK)
        );
    }
}
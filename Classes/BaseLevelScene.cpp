#include "BaseLevelScene.h"
#include "GameScene.h"
#include "SelectScene.h"
#include "ui/CocosGUI.h"
#include "Carrot.h"
#include "PickTower.h"
#include "Global.h"

bool BaseLevelScene::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 设置关卡背景
    auto bg = Sprite::create(getBgImagePath());
    bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(bg);

    // 添加游戏主场景
    auto gameScene = GameScene::create();
    this->addChild(gameScene);

    // 初始化游戏地图
    auto gameMap = GameMap::create();
    this->addChild(gameMap);

    // 初始化关卡设置
    currentWave = 0;
    initPath(gameMap);    // 设置路径
    getPath(gameMap);     // 获取路径点

    /* 创建起点标记 */
    auto delay = DelayTime::create(3.0f);
    auto createStartPoint = CallFunc::create([this]() {
        auto startPoint = Sprite::create("Level/startPoint.PNG");
        startPoint->setPosition(path[0]);
        this->addChild(startPoint);
    });
    auto sequence = Sequence::create(delay, createStartPoint, nullptr);
    runAction(sequence);

    // 初始化萝卜(玩家基地)
    globalCarrot = Carrot::create();
    globalCarrot->setPosition(path[path.size() - 1]);
    this->addChild(globalCarrot);

    // 设置防御塔位置
    std::vector<Vec2> positions;
    for (int y = 0; y < gameMap->GRID_HEIGHT - 1; ++y) {
        for (int x = 0; x < gameMap->GRID_WIDTH; ++x) {
            if (gameMap->gridMap[y][x]) {
                positions.push_back(gameMap->gridToPixel(x, y));
            }
        }
    }
    auto pickTower = PickTower::createWithPositions(positions);
    this->addChild(pickTower);

    // 设置波数显示
    waveLabel = Label::createWithTTF("0  " + std::to_string(getTotalWaves()), 
                                   "fonts/arial.ttf", 28);
    waveLabel->setPosition(Vec2(visibleSize.width / 2 - 75, visibleSize.height - 35));
    waveLabel->setColor(Color3B::WHITE);
    this->addChild(waveLabel);

    // 播放倒计时动画
    gameScene->createCountdownAnimation();

    // 设置游戏循环
    this->schedule(CC_SCHEDULE_SELECTOR(BaseLevelScene::startNextWave), 6.0f);
    this->scheduleUpdate();

    return true;
}

void BaseLevelScene::getPath(GameMap* gameMap) {
    for (const auto& grid : gameMap->path) {
        path.push_back(gameMap->gridToPixel(grid.x, grid.y));
    }
}

void BaseLevelScene::startNextWave(float dt) {
    if (currentWave >= getTotalWaves()) {
        unschedule(CC_SCHEDULE_SELECTOR(BaseLevelScene::startNextWave));
        if (dynamic_cast<Level1Scene*>(this)) {
            isLevel1Finish = true;
        } else if (dynamic_cast<Level2Scene*>(this)) {
            isLevel2Finish = true;
        }
        endGame();
        return;
    }

    spawnMonsters(currentWave);
    currentWave++;
    waveLabel->setString(std::to_string(currentWave) + "  " + std::to_string(getTotalWaves()));
}

void BaseLevelScene::spawnMonsters(int waveIndex) {
    int monsterType = (waveIndex % 3) + 1;
    int monsterCount = 4 + (waveIndex % 3);

    for (int i = 0; i < monsterCount; ++i) {
        auto monster = Monster::createWithType(monsterType);
        monster->setPosition(path.front());
        this->addChild(monster);
        monster->setVisible(false);
        monsters.push_back(monster);

        this->scheduleOnce([this, monster](float dt) {
            if (globalCarrot->health <= 0) {
                monster->cleanup();
                endGame();
            }
            
            auto appear = Sprite::create("Monster/appear.PNG");
            appear->setPosition(path.front());
            this->addChild(appear);

            auto fadeOut = FadeOut::create(0.5f);
            auto removeExplosion = RemoveSelf::create();
            auto sequence = Sequence::create(fadeOut, removeExplosion, nullptr);
            appear->runAction(sequence);

            monster->setVisible(true);
            monster->moveOnPath(this->path);
            
        }, i * 1.0f, "spawn_monster_" + std::to_string(i));
    }
}

void BaseLevelScene::endGame() {
    monsters.clear();
    bottles.clear();
    fans.clear();
    shits.clear();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 150));
    this->addChild(maskLayer, 6);

    ImageView* tips;
    if (globalCarrot->health <= 0) {
        tips = ImageView::create("GameScene/lose.png");
    } else {
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
        auto scene = LevelFactory::createLevel(dynamic_cast<Level1Scene*>(this) ? 1 : 2);
        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene, Color3B::BLACK));
    });

    // 返回选关按钮
    auto returnButton = ui::Button::create("GameScene/return_normal.png", "GameScene/return_selected.png");
    returnButton->setPosition(Vec2(210, 90));
    tips->addChild(returnButton);
    returnButton->addClickEventListener([=](Ref*) {
        Director::getInstance()->resume();
        Director::getInstance()->getScheduler()->setTimeScale(1);
        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, SelectScene::create(), Color3B::BLACK));
    });

    Director::getInstance()->pause();

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch* touch, Event* event) {
        return true;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, maskLayer);

    globalCarrot = nullptr;
}

void BaseLevelScene::update(float dt) {
    if (monsters.size()) {
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
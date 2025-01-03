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
using namespace cocos2d;
using namespace cocos2d::ui;

USING_NS_CC;

Scene* Level2Scene::createScene()
{
    return Level2Scene::create();
}

bool Level2Scene::init()
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
    auto bg = Sprite::create("Level/Level2_bg.png");
    bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(bg);

    /* ������Ϸ���� */
    auto gamescene = GameScene::create();
    this->addChild(gamescene);

    /* ��ӡ������̨��λ�� */
    auto gamemap = GameMap::create();
    this->addChild(gamemap);

    /* ��ʼ������·�� */
    vector<GameMap::Grid>path_ = {
    { 4,5 },
    { 5,5 },
    { 6,5 },
    { 7,5 },
    { 8,5 },
    { 9,5 },
    { 9,4 },
    { 9,3 },
    { 8,3 },
    { 7,3 },
    { 6,3 },
    { 5,3 },
    { 4,3 },
    { 3,3 },
    { 2,3 },
    { 2,2 },
    { 2,1 },
    { 3,1 },
    { 4,1 },
    { 5,1 },
    { 6,1 },
    { 7,1 },
    };
    for (int i = 0; i < path_.size(); i++) {
        gamemap->addPathPoint(path_[i]);
    }

    /* ��ʼ������·���Ͳ��� */
    currentWave = 0;
    getPath(gamemap);

    /* ����ʱ��������� */
    auto delay = DelayTime::create(3.0f);
    auto createCarrot = CallFunc::create([this]() {
        /* ������� */
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

    /*  */
    std::vector<Vec2> positions; // 洢Է̨λ
    for (int y = 0; y < gamemap->GRID_HEIGHT - 1; ++y) {
        for (int x = 0; x < gamemap->GRID_WIDTH; ++x) {
            if (gamemap->gridMap[y][x]) {
                // ��λ�����ӵ� positions ����
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
    this->schedule(CC_SCHEDULE_SELECTOR(Level2Scene::startNextWave), 6.0f);

    this->scheduleUpdate();
    return true;
}

void Level2Scene::getPath(GameMap* gamemap)
{
    // ����·������������
    for (const auto& grid : gamemap->path)
    {
        path.push_back(gamemap->gridToPixel(grid.x, grid.y));
    }
}

void Level2Scene::startNextWave(float dt) {
    if (currentWave >= totalWaves) {
        // ���в�����ɣ���Ϸ����
        unschedule(CC_SCHEDULE_SELECTOR(Level2Scene::startNextWave));
        isLevel2Finish = true;
        endGame();
        return;
    }

    // ��ʼ��һ�����������
    spawnMonsters(currentWave);

    // ׼����һ��
    currentWave++;
    waveLabel->setString(std::to_string(currentWave) + "  " + std::to_string(totalWaves));
}

void Level2Scene::spawnMonsters(int waveIndex) {
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
            // �ܲ�Ѫ����Ϊ0������Ϸ����
            if (globalCarrot->health <= 0) {
                monster->cleanup();
                endGame();
            }
            // ������Ч
            auto appear = cocos2d::Sprite::create("Monster/appear.PNG");
            appear->setPosition(path.front());
            this->addChild(appear);

            // ����һ���������Ƴ�������Ч
            auto fadeOut = cocos2d::FadeOut::create(0.5f); // ����ʱ����Ը�����Ҫ����
            auto removeExplosion = cocos2d::RemoveSelf::create();
            auto sequence = cocos2d::Sequence::create(fadeOut, removeExplosion, nullptr);
            appear->runAction(sequence);

            monster->setVisible(true);
            monster->moveOnPath(this->path);

            }, i * 1.0f, "spawn_monster_" + std::to_string(i)); // ÿ��1.0�����һֻ����
    }
}

void Level2Scene::endGame() {
    // ������Ϸ�����߼�

    /* ���ֻ�ʤ��ʾ�� */
    // �ɰ��
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 150));
    this->addChild(maskLayer, 6);

    // ������ʾ��
    ImageView* tips;
    if (globalCarrot->health <= 0) {
        tips = ImageView::create("GameScene/lose.png");
    }
    else {
        tips = ImageView::create("GameScene/win.png");
    }

    tips->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    maskLayer->addChild(tips, 2);

    // ���°�ť�������� tips ���� 
    // ����һ�ΰ�ť
    auto restartButton = ui::Button::create("GameScene/again_normal.png", "GameScene/again_selected.png");
    restartButton->setPosition(Vec2(210, 185));
    tips->addChild(restartButton);
    restartButton->addClickEventListener([=](Ref*) {
        Director::getInstance()->resume();
        Director::getInstance()->getScheduler()->setTimeScale(1);

        // ���¿�ʼ��Ϸ
        auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 0));  // ͸�������ֲ�
        this->addChild(maskLayer);
        float duration = 0.1f; // �����ĳ���ʱ��
        float targetY = visibleSize.height; // Ŀ��λ�õ�Y����

        auto moveUp = MoveTo::create(duration, Vec2(0, targetY));
        auto callback = CallFunc::create([]() {
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, Level2Scene::create(), Color3B::BLACK)); // �л����³���
            });
        auto sequence = Sequence::create(moveUp, callback, nullptr);
        maskLayer->runAction(sequence);
        });

    // ѡ��ؿ���ť
    auto returnButton = ui::Button::create("GameScene/return_normal.png", "GameScene/return_selected.png");
    returnButton->setPosition(Vec2(210, 90));
    tips->addChild(returnButton);
    returnButton->addClickEventListener([=](Ref*) {
        Director::getInstance()->resume();
        Director::getInstance()->getScheduler()->setTimeScale(1);

        // ����ѡ�����
        auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 0));  // ͸�������ֲ�
        this->addChild(maskLayer);
        float duration = 0.1f; // �����ĳ���ʱ��
        float targetY = visibleSize.height; // Ŀ��λ�õ�Y����

        auto moveUp = MoveTo::create(duration, Vec2(0, targetY));
        auto callback = CallFunc::create([]() {
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, SelectScene::create(), Color3B::BLACK)); // �л����³���
            });
        auto sequence = Sequence::create(moveUp, callback, nullptr);
        maskLayer->runAction(sequence);
        });

    /* ��Ϸ��ͣ�߼� */
    // ��Ϸ��ͣ
    Director::getInstance()->pause();

    // ȷ���û����ܵ����ͣ�˵������Ԫ�أ�����ͨ������һ������������ͣ�˵������������е���¼�
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](cocos2d::Touch* touch, cocos2d::Event* event) {
        return true; // ���ɴ����¼�
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, maskLayer);

    /* ���ܲ���ָ����� */
    globalCarrot = nullptr;
}

void Level2Scene::update(float dt) {
    // ���³����е���������
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
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include <string.h>
#include "ui/CocosGUI.h"
#include "SelectScene.h"
#include "Level1.h"
#include"Global.h"
#include <vector>
using namespace cocos2d;
using namespace cocos2d::ui;

USING_NS_CC;
std::vector<Monster*> monsters; 
Bottle* bottle = Bottle::create();


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

    // ��ʼ��·���Ͳ���
    currentWave = 0;
    getPath(gamemap);

    //globalCarrot = Carrot::create();
   // globalCarrot->setPosition(path[path.size() - 1]);
   // this->addChild(globalCarrot);

    // ����¼�
    Level1Scene::click(gamemap);


    // ��ʼ��һ��
    this->schedule(CC_SCHEDULE_SELECTOR(Level1Scene::startNextWave), 6.0f);

    
    //bottle->setPosition(this->getContentSize() / 2);
    //this->addChild(bottle);
    //this->scheduleUpdate();
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
void Level1Scene::click(GameMap* gamemap)
{
    for (int y = 0; y < gamemap->GRID_HEIGHT; ++y) {
        for (int x = 0; x < gamemap->GRID_WIDTH; ++x) {
            if (gamemap->gridMap[y][x]) {
                auto startSprite = ImageView::create("GameScene/click.png");
                // �޸�λ�ü��㷽ʽ��ȷ��ͼƬλ������λ��һ��
                startSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
                startSprite->setPosition(gamemap->gridToPixel(x, y));
                startSprite->setVisible(false);  // ��ͼƬ����Ϊ���ɼ�
                addChild(startSprite);
            }
        }
    }
    // ��Ӵ����¼�������
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [=](Touch* touch, Event* event) {
        Vec2 location = touch->getLocation();
        bool clickedOnImage = false;

        // ���������ӽڵ㣬���Ұ������λ�õ�ͼƬ�ڵ�
        for (auto image : getChildren()) {
            if (auto sprite = dynamic_cast<ImageView*>(image)) {
                if (sprite->getBoundingBox().containsPoint(location)) {
                    sprite->setVisible(true);
                    clickedOnImage = true;
                }
                else {
                    sprite->setVisible(false);
                }
            }
        }
        // ������λ��û��ͼƬ��û�е����������ͼƬ�������򲻽����κβ���
        if (!clickedOnImage) {
            return false;
        }

        return true;
    };

    
    // ���������б�������ʵ�֣�

    // ����ÿ֡���ø���
    this->scheduleUpdate();

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

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

void Level1Scene::update(float dt) {
    // ���� Bottle
    for (Monster* monster : monsters) {
         if (bottle->findTargetInAttackRange(monsters)) {
             bottle->attack(monster); // ��������ڷ�Χ�ڣ����й���
         }
        
    }

    // ���¹����б�������ʵ�֣�
}
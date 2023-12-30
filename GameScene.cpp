#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include <string.h>
#include"carrot.h"
#include"PickTower.h"
#include"Level1.h"
#include"Global.h"
#include "ui/CocosGUI.h"
#include"GoldCoin.h"
#include "SelectScene.h"
using namespace cocos2d;
using namespace cocos2d::ui;

USING_NS_CC;
int level = 1;


/* -------------------- ��ͼ��������� ---------------------- */
Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    /* ��ʼ������ */
    if (!Scene::init())
    {
        return false;
    }

    /* ��ȡ��Ļ��С */
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /* ���ò˵��� */
    auto menu = Sprite::create("GameScene/menu.PNG");
    menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 38));
    this->addChild(menu);

    /* ����ʣ�ನ�� */
    auto gameover = Sprite::create("GameScene/gameover.PNG");
    gameover->setPosition(Vec2(visibleSize.width / 2 - 80, visibleSize.height - 33));
    this->addChild(gameover);

    /* ��ʾ������� */
    // ���� GoldCoin ����
    GoldCoin* goldCoin = GoldCoin::create();

    if (goldCoin && goldCoin->init())
    {
        // �� GoldCoin ������ӵ�������
        this->addChild(goldCoin);

        // ���赱ǰ�������Ϊ 500
        int currentGold = 300;

        // ���½��������ʾ
        goldCoin->updateGoldValue(currentGold);
    }

    /* ���ٰ�ť */
    auto speedButton = Button::create("GameScene/speed_button.PNG");
    speedButton->setPosition(Vec2(visibleSize.width / 2 + 150, visibleSize.height - 38));
    this->addChild(speedButton, 1);
    speedButton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED)
        {
            static bool isDoubleSpeed = false;
            isDoubleSpeed = !isDoubleSpeed; // �л��ٶ�״̬

            float speed = isDoubleSpeed ? 2.0f : 1.0f; // ����״̬ѡ���ٶ�
            Director::getInstance()->getScheduler()->setTimeScale(speed);

            // ���°�ť��ͼ��
            std::string buttonImage = isDoubleSpeed ? "GameScene/speed_button_selected.PNG" : "GameScene/speed_button.PNG";
            speedButton->loadTextureNormal(buttonImage);
        }
        });

    /* ��ͣ��ť */
    auto pauseButton = Button::create("GameScene/pause_button.PNG");
    pauseButton->setPosition(Vec2(visibleSize.width / 2 + 300, visibleSize.height - 38));
    this->addChild(pauseButton);
    pauseButton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            // �л���ͣ״̬
            isGamePaused = !isGamePaused;
            if (isGamePaused) {
                Director::getInstance()->pause(); // ��ͣ��Ϸ
                // �л����ָ�ͼ��
                pauseButton->loadTextures("GameScene/pause_button_selected.PNG", "GameScene/pause_button_selected_light.PNG");
            }
            else {
                Director::getInstance()->resume(); // �ָ���Ϸ
                // �л�����ͣͼ��
                pauseButton->loadTextures("GameScene/pause_button.PNG", "GameScene/pause_button.PNG");
            }
        }
        });
    /* ѡ�ť */
    GameScene::options();

    return true;
}

// ʵ��ѡ�ť�µĲ˵�
void GameScene::options()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    /* ѡ�ť */
    auto choiceButton = Button::create("GameScene/choice_button.PNG");
    choiceButton->setPosition(Vec2(visibleSize.width / 2 + 400, visibleSize.height - 38));
    this->addChild(choiceButton, 1);
    choiceButton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED)
        {
            /* ����������ʾ�� */
            // ���ɰ��
            maskLayer = LayerColor::create(Color4B(0, 0, 0, 150));
            this->addChild(maskLayer, 6);

            // ������ʾ��
            auto options = ImageView::create("GameScene/options.png");
            options->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
            maskLayer->addChild(options, 2);

            // ���°�ť������� options ���� 
            // ���¿�ʼ��ť
            auto restartButton = ui::Button::create("GameScene/restart_normal.png", "GameScene/restart_selected.png");
            restartButton->setPosition(Vec2(210, 185));
            options->addChild(restartButton);
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
                    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, Level1Scene::create(), Color3B::BLACK)); // �л����³���
                    });
                auto sequence = Sequence::create(moveUp, callback, nullptr);
                maskLayer->runAction(sequence);
                });

            // ѡ��ؿ���ť
            auto returnButton = ui::Button::create("GameScene/return_normal.png", "GameScene/return_selected.png");
            returnButton->setPosition(Vec2(210, 90));
            options->addChild(returnButton);
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

            // ������Ϸ��ť
            auto resumeButton = Button::create("GameScene/resume_normal.png", "GameScene/resume_selected.png");
            resumeButton->setPosition(Vec2(210, 278));
            options->addChild(resumeButton);
            resumeButton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
                if (type == Widget::TouchEventType::ENDED)
                {
                    // �ָ���Ϸ
                    Director::getInstance()->resume();
                    // �Ƴ���ͣ�˵�
                    options->removeFromParent();
                    // �Ƴ��ɰ��
                    maskLayer->removeFromParent();
                }
                });

            /* ��Ϸ��ͣ�߼� */
            // ��Ϸ��ͣ
            Director::getInstance()->pause();

            // ȷ���û����ܵ����ͣ�˵������Ԫ�أ�����ͨ�����һ������������ͣ�˵������������е���¼�
            auto listener = EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);
            listener->onTouchBegan = [](cocos2d::Touch* touch, cocos2d::Event* event) {
                return true; // ���ɴ����¼�
            };
            Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this->maskLayer);
        }
        });
}

// ��������ʱ�����ĺ���
void GameScene::createCountdownAnimation()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    // ���������ɰ��
    maskLayer = LayerColor::create(Color4B(0, 0, 0, 150));
    this->addChild(maskLayer, 5); // ȷ���ɰ��Ĳ㼶��������Ԫ��

    // ȷ���û����ܵ������ʱ�����Ԫ�أ�����ͨ�����һ������������ͣ�˵������������е���¼�
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](cocos2d::Touch* touch, cocos2d::Event* event) {
        return true; // ���ɴ����¼�
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this->maskLayer);

    // ��������ʱԲ��
    auto circlebase = Sprite::create("GameScene/circlebase.PNG");
    circlebase->setPosition(visibleSize / 2);
    maskLayer->addChild(circlebase);
    auto circleSprite = Sprite::create("GameScene/circle.PNG");
    circleSprite->setPosition(Vec2(visibleSize.width / 2 + 5, visibleSize.height / 2));
    maskLayer->addChild(circleSprite);

    // ��ʼ������ʱ����
    auto countdownSprite = Sprite::create("GameScene/3.PNG"); // �滻Ϊ���ĳ�ʼ����ʱ����ͼƬ
    countdownSprite->setPosition(circleSprite->getPosition());
    maskLayer->addChild(countdownSprite);

    // Բ��ÿ�������һ��ѭ��
    circleSprite->runAction(RepeatForever::create(RotateBy::create(1.0f, 360)));

    // ����ʱ��ʼ��ÿ�����һ������
    int countdownNumber = 3; // ��3��ʼ����ʱ
    auto scheduler = Director::getInstance()->getScheduler();
    scheduler->schedule([=](float dt) mutable {
        countdownNumber--;
        // ���µ���ʱ���־��������
        std::string frameName = "GameScene/" + std::to_string(countdownNumber) + ".PNG";
        countdownSprite->setTexture(frameName);

        // ������ʱ����ʱ
        if (countdownNumber <= 0) {
            scheduler->unschedule("countdown", this); // ȡ������ʱ����
            countdownSprite->setVisible(false); // �������־���
            // ����ʱ������
            maskLayer->removeFromParent();
        }

        }, this, 1.0f, false, "countdown");
}

/* -------------------- �Ե�ͼ����ʵ�� ---------------------- */
bool GameMap::init()
{
    if (!Layer::init()) {
        return false;
    }
    setupGrid(); // ��������

    // ���õ���ʱ��ƥ�䶯��
    int countdownNumber = 3; // ��3��ʼ����ʱ
    auto scheduler = Director::getInstance()->getScheduler();
    scheduler->schedule([=](float dt) mutable {
        countdownNumber--;
        if (countdownNumber <= 0) {
            // ��ӡ���
            GameMap::printStartSprite();

        }
        }, this, 1.0f, false, "countdown");

    return true;
}

void GameMap::setupGrid() {
    gridMap.resize(GRID_HEIGHT, vector<bool>(GRID_WIDTH));

    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            // ��ʼ��ÿ������
            if (y == 7)
                gridMap[y][x] = false;
            else
                gridMap[y][x] = true;
        }
    }
}

void GameMap::addPathPoint(Grid g) {
    // ȷ�������ڵ�ͼ��Χ��
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
    // �������Է�����̨��λ��
    for (int y = 0; y < GRID_HEIGHT - 1; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            if (gridMap[y][x]) {
                // ��ӡ����
                auto StartSprite = ImageView::create("GameScene/StartSprite.png");
                StartSprite->setPosition(gridToPixel(x, y));
                this->addChild(StartSprite);
            }
        }
    }

    // ����һ����˸����
    auto blinkAction = cocos2d::Blink::create(2, 4);

    // ����һ���ص�����������˸�������Ƴ�ͼ��
    auto removeLayer = CallFunc::create([this]() {
        this->removeFromParent();
        });

    // ����һ���������У�����˸�����Ƴ�
    auto sequence = Sequence::create(blinkAction, removeLayer, nullptr);

    // ���ж�������
    this->runAction(sequence);

}
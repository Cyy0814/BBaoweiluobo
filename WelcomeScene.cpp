#include "WelcomeScene.h"
#include "audio/include/AudioEngine.h"
#include <string.h>
#include <SettingScene.h>
#include <HelpScene.h>
#include <SelectScene.h>
#include "GameScene.h"
#include "Level1.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
using namespace cocos2d::experimental;

USING_NS_CC;

Scene* WelcomeScene::createScene()
{
    return WelcomeScene::create();
}

// ��Ӳ˵���̬ͼ��
void WelcomeScene::setMenu(char picture[], Vec2 position)
{
    // ��ӽ���
    auto sprite = Sprite::create(picture);

    // �� Sprite ������ position λ��
    sprite->setPosition(position);
      
    // �� Sprite ��Ϊ������ӵ��˲�
    if (picture == "WelcomeScene/lock.PNG")
        this->addChild(sprite, 1);
    else
        this->addChild(sprite, 0);
}

// ��ʼ����ӭ����
bool WelcomeScene::init()
{
    /* ��ʼ������ */
    if ( !Scene::init() )
    {
        return false;
    }
    /* ��ȡ��Ļ��С */
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /* ������ͼ */
    setMenu("WelcomeScene/MainBG.PNG", Vec2(visibleSize.width / 2, visibleSize.height / 2));

    /* Ư������ */
    auto cloud = Sprite::create("WelcomeScene/Cloud.PNG");
    cloud->setOpacity(140); // �����Ƶ�͸����
    cloud->setPosition(Vec2(-cloud->getContentSize().width, 550)); // �����Ƶĳ�ʼλ��
    this->addChild(cloud, 0);
    // ʵ�ֺᴩ��Ļ
    auto move = MoveTo::create(5.0f, Vec2(visibleSize.width + 100, cloud->getPositionY()));
    auto resetPosition = CallFunc::create([=]() {
        cloud->setPosition(Vec2(-cloud->getContentSize().width, cloud->getPositionY()));
        });
    auto sequence = Sequence::create(move, resetPosition, nullptr);
    auto repeat = RepeatForever::create(sequence);
    cloud->runAction(repeat);

    setMenu("WelcomeScene/Leaf1.PNG", Vec2(410, 470));//  �ܲ�Ҷ��1
    setMenu("WelcomeScene/Leaf2.PNG", Vec2(540, 470));//  �ܲ�Ҷ��2
    setMenu("WelcomeScene/Leaf3.PNG", Vec2(480, 505));//  �ܲ�Ҷ��3
    setMenu("WelcomeScene/CarrotBody.PNG", Vec2(visibleSize.width / 2, visibleSize.height * 0.6)); // �����ܲ�
    setMenu("WelcomeScene/MainTitle.PNG", Vec2(visibleSize.width / 2 + 30, visibleSize.height / 2 - 30));// �������ܲ�������
    //��������//
    //******************************************************************//
    AudioEngine::preload("sound/backGround.ogg");
    int AudioID = AudioEngine::play2d("sound/backGround.ogg");
    //*******************************************************************//
    /* �����ƶ��Ĺ��� */ 
    auto flymonster = Sprite::create("WelcomeScene/FlyMonster.PNG");
    flymonster->setPosition(Vec2(180, 470)); // ���ù��޵ĳ�ʼλ��
    this->addChild(flymonster, 1);
    // ʵ�������ƶ�
    auto moveUp = MoveBy::create(1.5f, Vec2(0, 50)); // �����ƶ�50����λ
    auto moveDown = moveUp->reverse(); // �����ƶ�ͬ���ľ���
    auto floatAction = RepeatForever::create(Sequence::create(moveUp, moveDown, nullptr)); // �����ظ������ƶ��Ķ���
    flymonster->runAction(floatAction);
    /* ���ð�ť */
    auto settingsButton = ui::Button::create("Btn_Set.PNG", "Btn_SetLight.PNG");
    settingsButton->setPosition(Vec2(210, 230)); // ���ð�ť�ĳ�ʼλ��
    // ��ť����¼������߼�
    settingsButton->addClickEventListener([=](Ref*) {
        // �������ֲ�
        auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 0));  // ͸�������ֲ�
        this->addChild(maskLayer);
       
        float duration = 0.1f; // �����ĳ���ʱ��
        float targetY = visibleSize.height; // Ŀ��λ�õ�Y����

        auto moveUp = MoveTo::create(duration, Vec2(0, targetY));
        auto callback = CallFunc::create([]() {
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, SettingScene::create(), Color3B::BLACK)); // �л����³���
            });
        auto sequence = Sequence::create(moveUp, callback, nullptr);
        maskLayer->runAction(sequence);
        });
    this->addChild(settingsButton, 0);

    /* ������ť */
    auto helpButton = ui::Button::create("Btn_Help.PNG", "Btn_HelpLight.PNG");
    helpButton->setPosition(Vec2(770, 230)); // ���ð�ť�ĳ�ʼλ��
    // ��ť����¼������߼�
    helpButton->addClickEventListener([=](Ref*) {
        // �������ֲ�
        auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 0));  // ͸�������ֲ�
        this->addChild(maskLayer);

        float duration = 0.1f; // �����ĳ���ʱ��
        float targetY = visibleSize.height; // Ŀ��λ�õ�Y����

        auto moveUp = MoveTo::create(duration, Vec2(0, targetY));
        auto callback = CallFunc::create([]() {
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, HelpScene::create(), Color3B::BLACK)); // �л����³���
            });
        auto sequence = Sequence::create(moveUp, callback, nullptr);
        maskLayer->runAction(sequence);
        });
    this->addChild(helpButton, 0);

    /* ð��ģʽ */
    auto Normal = MenuItemImage::create("WelcomeScene/Btn_NormalModle.PNG", 
        "WelcomeScene/Btn_NormalModleLight.PNG", 
        CC_CALLBACK_1(WelcomeScene::gotoSelectScene, this));

    Normal->setPosition(Vec2(150, 80));
    auto menu = Menu::create(Normal, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /* Bossģʽ */ 
    auto Boss = MenuItemImage::create("WelcomeScene/Btn_Boss.PNG", 
        "WelcomeScene/Btn_BossLight.PNG", 
        CC_CALLBACK_1(WelcomeScene::bossLock, this));

    Boss->setPosition(Vec2(480, 80));
    menu = Menu::create(Boss, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /* ������ģʽ */ 
    auto MonsterNest = MenuItemImage::create("WelcomeScene/Btn_MonsterNest.PNG",
        "WelcomeScene/Btn_MonsterNestLight.PNG",
        CC_CALLBACK_1(WelcomeScene::nestLock, this));

    MonsterNest->setPosition(Vec2(810, 80));
    menu = Menu::create(MonsterNest, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /* lockͼ�� */
    setMenu("WelcomeScene/lock.PNG", Vec2(610, 70)); 
    setMenu("WelcomeScene/lock.PNG", Vec2(940, 70)); 
}

/* ����ؿ�ѡ����� */ 
void WelcomeScene::gotoSelectScene(cocos2d::Ref* pSender)
{
    AudioEngine::stopAll();
    // ��ȡ��Ļ��С
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // �������ֲ�
    auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 0));  // ͸�������ֲ�
    this->addChild(maskLayer);

    float duration = 0.1f; // �����ĳ���ʱ��
    float targetY = visibleSize.height; // Ŀ��λ�õ�Y����

    auto moveUp = MoveTo::create(duration, Vec2(0, targetY));
    auto callback = CallFunc::create([]() {                                   //�޸Ĵ˴�
        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, SelectScene::create(), Color3B::BLACK)); // �л����³���
        });
    auto sequence = Sequence::create(moveUp, callback, nullptr);
    maskLayer->runAction(sequence);
}

void WelcomeScene::bossLock(Ref* pSender)
{
    // ��ȡ��Ļ��С
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // ������ʾ��
    auto bosslock = ui::ImageView::create("WelcomeScene/boss_lock_image.png");
    bosslock->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(bosslock);

    // �����رհ�ť
    auto closeButton = ui::Button::create("WelcomeScene/lock_btn_normal.PNG",
        "WelcomeScene/lock_btn_selected.PNG");
    closeButton->setPosition(Vec2(270, 60));
    bosslock->addChild(closeButton); // ����ť��ӵ���ʾ����

    // ��ӵ���¼�������
    closeButton->addClickEventListener([=](Ref*) {
        bosslock->removeFromParent(); // �Ƴ���ʾ��
        });
}

void WelcomeScene::nestLock(Ref* pSender)
{
    // ��ȡ��Ļ��С
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // ������ʾ��
    auto bosslock = ui::ImageView::create("WelcomeScene/nest_lock_image.png");
    bosslock->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(bosslock);

    // �����رհ�ť
    auto closeButton = ui::Button::create("WelcomeScene/lock_btn_normal.PNG",
        "WelcomeScene/lock_btn_selected.PNG");
    closeButton->setPosition(Vec2(270, 60));
    bosslock->addChild(closeButton); // ����ť��ӵ���ʾ����

    // ��ӵ���¼�������
    closeButton->addClickEventListener([=](Ref*) {
        bosslock->removeFromParent(); // �Ƴ���ʾ��
        });
}
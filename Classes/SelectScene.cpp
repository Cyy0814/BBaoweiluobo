#include "SimpleAudioEngine.h"
#include "SelectScene.h"
#include "WelcomeScene.h"
#include "HelpScene.h"
#include <string.h>
#include "ui/CocosGUI.h"
#include "cocos/ui/UIImageView.h"

USING_NS_CC;

Scene* SelectScene::createScene()
{
    return SelectScene::create();
}

bool SelectScene::init()
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
    auto backGround = Sprite::create("SelectScene/stages_bg-hd_0.PNG");
    backGround->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(backGround, 0);

    /* �ƶ䱳�� */
    auto cloud1 = Sprite::create("SelectScene/BG_Left.PNG");
    cloud1->setPosition(Vec2(130, 75));
    this->addChild(cloud1, 0);

    auto cloud2 = Sprite::create("SelectScene/BG_Right.PNG");
    cloud2->setPosition(Vec2(830, 75));
    this->addChild(cloud2, 0);

    /* ���������水ť */
    auto backToWelcome = ui::Button::create("SelectScene/stages_bg-hd_41.PNG", "SelectScene/stages_bg-hd_40.PNG");
    backToWelcome->setPosition(Vec2(40, 610)); // ���ð�ť�ĳ�ʼλ��
    // ��ť����¼������߼�
    backToWelcome->addClickEventListener([=](Ref*) {
        // �������ֲ�
        auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 0));  // ͸�������ֲ�
        this->addChild(maskLayer);

        float duration = 0.1f; // �����ĳ���ʱ��
        float targetY = visibleSize.height; // Ŀ��λ�õ�Y����

        auto moveUp = MoveTo::create(duration, Vec2(0, targetY));
        auto callback = CallFunc::create([]() {
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, WelcomeScene::create(), Color3B::BLACK)); // �л����³���
            });
        auto sequence = Sequence::create(moveUp, callback, nullptr);
        maskLayer->runAction(sequence);
        });
    this->addChild(backToWelcome, 1);
  
    /* ������ť */
    auto helpButton = ui::Button::create("SelectScene/stages_bg-hd_42.PNG", "SelectScene/stages_bg-hd_33.PNG");
    helpButton->setPosition(Vec2(920, 610)); // ���ð�ť�ĳ�ʼλ��
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
    this->addChild(helpButton, 1);

    /* ���뻬��ѡ�ز��� */ 
    SelectScene::scroll();
}

void SelectScene::scroll()
{
    /* ��ȡ��Ļ��С */
    auto visibleSize = Director::getInstance()->getVisibleSize();

    int numLevels = 5;
    int levelPreviewWidth = 501;
    int gapX = 30;
    // ������������
    auto scrollView = ui::ScrollView::create();
    scrollView->setContentSize(Size(visibleSize.width, visibleSize.height));
    scrollView->setInnerContainerSize(Size(numLevels * (levelPreviewWidth + gapX) + 300, visibleSize.height));
    scrollView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    this->addChild(scrollView);

    // ѭ����ӡ�ؿ�Ԥ��ͼ
    for (int i = 0; i < numLevels; i++) {
        char a[30] = "SelectScene/Level_1.PNG";
        a[18] = '1' + i;
        auto imageView = Sprite::create(a);

        // ����ImageView��λ��
        float posX = visibleSize.width / 2 + i * levelPreviewWidth;
        float posY = visibleSize.height / 2 + 30;
        imageView->setPosition(Vec2(posX, posY));

        // ��ImageView���Ϊ�����������ӽڵ�
        scrollView->addChild(imageView);
    }

    // ���û�����ز���
    bool isDragging = false;  // �Ƿ������϶�
    Vec2 startTouchPos;  // ��ʼ����λ��
    Vec2 startInnerContainerPos;  // ��ʼ����ʱ�Ļ��������ڲ�λ��

    // ��Ӵ����¼�������
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [&](Touch* touch, Event* event) {
        if (scrollView->isTouchEnabled() && scrollView->getBoundingBox().containsPoint(touch->getLocation())) {
            isDragging = true;
            startTouchPos = touch->getLocation();
            startInnerContainerPos = scrollView->getInnerContainerPosition();
            return true;
        }
        return false;
    };
    listener->onTouchMoved = [&](Touch* touch, Event* event) {
        if (isDragging) {
            Vec2 delta = touch->getLocation() - startTouchPos;
            Vec2 newPosition = startInnerContainerPos + delta;
            scrollView->setInnerContainerPosition(newPosition);

            // ���¹ؿ�Ԥ��ͼ��͸����
           // updateLevelPreviewOpacity(newPosition.x);
        }
    };
    listener->onTouchEnded = [&](Touch* touch, Event* event) {
        if (isDragging) {
            isDragging = false;

            // �ָ��ؿ�Ԥ��ͼ��͸����
            //resetLevelPreviewOpacity();
        }
    };
    listener->onTouchCancelled = [&](Touch* touch, Event* event) {
        if (isDragging) {
            isDragging = false;

            // �ָ��ؿ�Ԥ��ͼ��͸����
          //  resetLevelPreviewOpacity();
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, scrollView);
}
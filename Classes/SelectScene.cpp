#include "SimpleAudioEngine.h"
#include "SelectScene.h"
#include "WelcomeScene.h"
#include "HelpScene.h"
#include"Level1.h"
#include"Level2.h"
#include"Global.h"
#include "ui/CocosGUI.h"
#include"cocos2d.h"
#include "cocos/ui/UIImageView.h"
#include "LevelFactory.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocos2d;

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

    return true;
}

void SelectScene::scroll()
{
    // ��ȡ��Ļ��С
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    int numLevels = 5; // ���ŵ�ͼ

    Button*  startButton = ui::Button::create("SelectScene/stages_bg-hd_30.PNG", "SelectScene/stages_bg-hd_28.PNG"); // ������ť

    // ���� PageView
    auto pageView = ui::PageView::create();
    pageView->setContentSize(visibleSize);
    pageView->setPosition(origin);
    pageView->setTag(TAG_PAGE_VIEW);
    this->addChild(pageView);

    // ѭ����ӡ�ؿ�Ԥ��ͼ�����뻬������
    for (int i = 0; i < numLevels; i++) {
        // ����ҳ��չʾ����
        auto layout = Layout::create();
        layout->setContentSize(Size(visibleSize.width, visibleSize.height));
        layout->setTag(TAG_LEVEL_1 + i); // ���ò�ͬ�ı�ǩ

        // ���ӹؿ�ͼƬ
        char level[30] = "SelectScene/Level_1.PNG";
        level[18] = '1' + i;
        auto levelImage = ImageView::create(level);
        levelImage->setTag(TAG_LEVEL_IMAGE);
        levelImage->setPosition(Vec2(layout->getContentSize().width / 2, layout->getContentSize().height / 2 + 30));
        layout->addChild(levelImage);
     
        // �����Ƿ�����
        if (i >= 2 || (i == 1 && !isLevel1Finish)) {
            // ��������ͼ��
            auto lock = ImageView::create("SelectScene/stages_bg-hd_31.PNG");
            lock->setTag(TAG_LOCK_IMAGE);
            lock->setPosition(Vec2(layout->getContentSize().width / 2 + 140, layout->getContentSize().height / 2 - 70));
            layout->addChild(lock);
        }

        // �����Ƿ���ɹؿ�
        if (i == 0 && isLevel1Finish || i == 1 && isLevel2Finish) {
            // ������ɵ�ͼ��
            auto finish = ImageView::create("SelectScene/finish.png");
            finish->setTag(TAG_FINISH_IMAGE);
            finish->setPosition(Vec2(layout->getContentSize().width / 2 + 150, layout->getContentSize().height / 2 - 50));
            layout->addChild(finish);
        }

        // ��ÿһ��ͼ���ӵ�����ҳ����
        pageView->addPage(layout);
    }

    // ��ʼ����ʹ��һҳ��Ϊ��ǰҳ
    this->updateLevelState(pageView->getCurrentPageIndex());

    // ���� PageView �¼�������������ҳ��仯�����¹ؿ�״̬
    pageView->addEventListener([this](Ref* sender, ui::PageView::EventType type) {
        if (type == ui::PageView::EventType::TURNING) {
            auto pageView = dynamic_cast<PageView*>(sender);
            int pageIndex = pageView->getCurrentPageIndex();
            this->updateLevelState(pageIndex);
        }
        });

    // ������ʼ��Ϸ��ť
    startButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 240)); // ��ťλ����ҳ��ײ�
    startButton->setTag(TAG_START_BUTTON);
    // �����ť�¼�
    startButton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED)
        {
            this->gotoGameScene(sender);
        }
        });
    this->addChild(startButton, 2);
}

/* ����ҳ��仯�����¹ؿ�״̬ */
void SelectScene::updateLevelState(int pageIndex)
{
    auto pageView = dynamic_cast<ui::PageView*>(this->getChildByTag(TAG_PAGE_VIEW));
    int i = 0;

    /* �Ե�ͼ��͸���Ƚ��и��� */
    for (auto& item : pageView->getChildren()) {
        auto layout = dynamic_cast<ui::Layout*>(item);
        auto levelImage = dynamic_cast<ui::ImageView*>(layout->getChildByTag(TAG_LEVEL_IMAGE));
        // �����Ƿ��ǵ�ǰҳ��������͸����
        levelImage->setOpacity(layout->getTag() == (TAG_LEVEL_1 + pageIndex) ? 255 : 50);

        // ����͸���ȱ仯
        if (i >= 2 || (i == 1 && !isLevel1Finish)) {
            auto lock = dynamic_cast<ui::ImageView*>(layout->getChildByTag(TAG_LOCK_IMAGE));
            lock->setOpacity(layout->getTag() == (TAG_LEVEL_1 + pageIndex) ? 255 : 50);
        }
        // ��ɱ�־��͸���ȱ仯
        if (i == 0 && isLevel1Finish || i == 1 && isLevel2Finish) {
            auto finish = dynamic_cast<ui::ImageView*>(layout->getChildByTag(TAG_FINISH_IMAGE));
            finish->setOpacity(layout->getTag() == (TAG_LEVEL_1 + pageIndex) ? 255 : 50);
        }
        i++;
    }

    /* ��ÿһҳ�Ĺ̶��������и��� */
    // ����̶������ı�ǩ
    const int TAG_FIXED_PART = 1000;

    // ���ӹ̶�������ÿ��ҳ��Ĳ�������һ��
    auto tower1 = ImageView::create("SelectScene/Tower_1.PNG");
    tower1->setTag(TAG_FIXED_PART);
    auto tower2 = ImageView::create("SelectScene/Tower_2.PNG");
    tower2->setTag(TAG_FIXED_PART + 1);
    auto tower3 = ImageView::create("SelectScene/Tower_3.PNG");
    tower3->setTag(TAG_FIXED_PART + 2);
    auto tower4 = ImageView::create("SelectScene/Tower_4.PNG");
    tower4->setTag(TAG_FIXED_PART + 3);
    auto tower5 = ImageView::create("SelectScene/Tower_5.PNG");
    tower5->setTag(TAG_FIXED_PART + 4);
    auto labelText1 = Label::createWithTTF("A total of 15 waves", "fonts/Marker Felt.ttf", 22);
    labelText1->setTag(TAG_FIXED_PART + 5);
    auto labelText2 = Label::createWithTTF("A total of 20 waves", "fonts/Marker Felt.ttf", 22);
    labelText2->setTag(TAG_FIXED_PART + 6);

    // �Ƴ���һ�����ӵĹ̶�����
    for (int i = 0; i < 7; i++) {
        this->removeChildByTag(TAG_FIXED_PART + i);
    }

    if (pageIndex == 0) {
        tower1->setPosition(Vec2(pageView->getContentSize().width / 2 - 30, pageView->getContentSize().height / 2 - 160));
        this->addChild(tower1);

        tower2->setPosition(Vec2(pageView->getContentSize().width / 2 + 30, pageView->getContentSize().height / 2 - 160));
        this->addChild(tower2);

        labelText1->setPosition(Vec2(pageView->getContentSize().width / 2 + 130, pageView->getContentSize().height / 2 + 200));
        this->addChild(labelText1);

    }
    else if (pageIndex == 1) {
        tower1->setPosition(Vec2(pageView->getContentSize().width / 2 - 60, pageView->getContentSize().height / 2 - 160));
        this->addChild(tower1);

        tower2->setPosition(Vec2(pageView->getContentSize().width / 2, pageView->getContentSize().height / 2 - 160));
        this->addChild(tower2);

        tower3->setPosition(Vec2(pageView->getContentSize().width / 2 + 60, pageView->getContentSize().height / 2 - 160));
        this->addChild(tower3);

        labelText1->setPosition(Vec2(pageView->getContentSize().width / 2 + 130, pageView->getContentSize().height / 2 + 200));
        this->addChild(labelText1);
    }
    else if (pageIndex == 2) {
        tower1->setPosition(Vec2(pageView->getContentSize().width / 2 - 60, pageView->getContentSize().height / 2 - 160));
        this->addChild(tower1);

        tower2->setPosition(Vec2(pageView->getContentSize().width / 2, pageView->getContentSize().height / 2 - 160));
        this->addChild(tower2);

        tower4->setPosition(Vec2(pageView->getContentSize().width / 2 + 60, pageView->getContentSize().height / 2 - 160));
        this->addChild(tower4);

        labelText2->setPosition(Vec2(pageView->getContentSize().width / 2 + 130, pageView->getContentSize().height / 2 + 200));
        this->addChild(labelText2);
    }
    else if (pageIndex == 3) {
        tower2->setPosition(Vec2(pageView->getContentSize().width / 2 - 30, pageView->getContentSize().height / 2 - 160));
        this->addChild(tower2);

        tower5->setPosition(Vec2(pageView->getContentSize().width / 2 + 30, pageView->getContentSize().height / 2 - 160));
        this->addChild(tower5);

        labelText2->setPosition(Vec2(pageView->getContentSize().width / 2 + 130, pageView->getContentSize().height / 2 + 200));
        this->addChild(labelText2);
    }
    else if (pageIndex == 4) {
        tower2->setPosition(Vec2(pageView->getContentSize().width / 2 - 30, pageView->getContentSize().height / 2 - 160));
        this->addChild(tower2);

        tower4->setPosition(Vec2(pageView->getContentSize().width / 2 + 30, pageView->getContentSize().height / 2 - 160));
        this->addChild(tower4);

        labelText2->setPosition(Vec2(pageView->getContentSize().width / 2 + 130, pageView->getContentSize().height / 2 + 200));
        this->addChild(labelText2);
    }

}

/* ������Ϸ���� */
void SelectScene::gotoGameScene(Ref* sender)
{
    auto button = dynamic_cast<Button*>(sender);
    int levelNum = button->getTag();
    
    // 使用工厂创建关卡
    auto level = LevelFactory::createLevel(levelNum);
    if (level) {
        Director::getInstance()->replaceScene(
            TransitionFade::create(0.5f, level, Color3B::BLACK)
        );
    } else {
        // 统一的错误处理
        CCLOG("Failed to create level %d", levelNum);
    }
}
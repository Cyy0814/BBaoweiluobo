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
    /* 初始化场景 */
    if (!Scene::init())
    {
        return false;
    }

    /* 获取屏幕大小 */
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /* 设置背景 */
    auto backGround = Sprite::create("SelectScene/stages_bg-hd_0.PNG");
    backGround->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(backGround, 0);

    /* 移动背景 */
    auto cloud1 = Sprite::create("SelectScene/BG_Left.PNG");
    cloud1->setPosition(Vec2(130, 75));
    this->addChild(cloud1, 0);

    auto cloud2 = Sprite::create("SelectScene/BG_Right.PNG");
    cloud2->setPosition(Vec2(830, 75));
    this->addChild(cloud2, 0);

    /* 返回主菜单按钮 */
    auto backToWelcome = ui::Button::create("SelectScene/stages_bg-hd_41.PNG", "SelectScene/stages_bg-hd_40.PNG");
    backToWelcome->setPosition(Vec2(40, 610)); // 按钮初始位置
    // 按钮点击事件处理逻辑
    backToWelcome->addClickEventListener([=](Ref*) {
        // 创建遮罩层
        auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 0));  // 半透明黑色遮罩层
        this->addChild(maskLayer);

        float duration = 0.1f; // 动画的持续时间
        float targetY = visibleSize.height; // 目标位置的Y坐标

        auto moveUp = MoveTo::create(duration, Vec2(0, targetY));
        auto callback = CallFunc::create([]() {
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, WelcomeScene::create(), Color3B::BLACK)); // 切换到新场景
            });
        auto sequence = Sequence::create(moveUp, callback, nullptr);
        maskLayer->runAction(sequence);
        });
    this->addChild(backToWelcome, 1);
  
    /* 帮助按钮 */
    auto helpButton = ui::Button::create("SelectScene/stages_bg-hd_42.PNG", "SelectScene/stages_bg-hd_33.PNG");
    helpButton->setPosition(Vec2(920, 610)); // 按钮初始位置
    // 按钮点击事件处理逻辑
    helpButton->addClickEventListener([=](Ref*) {
        // 创建遮罩层
        auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 0));  // 半透明黑色遮罩层
        this->addChild(maskLayer);

        float duration = 0.1f; // 动画的持续时间
        float targetY = visibleSize.height; // 目标位置的Y坐标

        auto moveUp = MoveTo::create(duration, Vec2(0, targetY));
        auto callback = CallFunc::create([]() {
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, HelpScene::create(), Color3B::BLACK)); // 切换到新场景
            });
        auto sequence = Sequence::create(moveUp, callback, nullptr);
        maskLayer->runAction(sequence);
        });
    this->addChild(helpButton, 1);

    /* 加载关卡选择界面 */ 
    SelectScene::scroll();

    return true;
}

void SelectScene::scroll()
{
    // 获取屏幕大小
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    int numLevels = 5; // 关卡数量

    Button*  startButton = ui::Button::create("SelectScene/stages_bg-hd_30.PNG", "SelectScene/stages_bg-hd_28.PNG"); // 按钮

    // 创建PageView
    auto pageView = ui::PageView::create();
    pageView->setContentSize(visibleSize);
    pageView->setPosition(origin);
    pageView->setTag(TAG_PAGE_VIEW);
    this->addChild(pageView);

    // 循环创建关卡界面
    for (int i = 0; i < numLevels; i++) {
        // 创建关卡界面布局
        auto layout = Layout::create();
        layout->setContentSize(Size(visibleSize.width, visibleSize.height));
        layout->setTag(TAG_LEVEL_1 + i); // 使用不同tag标识不同关卡

        // 加载关卡图片
        char level[30] = "SelectScene/Level_1.PNG";
        level[18] = '1' + i;
        auto levelImage = ImageView::create(level);
        levelImage->setTag(TAG_LEVEL_IMAGE);
        levelImage->setPosition(Vec2(layout->getContentSize().width / 2, layout->getContentSize().height / 2 + 30));
        layout->addChild(levelImage);
     
        // 判断是否解锁
        if (i >= 2 || (i == 1 && !isLevel1Finish)) {
            // 加载锁定图片
            auto lock = ImageView::create("SelectScene/stages_bg-hd_31.PNG");
            lock->setTag(TAG_LOCK_IMAGE);
            lock->setPosition(Vec2(layout->getContentSize().width / 2 + 140, layout->getContentSize().height / 2 - 70));
            layout->addChild(lock);
        }

        // 判断是否通关
        if (i == 0 && isLevel1Finish || i == 1 && isLevel2Finish) {
            // 加载通关图片
            auto finish = ImageView::create("SelectScene/finish.png");
            finish->setTag(TAG_FINISH_IMAGE);
            finish->setPosition(Vec2(layout->getContentSize().width / 2 + 150, layout->getContentSize().height / 2 - 50));
            layout->addChild(finish);
        }

        // 将关卡界面添加到PageView
        pageView->addPage(layout);
    }

    // 初始化时默认显示第一关
    this->updateLevelState(pageView->getCurrentPageIndex());

    // PageView监听事件，监听页面变化，更新关卡状态
    pageView->addEventListener([this](Ref* sender, ui::PageView::EventType type) {
        if (type == ui::PageView::EventType::TURNING) {
            auto pageView = dynamic_cast<PageView*>(sender);
            int pageIndex = pageView->getCurrentPageIndex();
            this->updateLevelState(pageIndex);
        }
        });

    // 初始化时显示开始按钮
    startButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 240)); // 按钮位置在页面底部
    startButton->setTag(TAG_START_BUTTON);
    // 按钮监听事件
    startButton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED)
        {
            this->gotoGameScene(sender);
        }
        });
    this->addChild(startButton, 2);
}

/* 更新关卡状态 */
void SelectScene::updateLevelState(int pageIndex)
{
    auto pageView = dynamic_cast<ui::PageView*>(this->getChildByTag(TAG_PAGE_VIEW));
    int i = 0;

    /* 遍历所有关卡界面，更新图片透明度 */
    for (auto& item : pageView->getChildren()) {
        auto layout = dynamic_cast<ui::Layout*>(item);
        auto levelImage = dynamic_cast<ui::ImageView*>(layout->getChildByTag(TAG_LEVEL_IMAGE));
        // 判断是否当前页面，更新图片透明度
        levelImage->setOpacity(layout->getTag() == (TAG_LEVEL_1 + pageIndex) ? 255 : 50);

        // 其他图片透明度不变
        if (i >= 2 || (i == 1 && !isLevel1Finish)) {
            auto lock = dynamic_cast<ui::ImageView*>(layout->getChildByTag(TAG_LOCK_IMAGE));
            lock->setOpacity(layout->getTag() == (TAG_LEVEL_1 + pageIndex) ? 255 : 50);
        }
        // 其他图片透明度不变
        if (i == 0 && isLevel1Finish || i == 1 && isLevel2Finish) {
            auto finish = dynamic_cast<ui::ImageView*>(layout->getChildByTag(TAG_FINISH_IMAGE));
            finish->setOpacity(layout->getTag() == (TAG_LEVEL_1 + pageIndex) ? 255 : 50);
        }
        i++;
    }

    /* 固定部分图片 */
    // 固定部分tag
    const int TAG_FIXED_PART = 1000;

    // 创建固定部分图片
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

    // 删除固定部分所有图片
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

/* 进入游戏场景 */
void SelectScene::gotoGameScene(Ref* sender)
{
    auto button = dynamic_cast<Button*>(sender);
    int levelNum = button->getTag();
    
    //---------------------------------refactored with factory pattern---------------------------------//
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
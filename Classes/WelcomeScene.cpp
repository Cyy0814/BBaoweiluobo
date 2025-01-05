#include "WelcomeScene.h"
#include "SimpleAudioEngine.h"
#include <string.h>
#include <SettingScene.h>
#include <HelpScene.h>
#include <SelectScene.h>
#include "GameScene.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;

USING_NS_CC;

Scene* WelcomeScene::createScene()
{
    return WelcomeScene::create();
}

// 添加菜单静态图标
void WelcomeScene::setMenu(char picture[], Vec2 position)
{
    // 创建精灵
    auto sprite = Sprite::create(picture);

    // 设置精灵的位置
    sprite->setPosition(position);
      
    // 设置精灵为场景的子节点
    if (picture == "WelcomeScene/lock.PNG")
        this->addChild(sprite, 1);
    else
        this->addChild(sprite, 0);
}


// 初始化欢迎场景
bool WelcomeScene::init()
{
    /* 初始化场景 */
    if ( !Scene::init() )
    {
        return false;
    }

    /* 获取屏幕大小 */
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /* 加载背景 */
    setMenu("WelcomeScene/MainBG.PNG", Vec2(visibleSize.width / 2, visibleSize.height / 2));

    /* 飘动的云 */
    auto cloud = Sprite::create("WelcomeScene/Cloud.PNG");
    cloud->setOpacity(140); // 设置云的透明度
    cloud->setPosition(Vec2(-cloud->getContentSize().width, 550)); // 设置云的初始位置
    this->addChild(cloud, 0);
    // 实现横穿屏幕
    auto move = MoveTo::create(5.0f, Vec2(visibleSize.width + 100, cloud->getPositionY()));
    auto resetPosition = CallFunc::create([=]() {
        cloud->setPosition(Vec2(-cloud->getContentSize().width, cloud->getPositionY()));
        });
    auto sequence = Sequence::create(move, resetPosition, nullptr);
    auto repeat = RepeatForever::create(sequence);
    cloud->runAction(repeat);

    setMenu("WelcomeScene/Leaf1.PNG", Vec2(410, 470));//  Ҷ1
    setMenu("WelcomeScene/Leaf2.PNG", Vec2(540, 470));//  Ҷ2
    setMenu("WelcomeScene/Leaf3.PNG", Vec2(480, 505));//  Ҷ3
    setMenu("WelcomeScene/CarrotBody.PNG", Vec2(visibleSize.width / 2, visibleSize.height * 0.6)); // 
    setMenu("WelcomeScene/MainTitle.PNG", Vec2(visibleSize.width / 2 + 30, visibleSize.height / 2 - 30));// 

    /* 设置按钮 */
    auto flymonster = Sprite::create("WelcomeScene/FlyMonster.PNG");
    flymonster->setPosition(Vec2(180, 470)); // 怪物的初始位置
    this->addChild(flymonster, 1);
    // 实现怪物移动
    auto moveUp = MoveBy::create(1.5f, Vec2(0, 50)); // 移动50像素
    auto moveDown = moveUp->reverse(); // 移动相同距离
    auto floatAction = RepeatForever::create(Sequence::create(moveUp, moveDown, nullptr)); // 重复移动动作
    flymonster->runAction(floatAction);

    /* 设置按钮 */
    auto settingsButton = ui::Button::create("WelcomeScene/Btn_Set.PNG", "WelcomeScene/Btn_SetLight.PNG");
    settingsButton->setPosition(Vec2(210, 230)); // 按钮的初始位置
    // 按钮点击事件处理逻辑
    settingsButton->addClickEventListener([=](Ref*) {
        // 创建遮罩层
        auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 0));  // 遮罩层
        this->addChild(maskLayer);

        float duration = 0.1f; // 动画的持续时间
        float targetY = visibleSize.height; // 目标位置的Y坐标

        auto moveUp = MoveTo::create(duration, Vec2(0, targetY));
        auto callback = CallFunc::create([]() {
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, SettingScene::create(), Color3B::BLACK)); // 切换到新场景
            });
        auto sequence = Sequence::create(moveUp, callback, nullptr);
        maskLayer->runAction(sequence);
        });
    this->addChild(settingsButton, 0);

    /* 设置按钮 */
    auto helpButton = ui::Button::create("WelcomeScene/Btn_Help.PNG", "WelcomeScene/Btn_HelpLight.PNG");
    helpButton->setPosition(Vec2(770, 230)); // 按钮的初始位置
    // 按钮点击事件处理逻辑
    helpButton->addClickEventListener([=](Ref*) {
        // 创建遮罩层
        auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 0));  // 遮罩层
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
    this->addChild(helpButton, 0);

    /* 普通模式 */ 
    auto Normal = MenuItemImage::create("WelcomeScene/Btn_NormalModle.PNG", 
        "WelcomeScene/Btn_NormalModleLight.PNG", 
        CC_CALLBACK_1(WelcomeScene::gotoSelectScene, this));

    Normal->setPosition(Vec2(150, 80));
    auto menu = Menu::create(Normal, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /* Boss模式 */ 
    auto Boss = MenuItemImage::create("WelcomeScene/Btn_Boss.PNG", 
        "WelcomeScene/Btn_BossLight.PNG", 
        CC_CALLBACK_1(WelcomeScene::bossLock, this));

    Boss->setPosition(Vec2(480, 80));
    menu = Menu::create(Boss, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /* 怪物模式 */ 
    auto MonsterNest = MenuItemImage::create("WelcomeScene/Btn_MonsterNest.PNG",
        "WelcomeScene/Btn_MonsterNestLight.PNG",
        CC_CALLBACK_1(WelcomeScene::nestLock, this));

    MonsterNest->setPosition(Vec2(810, 80));
    menu = Menu::create(MonsterNest, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /* lock图标 */
    setMenu("WelcomeScene/lock.PNG", Vec2(610, 70)); 
    setMenu("WelcomeScene/lock.PNG", Vec2(940, 70)); 
}

/* 选择场景 */ 
void WelcomeScene::gotoSelectScene(cocos2d::Ref* pSender)
{
    // 获取屏幕大小
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 创建遮罩层
    auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 0));  // 遮罩层
    this->addChild(maskLayer);

    float duration = 0.1f; // 动画的持续时间
    float targetY = visibleSize.height; // 目标位置的Y坐标

    auto moveUp = MoveTo::create(duration, Vec2(0, targetY));
    auto callback = CallFunc::create([]() {                                   //修改此处
        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, SelectScene::create(), Color3B::BLACK)); // 切换到新场景
        });
    auto sequence = Sequence::create(moveUp, callback, nullptr);
    maskLayer->runAction(sequence);
}

void WelcomeScene::bossLock(Ref* pSender)
{
    // 获取屏幕大小
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 创建怪物锁
    auto bosslock = ui::ImageView::create("WelcomeScene/boss_lock_image.png");
    bosslock->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(bosslock);

    // 设置关闭按钮
    auto closeButton = ui::Button::create("WelcomeScene/lock_btn_normal.PNG",
        "WelcomeScene/lock_btn_selected.PNG");
    closeButton->setPosition(Vec2(270, 60));
    bosslock->addChild(closeButton); // 按钮添加到怪物锁上

    // 按钮点击事件处理逻辑
    closeButton->addClickEventListener([=](Ref*) {
        bosslock->removeFromParent(); // 移除怪物锁
        });
}

void WelcomeScene::nestLock(Ref* pSender)
{
    // 获取屏幕大小
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 创建怪物锁
    auto bosslock = ui::ImageView::create("WelcomeScene/nest_lock_image.png");
    bosslock->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(bosslock);

    // 设置关闭按钮
    auto closeButton = ui::Button::create("WelcomeScene/lock_btn_normal.PNG",
        "WelcomeScene/lock_btn_selected.PNG");
    closeButton->setPosition(Vec2(270, 60));
    bosslock->addChild(closeButton); // 按钮添加到怪物锁上

    // 按钮点击事件处理逻辑
    closeButton->addClickEventListener([=](Ref*) {
        bosslock->removeFromParent(); // 移除怪物锁
        });
}
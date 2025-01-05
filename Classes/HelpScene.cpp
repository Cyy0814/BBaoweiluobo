#include "SimpleAudioEngine.h"
#include "HelpScene.h"
#include "WelcomeScene.h"
#include <string.h>
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* HelpScene::createScene()
{
    return HelpScene::create();
}

bool HelpScene::init()
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
    auto sprite = Sprite::create("HelperScene/help_3-hd_bg.png");
    sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(sprite, 0);

    /* 设置标签 */ 
    auto to_be_continue = Label::createWithTTF("To be continue...", "fonts/arial.ttf", 30);
    to_be_continue->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(to_be_continue);

    /* 返回主菜单按钮 */
    auto backToWelcome = ui::Button::create("HelperScene/help_1-hd_67.PNG", "HelperScene/help_1-hd_38.PNG");
    backToWelcome->setPosition(Vec2(60, 580)); // 按钮初始位置
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
    this->addChild(backToWelcome, 0);
}
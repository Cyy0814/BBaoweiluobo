#include "SimpleAudioEngine.h"
#include "SettingScene.h"
#include "WelcomeScene.h"
#include <string.h>
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* SettingScene::createScene()
{
    return SettingScene::create();
}

bool SettingScene::init()
{
    /* ��ʼ������ */
    if (!Scene::init())
    {
        return false;
    }

    /* ��ȡ��Ļ��С */
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ���ñ���
    auto sprite = Sprite::create("SettingScene/SettingBG1.png");
    sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(sprite, 0);

    /* ������ǩ */
    auto to_be_continue = Label::createWithTTF("To be continue...", "fonts/arial.ttf", 30);
    to_be_continue->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 50));
    this->addChild(to_be_continue);

    /* ���������水ť */
    auto backToWelcome = ui::Button::create("SettingScene/Btn_Return.PNG", "SettingScene/Btn_ReturnLight.PNG");
    backToWelcome->setPosition(Vec2(60, 580)); // ���ð�ť�ĳ�ʼλ��
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
    this->addChild(backToWelcome, 0);
}
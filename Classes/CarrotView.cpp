#include "CarrotView.h"

USING_NS_CC;

//---------------------------------refactored with observer pattern---------------------------------//
CarrotView* CarrotView::create(Carrot* carrot) {
    CarrotView* view = new (std::nothrow) CarrotView();
    if (view && view->init(carrot)) {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool CarrotView::init(Carrot* carrot) {
    if (!Node::init()) {
        return false;
    }
    
    this->carrot = carrot;
    carrot->addObserver(this);
    
    carrotTextures = { 
        "myCarrot/HP_MAX.PNG",
        "myCarrot/HP_9.PNG",
        "myCarrot/HP_7-8.PNG",
        "myCarrot/HP_5-6.PNG",
        "myCarrot/HP_4.PNG",
        "myCarrot/HP_3.PNG",
        "myCarrot/HP_2.PNG",
        "myCarrot/HP_1.PNG" 
    };

    // 延迟3秒后创建萝卜
    auto delay = DelayTime::create(3.0f);
    auto createCarrot = CallFunc::create([this]() {
        setupSprite(Vec2::ZERO);
        setupUI();
        setupSwingAction();
    });
    auto sequence = Sequence::create(delay, createCarrot, nullptr);
    runAction(sequence);

    return true;
}

void CarrotView::setupUI() {
    // 创建血量底图精灵
    auto healthBackground = cocos2d::Sprite::create("myCarrot/Hp.png");
    healthBackground->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2 + 60));
    this->addChild(healthBackground);

    // 创建血量标签
    healthLabel = cocos2d::Label::createWithTTF("", "fonts/arial.ttf", 24);
    healthLabel->setPosition(healthBackground->getContentSize().width / 2 + 10, healthBackground->getContentSize().height / 2);
    healthLabel->setString(std::to_string(carrot->getHealth()));
    healthBackground->addChild(healthLabel);
}

void CarrotView::setupSprite(const Vec2& position) {
    carrotSprite = Sprite::create("myCarrot/HP_MAX.PNG");
    carrotSprite->setPosition(position.x + 10, position.y + 10);
    this->addChild(carrotSprite);
}

void CarrotView::setupSwingAction() {
    auto swingAction = Sequence::create(
        RotateBy::create(0.3f, 30.0f),
        RotateBy::create(0.3f, -30.0f),
        RotateBy::create(0.3f, -30.0f),
        RotateBy::create(0.3f, 30.0f),
        nullptr
    );

    auto standAction = RotateTo::create(0.3f, 0.0f);
    auto rotateAction = RepeatForever::create(
        Sequence::create(
            swingAction,
            DelayTime::create(6.0f),
            standAction,
            nullptr
        )
    );

    carrotSprite->runAction(rotateAction);
}

void CarrotView::updateHealthLabel(int health) {
    if (healthLabel) {
        healthLabel->setString(std::to_string(health));
    }
}

//---------------------------------refactored with observer pattern---------------------------------//
//---------------------------------refactored with state pattern---------------------------------//
void CarrotView::update(const std::string& message, int value) {
    if (message == "health_changed") {
        updateHealthLabel(value);
    }
    else if (message == "state_changed") {
        // 使用新的状态纹理更新外观
        if (carrotSprite && carrot) {
            carrotSprite->setTexture(carrot->getCurrentTexturePath());
        }
    }
}

CarrotView::~CarrotView() {
    if (carrot) {
        carrot->removeObserver(this);
    }
}

void CarrotView::cleanup() {
    if (carrot) {
        carrot->removeObserver(this);
    }
    Node::cleanup();
}

void CarrotView::removeFromParent() {
    if (carrot) {
        carrot->removeObserver(this);
    }
    Node::removeFromParent();
} 
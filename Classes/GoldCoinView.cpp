#include "GoldCoinView.h"

USING_NS_CC;

GoldCoinView* GoldCoinView::create(GoldCoin* goldCoin) {
    GoldCoinView* view = new (std::nothrow) GoldCoinView();
    if (view && view->init(goldCoin)) {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool GoldCoinView::init(GoldCoin* goldCoin) {
    if (!Node::init()) {
        return false;
    }
    
    this->goldCoin = goldCoin;
    goldCoin->addObserver(this);
    setupUI();

    return true;
}

void GoldCoinView::setupUI() {
    // 创建金币数量标签
    goldLabel = Label::createWithTTF("", "fonts/arial.ttf", 24);
    goldLabel->setPosition(Vec2(120, 607));
    this->addChild(goldLabel);
    
    // 设置初始值
    updateGoldLabel(goldCoin->getValue());
}

void GoldCoinView::updateGoldLabel(int value) {
    if (goldLabel) {
        goldLabel->setString(std::to_string(value));
    }
}

// Refactored with Observer Pattern
void GoldCoinView::update(const std::string& message, int value) {
    if (message == "gold_changed") {
        updateGoldLabel(value);
    }
}

GoldCoinView::~GoldCoinView() {
    if (goldCoin) {
        goldCoin->removeObserver(this);
    }
}

void GoldCoinView::cleanup() {
    if (goldCoin) {
        goldCoin->removeObserver(this);
    }
    Node::cleanup();
}

void GoldCoinView::removeFromParent() {
    if (goldCoin) {
        goldCoin->removeObserver(this);
    }
    Node::removeFromParent();
} 
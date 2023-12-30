#include "PickTower.h"
#include "BottleBullet.h"
#include "Level1.h"
using namespace cocos2d;

USING_NS_CC;

bool PickTower::init()
{
    if (!Node::init()) {
        return false;
    }

    // ��ʼ�� tower1 �� tower2
    tower1 = Sprite::create("Tower/Bottle/CanClick1.PNG", Rect(0, 0, 78, 82));
    if (tower1 != nullptr) {
        tower1->setVisible(false);
        this->addChild(tower1);
    }

    tower2 = Sprite::create("Tower/Fan/CanClick1.PNG", Rect(0, 82, 78, 78));
    if (tower2 != nullptr) {
        tower2->setVisible(false);
        this->addChild(tower2);
    }

    // ��ʼ�������ڵ�ͼ�񲢴洢��������
    for (const auto& pos : initPositions) {
        auto sprite = Sprite::create("GameScene/click.png");
        sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        sprite->setPosition(pos);
        sprite->setVisible(false); // ��ʼ����Ϊ���ɼ�
        addChild(sprite);
        gridSprites.push_back(sprite);
    }

    // ��������
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 location = touch->getLocation();

        for (auto& gridSprite : gridSprites) {
            if (gridSprite->getBoundingBox().containsPoint(location)) {
                // ��ʾ���������ͼ��Ͷ�Ӧ�� tower1, tower2
                gridSprite->setVisible(true);
                this->tower1->setPosition(gridSprite->getPosition().x, gridSprite->getPosition().y + 72);
                this->tower2->setPosition(gridSprite->getPosition().x - 72, gridSprite->getPosition().y + 72);
                this->tower1->setVisible(true);
                this->tower2->setVisible(true);
            }
            else {
                // ������������ͼ�����Ӧ�� tower1, tower2
                gridSprite->setVisible(false);
            }
        }

        return true;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

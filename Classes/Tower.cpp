#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "cocos/ui/UIImageView.h"
#include "Tower.h"


USING_NS_CC;

using namespace cocos2d::ui;
using namespace cocos2d;

Tower::Tower() : level(1), attackPower(10), attackSpeed(1.0f), towerType(0)
{
    // ��ʼ������
}

//Tower::~Tower()
//{
//    // �������
//}
Tower* Tower::create(Vec2 position,int towerType)
{
    Tower* tower = new (std::nothrow) Tower();
    if (tower && tower->initWithFile("tower_base.png"))
    {
        tower->autorelease();
        tower->towerType = towerType;
        tower->setPosition(position);
        // �������ͳ�ʼ����ͬ������
        return tower;
    }
    CC_SAFE_DELETE(tower);
    return nullptr;
}

bool Tower::init()
{
    /**������init�����������ú�����ע�ͣ��������Ϊ�ж�һ�������Ƿ�������ʼ��
     * Initializes a sprite with a texture and a rect in points, optionally rotated.
     *
     * After initialization, the offset will be (0,0).
     * @note    This is the designated initializer.
     *
     * @param   texture    A Texture2D object whose texture will be applied to this sprite.
     * @param   rect        A rectangle assigned the contents of texture.
     * @param   rotated     Whether or not the texture rectangle is rotated.
     * @return  True if the sprite is initialized properly, false otherwise.
     */
    if (!Sprite::init())
    {
        return false;
    }

    // ��ʼ����̨
    return true;
}
void Tower::attack()
{
    // ʵ�ֹ����߼�
    // ������漰������һ�� Projectile �࣬�����䷢�䵽Ŀ��
}

void Tower::upgrade()
{
    // ������̨���������
    //��ԭ�ȵ���̨�Ƴ�����������һ������̨
    
}

void Tower::remove()
{
    // �Ƴ���̨
    this->removeFromParentAndCleanup(true);
}

// �������̨���Ϳ���ͨ���̳�Tower�ಢ�����䷽����ʵ��
void Tower::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    // ����һ�����ƽڵ�
    auto drawNode = cocos2d::DrawNode::create();
    // ����Բ����ɫ��͸����
    cocos2d::Color4F color(1.0f, 0.0f, 0.0f, 0.3f); // ����ʹ�ú�ɫ��Ϊʾ��
    // ����һ��Բ�Σ���������Ϊ��Բ�����ꡢ�뾶������ʱʹ�õ���ɫ
    drawNode->drawSolidCircle(getPosition(), attackRange, 0, 20, color);
    // �����ƽڵ���ӵ�������
    getParent()->addChild(drawNode);
}

void Tower::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event) {
    // �ҵ����Ƴ�������Χ�Ļ��ƽڵ�
    auto drawNode = getParent()->getChildByName("AttackRangeNode");
    if (drawNode) {
        drawNode->removeFromParent();
    }
}

void Tower::setTowerImage(const std::string& imagePath) {
    auto texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(imagePath);
    setTexture(texture);
}
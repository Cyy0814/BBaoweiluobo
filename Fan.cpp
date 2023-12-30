// Fan.cpp
#include"Fan.h"
#include"FanBullet.h"
USING_NS_CC;



bool Fan::init(const std::string& topImageFile, const std::string& bottomImageFile) {
    if (!Sprite::init()) {
        return false;
    }

    // �����ϲ����²���Sprite����
    auto topSprite = Sprite::create(topImageFile);
    auto bottomSprite = Sprite::create(bottomImageFile);

    // ���ϲ����²���Sprite�������Ϊ�����ӽڵ㣬���������ǵ�λ��
    
    addChild(bottomSprite);
    addChild(topSprite);
    // �����ϲ����²�Sprite��λ��ƫ��
    
    bottomSprite->setPosition(Vec2(0, -bottomSprite->getContentSize().height / 2));
    topSprite->setPosition(Vec2(5, topSprite->getContentSize().height / 2 - 20));
    return true;
}


Fan* Fan::create(const Vec2& position) {
    Fan* fan = new (std::nothrow) Fan();
    if (fan && fan->init("Tower/Fan/ID4_5.PNG","Tower/Fan/ID4_0.PNG")) { // ʹ����������ͼƬ
        fan->autorelease();
        fan->setPosition(position);
        fan->attackRange = 100.0f; // ���ù�����Χ
        fan->attackSpeed = 1.0f; // ���ù����ٶ�
        fan->timeSinceLastAttack = 0;
        return fan;
    }
    CC_SAFE_DELETE(fan);
    return nullptr;
}

void Fan::update(float dt, std::vector<Monster*> monsters) {
    timeSinceLastAttack += dt;
    if (timeSinceLastAttack >= attackSpeed) {
        checkForMonstersInRange(monsters);
        if (!monstersInRange.empty()) {
            attack(monstersInRange.front()); // ������һ������
            timeSinceLastAttack = 0;
        }
    }
}

bool Fan::isMonsterInRange(Monster* monster) {
    return (getPosition().distance(monster->getPosition()) <= attackRange);
}

void Fan::checkForMonstersInRange(std::vector<Monster*> monsters) {
    // ���� monsters �ǳ��������й�����б�
    monstersInRange.clear();
    if (monsters.size())
    {
        for (Monster*& monster : monsters) {
            if (isMonsterInRange(monster)) {
                monstersInRange.push_back(monster);
            }
        }
    }
}

void Fan::attack(Monster* target) {
    auto fanbullet = FanBullet::createWithTarget(target, "Resource/Tower/Fan/ID1_0.PNG", 1000.0f, 10);
    fanbullet->setPosition(getPosition());
    this->getParent()->addChild(fanbullet);
}

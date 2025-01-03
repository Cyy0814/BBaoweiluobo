#ifndef __CARROT_VIEW_H__
#define __CARROT_VIEW_H__

#include "cocos2d.h"
#include "Observer.h"
#include "Carrot.h"

class CarrotView : public cocos2d::Node, public Observer {
private:
    cocos2d::Label* healthLabel;
    cocos2d::Sprite* healthBackground;
    cocos2d::Sprite* carrotSprite;
    std::vector<std::string> carrotTextures;
    Carrot* carrot;

public:
    static CarrotView* create(Carrot* carrot);
    virtual bool init(Carrot* carrot);
    virtual ~CarrotView();
    
    // UI和外观相关
    void setupUI();
    void setupSprite(const Vec2& position);
    void setupSwingAction();
    void updateHealthLabel(int health);
    
    // 观察者接口实现
    virtual void update(const std::string& message, int value) override;
    
    // 清理方法
    void cleanup() override;
    void removeFromParent() override;
};

// 未完整实现的音效系统，仅用来表明可以添加的观察者
class SoundEffect : public Observer {
    void update(const string& msg, int value) {
        if(msg == "monster_died") 
            playDeathSound();
    }
};

// 未完整实现的成就系统，仅用来表明可以添加的观察者
class AchievementSystem : public Observer {
    void update(const string& msg, int value) {
        if(msg == "carrot_health_changed" && value <= 1)
            unlockAchievement("close_call");
    }
};
#endif 
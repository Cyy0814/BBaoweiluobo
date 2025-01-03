#include "SingleWave.h"
#include "cocos2d.h"

USING_NS_CC;

//---------------------------------refactored with composite---------------------------------//
void SingleWave::startWave() {
    // 叶子节点直接处理怪物生成
    int monsterIndex = 0;
    auto scheduler = Director::getInstance()->getScheduler();
    
    scheduler->schedule([this, monsterIndex](float dt) mutable {
        if (monsterIndex < monsters.size()) {
            Monster* monster = monsters[monsterIndex++];
            monster->setVisible(true);
            monster->startMoving();
        }
    }, this, spawnInterval, monsters.size() - 1, 0, false, "spawn_monster");
}

void SingleWave::addMonster(Monster* monster) {
    // 叶子节点直接管理怪物
    monster->setVisible(false);
    monsters.push_back(monster);
} 
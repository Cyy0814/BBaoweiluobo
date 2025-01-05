#include "Level1.h"
#include "CompositeWave.h"
#include "SingleWave.h"
#include "Monster.h"

Scene* Level1Scene::createScene() {
    return Level1Scene::create();
}

bool Level1Scene::init() {
    if (!BaseLevelScene::init()) {
        return false;
    }
    return true;
}

void Level1Scene::initPath(GameMap* gameMap) {
    vector<GameMap::Grid> path_ = {
        { 1,5 }, { 1,4 }, { 1,3 }, { 1,2 }, { 2,2 },
        { 3,2 }, { 4,2 }, { 4,3 }, { 5,3 }, { 6,3 },
        { 7,3 }, { 7,2 }, { 8,2 }, { 9,2 }, { 10,2 },
        { 10,3 }, { 10,4 }, { 10,5 }
    };
    
    for (const auto& grid : path_) {
        gameMap->addPathPoint(grid);
    }
}

/*Refactored with Composite Pattern*/
void Level1Scene::setupWaves() {

    for (int waveIndex = 0; waveIndex < getTotalWaves(); ++waveIndex) {
        // 创建主波次(组合波次)
        auto compositeWave = new CompositeWave();
        
        // 每个主波次包含2个子波次
        for (int subWaveIndex = 0; subWaveIndex < 2; ++subWaveIndex) {
            auto singleWave = new SingleWave();
            
            // 根据波次索引设置怪物类型和数量
            int monsterType = (waveIndex % 3) + 1;  // 循环使用3种怪物类型
            int monsterCount = 4 + (waveIndex % 3); // 每波4-6个怪物
            
            // 创建怪物并添加到子波次中
            for (int i = 0; i < monsterCount; ++i) {
                auto monster = Monster::createWithType(monsterType);
                monster->setPosition(path.front());
                this->addChild(monster);
                monster->setVisible(false);
                monster->setMoveCallback([this, monster]() {
                    monster->moveOnPath(this->path);
                });
                
                singleWave->addMonster(monster);
            }
            
            // 将子波次添加到主波次中
            compositeWave->addWave(singleWave);
        }
        
        // 将主波次添加到波次管理器
        waveManager->addWave(compositeWave);
    }
}
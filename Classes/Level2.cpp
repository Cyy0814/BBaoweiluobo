#include "Level2.h"
#include "CompositeWave.h"
#include "SingleWave.h"
#include "Monster.h"

/*Refactored with Factory Pattern*/

Scene* Level2Scene::createScene() {
    return Level2Scene::create();
}

bool Level2Scene::init() {
    if (!BaseLevelScene::init()) {
        return false;
    }
    return true;
}

void Level2Scene::initPath(GameMap* gameMap) {
    // 定义关卡2的路径点
    vector<GameMap::Grid> path_ = {
        { 4,5 }, { 5,5 }, { 6,5 }, { 7,5 }, { 8,5 },
        { 9,5 }, { 9,4 }, { 9,3 }, { 8,3 }, { 7,3 },
        { 6,3 }, { 5,3 }, { 4,3 }, { 3,3 }, { 2,3 },
        { 2,2 }, { 2,1 }, { 3,1 }, { 4,1 }, { 5,1 },
        { 6,1 }, { 7,1 }
    };
    
    for (const auto& grid : path_) {
        gameMap->addPathPoint(grid);
    }
}

/*Refactored with Composite Pattern*/
void Level2Scene::setupWaves() {
    for (int waveIndex = 0; waveIndex < getTotalWaves(); ++waveIndex) {
        // 创建主波次(组合波次)
        auto compositeWave = new CompositeWave();
        
        // Level2 的特殊波次设置：每波包含3个子波次，怪物更强
        for (int subWaveIndex = 0; subWaveIndex < 3; ++subWaveIndex) {
            auto singleWave = new SingleWave();
            
            // 创建该波次的怪物（Level2 的怪物更强）
            int monsterType = ((waveIndex + 1) % 3) + 1;
            int monsterCount = 5 + (waveIndex % 4); // 比 Level1 多一个怪物
            
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
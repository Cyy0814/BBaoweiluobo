#include "Level2.h"

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
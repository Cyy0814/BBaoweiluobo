#include "Level1.h"

/*Refactored with Factory Pattern*/

Scene* Level1Scene::createScene()
{
    return Level1Scene::create();
}

bool Level1Scene::init()
{
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
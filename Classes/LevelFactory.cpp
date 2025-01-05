#include "LevelFactory.h"
#include "Level1.h"
#include "Level2.h"

/**
 * @brief 根据关卡编号创建对应的关卡场景
 * 
 * @param levelNum 关卡编号
 * @return BaseLevelScene* 关卡场景实例
 */
BaseLevelScene* LevelFactory::createLevel(int levelNum) {
    // 参数检查
    if (levelNum < 1 || levelNum > MAX_LEVEL) {
        CCLOG("Invalid level number: %d", levelNum);
        return nullptr;
    }

    BaseLevelScene* level = nullptr;
    
    // 根据关卡号创建对应场景
    switch(levelNum) {
        case 1:
            level = Level1Scene::create();
            break;
        case 2:
            level = Level2Scene::create();
            break;
        default:
            CCLOG("Level %d not implemented yet", levelNum);
            break;
    }
    
    return level;
} 
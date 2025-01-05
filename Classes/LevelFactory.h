#ifndef __LEVEL_FACTORY_H__
#define __LEVEL_FACTORY_H__

#include "cocos2d.h"
#include "BaseLevelScene.h"

//---------------------------------refactored with factory pattern---------------------------------//
/**
 * @brief 关卡工厂类
 * 
 * 负责创建不同的关卡场景实例
 */
class LevelFactory {
public:
    /**
     * @brief 创建指定编号的关卡
     * @param levelNum 关卡编号
     * @return 关卡场景实例
     * @throws 当关卡号无效时返回nullptr
     */
    static BaseLevelScene* createLevel(int levelNum);

private:
    // 最大关卡数
    static const int MAX_LEVEL = 2;
    
    // 禁止实例化
    LevelFactory() {}
};

#endif 
#ifndef GLOBAL_H
#define GLOBAL_H

#include "ui/CocosGUI.h"
#include "Carrot.h"
#include "CarrotView.h"
#include "Monster.h"
#include "GoldCoin.h"
#include "GoldCoinView.h"
#include "Fan.h"
#include "Bottle.h"
#include "Shit.h"


extern Carrot* globalCarrot;      // 全局变量胡萝卜
extern GoldCoin* goldCoin;        // 金币系统
extern CarrotView* globalCarrotView;
extern GoldCoinView* goldCoinView;

extern std::vector<Monster*> monsters; // 存储怪物

// 防御塔
extern std::vector<Fan*> fans;
extern std::vector<Bottle*> bottles;
extern std::vector<Shit*> shits;

// 记录是否完成关卡
extern bool isLevel1Finish;
extern bool isLevel2Finish;

#endif
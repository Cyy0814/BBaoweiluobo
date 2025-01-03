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


extern Carrot* globalCarrot; // ȫ�ֱ���������
extern GoldCoin* goldCoin; // ���ϵͳ
extern CarrotView* globalCarrotView;
extern GoldCoinView* goldCoinView;

extern std::vector<Monster*> monsters; // 洢

// 潨̨
extern std::vector<Fan*> fans;
extern std::vector<Bottle*> bottles;
extern std::vector<Shit*> shits;

// ��¼�Ƿ���ɹؿ�
extern bool isLevel1Finish;
extern bool isLevel2Finish;

#endif
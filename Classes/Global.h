#ifndef GLOBAL_H
#define GLOBAL_H

#include "ui/CocosGUI.h"
#include "Carrot.h"
#include "Monster.h"
#include "GoldCoin.h"
#include "Fan.h"
#include "TowerBottle.h"
#include "Shit.h"


extern Carrot* globalCarrot; // ȫ�ֱ���������
extern GoldCoin* goldCoin; // ���ϵͳ

extern std::vector<Monster*> monsters; // �����洢���������
extern std::vector<Fan*> fans;
extern std::vector<Bottle*> bottles;
extern std::vector<Shit*> shits;

#endif
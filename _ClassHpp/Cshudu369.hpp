

#include"../_ClassHpp/Cshudu.hpp"

#pragma once
#include<stack>
#include<set>
#include<vector>
#include<map>
#include<algorithm>
#include"../shudu2Dlg.h"

class CSHUDU369
{
public:
	CSHUDU369(BYTE(*arry)[9][9]);
	~CSHUDU369();
	int init();
	int work();
	int onlyNumBlock();//求解所有块的不存在多义性的格子
	int OnlyNum(vector<tagBox*>* v);
	int gongAlg(vector<tagBox*>* v);
	bool upData(int _value, tagBox* b);
	int upBlock(CSHUDU* c);
	bool setBitInfo(tagBox* b, int val);
	void getRowCell(tagBox* a, set<tagBox*>& s, bool second = false);
	void getColCell(tagBox* a, set<tagBox*>& s, bool second = false);
	void getGongCell(tagBox* a, set<tagBox*>& s, bool second = false);
	void getAlCell(tagBox* a, set<tagBox*>& s, bool second = false);
	void ctFind(tagBox* a, set<tagBox*>& s);
	int	analysis();
	bool dfs(tagBox* p, int no = -1);
	bool resetBit(tagBox* b);
	CSHUDU* getMainBlock(tagBox* p);
	CSHUDU* getSecondBlock(tagBox* p);
	//如果是那四个区域的公共格子
	bool isUnioCell(tagBox* p) { return p->blockId == CT && p->blockId2 != UNKNOW; };
	//布尔值用来确定寻找依据是否是主块还是副块
	vector<tagBox*>* getAboutVec(tagBox* a, bool second = false);
private:
	int bCheckLine_ = false;
	int _countFalse = 0;
	CSHUDU* _lt;
	CSHUDU* _rt;
	CSHUDU* _ct;
	CSHUDU* _ld;
	CSHUDU* _rd;
};
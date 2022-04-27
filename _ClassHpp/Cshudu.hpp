#pragma once
#include<stack>
#include<set>
#include<vector>
#include<map>
#include<algorithm>
#include"../shudu2Dlg.h"

#define MAXFALSE 7000000
using namespace std;

enum MyEnum
{
	LT = 0,
	RT,
	CT,
	LD,
	RD,
	UNKNOW
};

template <class T, class T1>
void mapToVec(T& m, T1& vec)
{
	for (auto it = m.begin(); it != m.end(); it++)
		for (tagBox*b : it->second)
			vec.push_back(b);
}
//数独的每一个格子
struct tagBox
{
	tagBox::tagBox(int _row, int _col, BYTE* _p);
	//跟另一个格子是否有关联.同行，列,宫
	bool isRel(tagBox* b);
	bool isRe2(tagBox* b);
	//给出可能是的数
	vector<int> getPosb();
	bool operator < (const tagBox& x)
	{
		return countPosb < x.countPosb;
	}
	bool exclude(int v);
	bool posbv(int v) { return r[v]; };
	int row;
	int col;
	int gong = 999;//在第几个宫
	int countPosb = 9;
	int value = 0;//已经确定的值
	BYTE* ptr_;//绑定一个地址
	BYTE* ptr2_ = NULL;//公共格子绑两个
	//在其他块中的横纵坐标
	int row2 = 999;
	int col2 = 999;
	int gong2 = 999;
	MyEnum blockId = UNKNOW;
	MyEnum blockId2 = UNKNOW;//同时又是另外一个块的
private:
	bool r[10];
};

class CSHUDU
{
public:
	CSHUDU(BYTE(*arry)[9], MyEnum bid);
	~CSHUDU();
	//找出能直接确定的,每行或者列
	int OnlyNum();
	//宫的检查
	int gongAlg();
	//初始化,参数二为关联其他块的格子
	int init(MyEnum blockId,vector<tagBox*>* vCtcell = NULL);
	//
	int work();
	//完全确认准确的时候更新数据.
	bool upData(int _value, tagBox* b, bool second = false);
	//算法，解析
	int parse();
	//深度优先
	bool dfs(tagBox* p, int no = -1);
	//广度优先
	bool bfs(tagBox* p, int no = -1);
	//更新debug数组，标记位.
	bool setBitInfo(tagBox* b, int val, bool second = false);
	int getGid(int& row, int& col);
	//
	int getBit(int arr[3], int count, int val, int& arrId, int& off);
	bool resetBit(tagBox* b);
	int lineCell(tagBox* a);
	void getRowCell(tagBox* a, set<tagBox*>& s, bool second = false);
	void getColCell(tagBox* a, set<tagBox*>& s, bool second = false);
	void getGongCell(tagBox* a, set<tagBox*>& s, bool second = false);
	void getAlCell(tagBox* a, set<tagBox*>& s, bool second = false);
	BYTE(*getDbgarry())[9]{return dbgArry_;}
	vector<tagBox*>* getAll() { return &_alBox; }
	MyEnum _blockId = UNKNOW;
private:
	bool bCheckLine_;
	vector<tagBox*> _alBox;
	BYTE(*dbgArry_)[9];
	//用一个bit来记录每行，列，宫的数字是否存在。  81个bit，10个字节，三个整型
	int checkRow_[3];
	//列
	int checkCol_[3];
	//宫
	int checkGong_[3];
	//对角线
	int checkLine_;
	int _countFalse;
};


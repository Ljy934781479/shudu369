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
//������ÿһ������
struct tagBox
{
	tagBox::tagBox(int _row, int _col, BYTE* _p);
	//����һ�������Ƿ��й���.ͬ�У���,��
	bool isRel(tagBox* b);
	bool isRe2(tagBox* b);
	//���������ǵ���
	vector<int> getPosb();
	bool operator < (const tagBox& x)
	{
		return countPosb < x.countPosb;
	}
	bool exclude(int v);
	bool posbv(int v) { return r[v]; };
	int row;
	int col;
	int gong = 999;//�ڵڼ�����
	int countPosb = 9;
	int value = 0;//�Ѿ�ȷ����ֵ
	BYTE* ptr_;//��һ����ַ
	BYTE* ptr2_ = NULL;//�������Ӱ�����
	//���������еĺ�������
	int row2 = 999;
	int col2 = 999;
	int gong2 = 999;
	MyEnum blockId = UNKNOW;
	MyEnum blockId2 = UNKNOW;//ͬʱ��������һ�����
private:
	bool r[10];
};

class CSHUDU
{
public:
	CSHUDU(BYTE(*arry)[9], MyEnum bid);
	~CSHUDU();
	//�ҳ���ֱ��ȷ����,ÿ�л�����
	int OnlyNum();
	//���ļ��
	int gongAlg();
	//��ʼ��,������Ϊ����������ĸ���
	int init(MyEnum blockId,vector<tagBox*>* vCtcell = NULL);
	//
	int work();
	//��ȫȷ��׼ȷ��ʱ���������.
	bool upData(int _value, tagBox* b, bool second = false);
	//�㷨������
	int parse();
	//�������
	bool dfs(tagBox* p, int no = -1);
	//�������
	bool bfs(tagBox* p, int no = -1);
	//����debug���飬���λ.
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
	//��һ��bit����¼ÿ�У��У����������Ƿ���ڡ�  81��bit��10���ֽڣ���������
	int checkRow_[3];
	//��
	int checkCol_[3];
	//��
	int checkGong_[3];
	//�Խ���
	int checkLine_;
	int _countFalse;
};



#include <pch.h>
#include "../_ClassHpp/Cshudu369.hpp"

bool cmp1(tagBox* a, tagBox* b);

bool CSHUDU369::setBitInfo(tagBox* b, int val)
{
	if (b->blockId == CT && b->blockId2 == RD && b->row2 == 2 && b->col2 == 1)
		int z = 0;
	//必须要保证两个块都是设置成功的才行
	CSHUDU* b1 = getMainBlock(b);
	CSHUDU* b2 = getSecondBlock(b);
	if (!b1->setBitInfo(b, val))
		return false;
	if (b2 == nullptr)
		return true;
	if (b2->setBitInfo(b, val,true))
		return true;
	else
	{
		b1->resetBit(b);
		return false;
	}
	return false;
}

void CSHUDU369::getRowCell(tagBox* a, set<tagBox*>& s, bool second)
{
	vector<tagBox*>* v = getAboutVec(a, second);
	int row = second ? a->row2 : a->row;
	for (int i = 0; i < 9; i++)
	{//同一行的
		tagBox* ref = (*v)[row * 9 + i];
		if (ref == a || ref->value)
			continue;
		s.insert(ref);
	}
}

void CSHUDU369::getColCell(tagBox* a, set<tagBox*>& s, bool second)
{
	vector<tagBox*>* v = getAboutVec(a, second);
	int col = second ? a->col2 : a->col;
	for (int i = col; i < 81; i += 9)
	{//同一列的
		tagBox* ref = (*v)[i];
		if (ref == a || ref->value)
			continue;
		s.insert(ref);
	}
}

void CSHUDU369::getGongCell(tagBox* a, set<tagBox*>& s, bool second)
{
	vector<tagBox*>* v = getAboutVec(a, second);
	int gong = second ? a->gong2 : a->gong;
	int col = second ? a->col2 : a->col;
	//同一个宫的
	int index = (gong - 1) / 3 * 27 + col / 3 * 3;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			tagBox* ref = (*v)[index + i * 9 + j];
			if (ref == a || ref->value)
				continue;
			s.insert(ref);
		}
}

void CSHUDU369::getAlCell(tagBox* a, set<tagBox*>& s, bool second)
{
	getRowCell(a, s, second);
	getColCell(a, s, second);
	getGongCell(a, s, second);
}

void CSHUDU369::ctFind(tagBox* a, set<tagBox*>& s)
{
#define CTFIND\
	if (ref == a || ref->value)\
		continue;\
	if (!ref->posbv(a->value) || ref->blockId ==CT)\
		continue;\
	s.insert(ref);\

	vector<tagBox*>* v = getAboutVec(a, true);
	for (int i = a->col2; i < 81; i += 9)
	{//同一列的
		tagBox* ref = (*v)[i];
		CTFIND
	}
	for (int i = 0; i < 9; i++)
	{//同一行的
		tagBox* ref = (*v)[a->row2 * 9 + i];
		CTFIND
	}
}

int CSHUDU369::analysis()
{
#define GETUNKNOW(v,s)\
	for(tagBox* a : *v)\
	{\
		if(a->value || (a->blockId == CT && a->blockId2 != UNKNOW ))\
			continue;\
		s.push_back(a);\
	}\
		
	vector<tagBox*> vec;
	GETUNKNOW(_ct->getAll(), vec)
	GETUNKNOW(_lt->getAll(), vec)
	GETUNKNOW(_rt->getAll(), vec)
	GETUNKNOW(_ld->getAll(), vec)
	GETUNKNOW(_rd->getAll(), vec)
	sort(vec.begin(), vec.end(), cmp1);
	if(vec.size())
		dfs(vec[0]);
	return 0;
}

vector<tagBox*>* CSHUDU369::getAboutVec(tagBox* a, bool second)
{
	MyEnum bid = second ? a->blockId2 : a->blockId;
	if (bid == LT)
		return _lt->getAll();
	if (bid == RT)
		return _rt->getAll();
	if (bid == CT)
		return _ct->getAll();
	if (bid == LD)
		return _ld->getAll();
	if (bid == RD)
		return _rd->getAll();
	return nullptr;
}

CSHUDU369::CSHUDU369(BYTE(*arry)[9][9])
{
	_ct = new CSHUDU(arry[2], CT);
	_lt = new CSHUDU(arry[0], LT);
	_rt = new CSHUDU(arry[1], RT);
	_ld = new CSHUDU(arry[3], LD);
	_rd = new CSHUDU(arry[4], RD);
}

CSHUDU369::~CSHUDU369()
{
	delete _lt;
	delete _rt;
	delete _ct;
	delete _ld;
	delete _rd;
}

int CSHUDU369::init()
{
	_ct->init(CT);//先new出中间块的所有格子
	//其他四个块陆续new出格子,只不过如果是和中间块的格子共同拥有的就不再nwe出来,而是从中间块的取
	_lt->init(LT, _ct->getAll());
	_rt->init(RT, _ct->getAll());
	_ld->init(LD, _ct->getAll());
	_rd->init(RD, _ct->getAll());
	return 0;
}

int CSHUDU369::work()
{
	init();
	int x = onlyNumBlock();
	analysis();
	return _countFalse;
}

int CSHUDU369::onlyNumBlock()
{
	vector<CSHUDU*> v;
	v.reserve(5);
	v.push_back(_lt);
	v.push_back(_rt);
	v.push_back(_ld);
	v.push_back(_rd);
	v.push_back(_ct);
	CSHUDU* it = v[0];
	int cid = 0;
	int total = 0;
	while (1)
	{
		if (it == _rt)
			int ww = 0;
		int c1 = upBlock(it);
		total += c1;
		if (it == _ct && c1 == 0)
			break;//一直重复到中间的块没有任何更新
		cid = (cid + 1) % 5;
		it = v[cid];
	}
	return total;
}

int CSHUDU369::OnlyNum(vector<tagBox*>* v)
{
	int total = 0;
	vector<tagBox*> vRound;
	for (tagBox* a : *v)
		if (!a->value)
			vRound.push_back(a);
	while (vRound.size())
	{//遍历每一个不确定值的，尝试找出
		vector<tagBox*> vNext;
		int count = 0;
		for (tagBox* a : vRound)
		{
			if (a->countPosb == 1)
			{//如果可能的数字只有一个
				int z = a->getPosb()[0];
				if (upData(z, a))
					count++;//只要有新的,就可能还有连带关系,直到找不到任何更新
			}
			else
				vNext.push_back(a);
		}
		if (!count)
			break;
		total += count;
		vRound = vNext;
	}
	return total;
}

int CSHUDU369::gongAlg(vector<tagBox*>* v)
{
	int total = 0;
	for (int i = 1; i < 10; i++)//每一个宫内的检查
	{//用一个数组来记录这个宫是否已经存在某个数字
		bool r[10] = { false };
		vector<tagBox*> vUnknow;
		int offest = i % 3;
		int index = (i - 1) / 3 * 27 + (offest ? (offest - 1) * 3 : 6);
		for (int row = 0; row < 3; row++)
			for (int j = 0; j < 3; j++)
			{
				tagBox* a = (*v)[index + row * 9 + j];
				if (a->value)//确定值就标记
					r[a->value] = true;
				else
					vUnknow.push_back(a);
			}
		for (int j = 1; j < 10; j++)
		{//如果这个宫内已经有这个值
			if (r[j])
				continue;
			vector<tagBox*> v1;
			for (tagBox* a : vUnknow)
				if (a->posbv(j))
					v1.push_back(a);
			//如果可能是数值j的格子只有一个,那么就可以确定
			if (v1.size() != 1)
				continue;
			if (upData(j, v1[0]))
				total++;
		}
	}
	return total;
}

bool CSHUDU369::upData(int _value, tagBox* b)
{
	bool bset = setBitInfo(b, _value);
	if (bset == false)
		return false;
	set<tagBox*> s;
	getAlCell(b, s);
	//如果是公共部分的格子,则还要更新另一个块的数据
	if (isUnioCell(b))
		getAlCell(b, s, true);
	for (tagBox* t : s)
		t->exclude(_value);
	return true;
}

int CSHUDU369::upBlock(CSHUDU* c)
{
	int cn = 0;
	while (1)
	{//每个块内的行列宫循环更新
		int n = 0;
		n += OnlyNum(c->getAll());
		n += gongAlg(c->getAll());
		cn += n;
		if (n == 0)
			break;
	}
	return cn;
}

bool CSHUDU369::dfs(tagBox* p, int no)
{
#define DFSTEMP(s,b)\
	for(auto it = s.begin();it!=s.end() && b==true;it++)\
	{\
		count++;\
		if (dfs(*it, i))\
			ok++;\
		else\
		{\
			resetBit(p);\
			bok = false;\
			_countFalse++;\
		}\
	}\

	set<tagBox*> sRow;
	set<tagBox*> sCol;
	getRowCell(p, sRow);
	getColCell(p, sCol);
	if (isUnioCell(p))
	{
		getRowCell(p, sRow,true);
		getColCell(p, sCol,true);
	}
	if (sRow.size() == 0 && sCol.size() == 0)
		int z = 0;
	for (int i = 1; i < 10; i++)
	{//逐一猜测
		if (i == no || p->posbv(i) == false || !setBitInfo(p, i))
			continue;
		int count = 0;
		int ok = 0;
		bool bok = true;
		for (auto it = sRow.begin(); it != sRow.end() && bok == true; it++)
		{
			count++; 
			if (dfs(*it, i))
				ok++; 
			else
			{
				resetBit(p); 
				bok = false; 
				_countFalse++; 
			}
		}

		for (auto it = sCol.begin(); it != sCol.end() && bok == true; it++)
		{
			count++; 
			if (dfs(*it, i))
				ok++; 
			else
			{
				resetBit(p); 
				bok = false; 
				_countFalse++; 
			}
		}

		if (count == ok)
			return true;
	}
	return false;
}

bool CSHUDU369::resetBit(tagBox* b)
{
	CSHUDU* b1 = getMainBlock(b);
	CSHUDU* b2 = getSecondBlock(b);
	b1->resetBit(b);
	if (b2)
		b2->resetBit(b);
	return true;
}

CSHUDU* CSHUDU369::getMainBlock(tagBox* p)
{
	if (p->blockId == CT)
		return _ct;
	if (p->blockId == LT)
		return _lt;
	if (p->blockId == RT)
		return _rt;
	if (p->blockId == LD)
		return _ld;
	if (p->blockId == RD)
		return _rd;
	return nullptr;
}

CSHUDU* CSHUDU369::getSecondBlock(tagBox* p)
{
	if(p->blockId2==UNKNOW)
		return nullptr;
	if (p->blockId2 == CT)
		return _ct;
	if (p->blockId2 == LT)
		return _lt;
	if (p->blockId2 == RT)
		return _rt;
	if (p->blockId2 == LD)
		return _ld;
	if (p->blockId2 == RD)
		return _rd;
	return nullptr;
}

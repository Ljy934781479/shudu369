
#include <pch.h>
#include<map>
#include<queue>
#include "../_ClassHpp/Cshudu.hpp"
#include <wtypes.h>

bool cmp1(tagBox* a, tagBox* b)
{
	return a->countPosb < b->countPosb;
}

tagBox::tagBox(int _row, int _col, BYTE* _p)
{
	row = _row;
	col = _col;
	value = *_p;
	ptr_ = _p;
	//����ڵڼ�����
	int cid = col / 3 + 1;
	int rid = row / 3 + 1;
	gong = (rid - 1) * 3 + cid;
	memset(r, true, 10);
}

vector<int> tagBox::getPosb()
{
	vector<int> result;
	for (int i = 1; i < 10; i++)
		if (r[i])
			result.push_back(i);
	countPosb = result.size();
	return result;
}

bool tagBox::isRel(tagBox* b)
{
	if (this == b)
		return false;
	if (b->col == col || b->row == row || b->gong == gong)
		return true;
	return false;
}

bool tagBox::isRe2(tagBox* b)
{
	if (this == b)
		return false;
	if (b->col == col2 && b->row == row2 && b->gong == gong2 && blockId2 == b->blockId)
		return true;
	return false;
}

bool tagBox::exclude(int v)
{
	if (row == 8 && col == 8 && v == 6 && blockId == RT)
		int z = 0;
	if (!r[v])
		return false;
	countPosb -= 1;
	r[v] = false;
	return true;
}

bool CSHUDU::upData(int _value, tagBox* b, bool second)
{
	if (b->blockId == CT && b->row == 8 && b->col == 7 && _value == 5)
		int z = 0;
	bool bset = setBitInfo(b, _value, second);
	if (!bset && b->countPosb == 1)
	{
		bCheckLine_ = false;//����������ֻʣ��һ�ֿ��ܲ�������ʧ��,�����û��������Ѿ�������Խ���������
		setBitInfo(b, _value, second);
	}
	set<tagBox*> s;
	getAlCell(b, s, second);
	for (tagBox* t : s)
		t->exclude(_value);
	return true;
}

//----------------------------------------------------------------------------------------

CSHUDU::CSHUDU(BYTE(*arry)[9],MyEnum bid)
{
	_countFalse = 0;
	dbgArry_ = arry;
	memset(checkRow_, 0, 12);
	memset(checkCol_, 0, 12);
	memset(checkGong_, 0, 12);
	//��ҪҪ������Խ���.
	bCheckLine_ = false;//369�߽װ汾��Ҫ����Խ��ߵĻ��Ѷ�̫���ˣ��ȹص�
	_blockId = bid;
	checkLine_ = 0;
}

CSHUDU::~CSHUDU()
{
	for (tagBox* a : _alBox)
		delete a;
}

//---4420
int CSHUDU::OnlyNum()
{
	int total = 0;
	vector<tagBox*> vRound;
	for (tagBox* a : _alBox)
		if (!a->value)
			vRound.push_back(a);
	while (vRound.size())
	{//����ÿһ����ȷ��ֵ�ģ������ҳ�
		vector<tagBox*> vNext;
		int count = 0;
		for (tagBox* a : vRound)
		{
			if (a->countPosb == 1)
			{//������ܵ�����ֻ��һ��
				int z = a->getPosb()[0];
				if(upData(z, a))
					count++;//ֻҪ���µ�,�Ϳ��ܻ���������ϵ,ֱ���Ҳ����κθ���
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

int CSHUDU::gongAlg()
{
	int total = 0;
	for (int i = 1; i < 10; i++)//ÿһ�����ڵļ��
	{//��һ����������¼������Ƿ��Ѿ�����ĳ������
		bool r[10] = { false };
		vector<tagBox*> vUnknow;
		int offest = i % 3;
		int index = (i - 1) / 3 * 27 + (offest ? (offest - 1) * 3 : 6);
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
			{
				tagBox* a = _alBox[index + i * 9 + j];
				int v = a->value;
				if (v)//ȷ��ֵ�ͱ��
					r[v] = true;
				else
					vUnknow.push_back(a);
			}
		for (int j = 1; j < 10; j++)
		{//�����������Ѿ������ֵ
			if (r[j])
				continue;
			vector<tagBox*> v1;
			for (tagBox* a : vUnknow)
				if (a->posbv(j))
					v1.push_back(a);
			//�����������ֵj�ĸ���ֻ��һ��,��ô�Ϳ���ȷ��
			if (v1.size() != 1)
				continue;
			if(upData(j, v1[0]))
				total++;
		}
	}
	return total;
}


int CSHUDU::init(MyEnum blockId, vector<tagBox*>* vCtcell)
{//���������и���,��������ǹ������沿������Ǹ����ж�Ӧ�ĸ�ֵ
#define INITTEMP(a,bid)\
	{\
		b = (*vCtcell)[a];\
		b->row2 = i;\
		b->col2 = j;\
		b->gong2 = getGid(i,j);\
		b->blockId = CT;\
		b->blockId2 = bid;\
		b->ptr2_ = &dbgArry_[i][j];\
	}\

	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{//���ĸ������ǹ��ø��ӵ�,�Ͳ���new��,���м���ǿ���
			tagBox* b = NULL;
			if (blockId == LT && i > 5 && j > 5)
				INITTEMP((i - 6) * 9 + j - 6,LT)    //���Ͽ�e
			else if(blockId == RT && i > 5 && j < 3)
				INITTEMP(6 + (i - 6) * 9 + j,RT)	//���Ͽ�
			else if (blockId == LD && i < 3 && j > 5)
				INITTEMP(54 + i * 9 + j - 6,LD)	//���¿�
			else if (blockId == RD && i <3 && j < 3)
				INITTEMP(60 + i * 9 + j,RD)		//���¿�
			else
			{
				b = new tagBox(i, j, &dbgArry_[i][j]);
				b->blockId = blockId;
			}
			_alBox.push_back(b);
		}
	for (tagBox* a : _alBox)
	{
		if (!a->value)
			continue;
		bool second = false;
		if (a->blockId == CT && a->blockId2 != UNKNOW)
			second = true;
		upData(a->value, a,second);
	}
	return 0;
}

int CSHUDU::work()
{
	int total = 0;
	while (1)
	{//�������ÿ�������У���.  ֱ����Ҳ�Ҳ��������ĸ���
		int upCount = 0;
		upCount += OnlyNum();
		upCount += gongAlg();
		if (upCount == 0)
			break;
		total += upCount;
	}
	return total;
}

int CSHUDU::parse()
{
	vector<tagBox*> vec;
	for (tagBox* it : _alBox)
	{
		if (it->value)
			continue;
		vec.push_back(it);
	}
	if (vec.size()==0)
		return 999;
	/**************************************/
	sort(vec.begin(),vec.end(), cmp1);
	//return dfs(vec[0]);//�����
	return 0;
}

bool CSHUDU::dfs(tagBox* p, int no)
{
	return false;
}

bool CSHUDU::bfs(tagBox* p, int no)
{
	return false;
}

int CSHUDU::getGid(int& row, int& col)
{
	//����ڵڼ�����
	int cid = col / 3 + 1;
	int rid = row / 3 + 1;
	return (rid - 1) * 3 + cid;
}

int CSHUDU::getBit(int arr[3], int count, int val,int& arrId, int& off)
{
	arrId = count * 9 + val - 1;//bitλ�ڵڼ���
	int r = arrId % 32;//����
	arrId = arrId / 32;//�ڵڼ���int.
	off = 32 - r - 1;//ƫ����
	return arr[arrId] << r >> 31;
}

bool CSHUDU::setBitInfo(tagBox* b, int val,bool second)
{
	if (second)
		int  o = 0;
	int row = second ? b->row2 : b->row;
	int col = second ? b->col2 : b->col;
	int gong = second ? b->gong2 : b->gong;
	int dl = lineCell(b);
	if (bCheckLine_)
	{
		if ((dl == 1 || dl == 3) && (checkLine_ << (val - 1) >> 31))
			return false;
		if ((dl == 2 || dl == 3) && (checkLine_ << (8 + val) >> 31))
			return false;
	}
	//�õ�bit�±�
	int o1, o2, o3 = 0;
	int i1, i2, i3 = 0;
	bool r1 = getBit(checkRow_, row, val, i1, o1);
	bool c1 = getBit(checkCol_, col, val, i2, o2);
	bool g1 = getBit(checkGong_, gong, val, i3, o3);
	//���bitλ,�����Ϊ0,����ʧ��
	if (r1 || c1 || g1)
		return false;
	if (b->blockId == RT && val == 1 && b->row==7)
		int c = 000;
	//����bit
	checkRow_[i1] |= (1 << o1);
	checkCol_[i2] |= (1 << o2);
	checkGong_[i3] |= (1 << o3);
	if (bCheckLine_)
	{
		if (dl == 1)
			checkLine_ |= (1 << (32 - val));
		else if (dl == 2)
			checkLine_ |= (1 << (32 - 9 - val));
		else if (dl == 3)
		{
			checkLine_ |= (1 << (32 - val));
			checkLine_ |= (1 << (32 - 9 - val));
		}
	}
	b->value = val;
	*b->ptr_ = val;
	if(b->ptr2_)
		*b->ptr2_ = val;
	return true;
}

bool CSHUDU::resetBit(tagBox* b)
{
	int v = b->value;
	int o1, o2, o3 = 0;
	int i1, i2, i3 = 0;
	getBit(checkRow_, b->row, v, i1, o1);
	getBit(checkCol_, b->col, v, i2, o2);
	getBit(checkGong_, b->gong, v, i3, o3);
	//����bitλΪ0
	checkRow_[i1] &= ~(1 << o1);
	checkCol_[i2] &= ~(1 << o2);
	checkGong_[i3] &= ~(1 << o3);
	if (bCheckLine_)
	{
		int dl = lineCell(b);
		if (dl == 1)
			checkLine_ &= ~(1 << (32 - v));
		else if (dl == 2)
			checkLine_ &= ~(1 << (32 - 9 - v));
		else if (dl == 3)
		{
			checkLine_ &= ~(1 << (32 - v));
			checkLine_ &= ~(1 << (32 - 9 - v));
		}
	}
	b->value = 0;
	*b->ptr_ = 0;
	if (b->ptr2_)
		*b->ptr2_ = 0;
	return false;
}

int CSHUDU::lineCell(tagBox* a)
{//��Խ���1  �ҶԽ���2
	int res = 0;
	if (a->row == a->col)
		res += 1;
	for (int i = 0, j = 8; i < 9; i++, j--)
		if (a->row == i && a->col == j)
		{
			res += 2;
			break;
		}
	return res;
}

void CSHUDU::getRowCell(tagBox* a, set<tagBox*>& s, bool second)
{
	int row = second ? a->row2 : a->row;
	for (int i = 0; i < 9; i++)
	{//ͬһ�е�
		tagBox* ref = _alBox[row * 9 + i];
		if (ref == a || ref->value)
			continue;
		s.insert(ref);
	}
}

void CSHUDU::getColCell(tagBox* a, set<tagBox*>& s, bool second)
{
	int col = second ? a->col2 : a->col;
	for (int i = col; i < 81; i += 9)
	{//ͬһ�е�
		tagBox* ref = _alBox[i];
		if (ref == a || ref->value)
			continue;
		s.insert(ref);
	}
}

void CSHUDU::getGongCell(tagBox* a, set<tagBox*>& s, bool second)
{
	int gong = second ? a->gong2 : a->gong;
	int col = second ? a->col2 : a->col;
	//ͬһ������
	int index = (gong - 1) / 3 * 27 + col / 3 * 3;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			tagBox* ref = _alBox[index + i * 9 + j];
			if (ref == a || ref->value)
				continue;
			s.insert(ref);
		}
}

void CSHUDU::getAlCell(tagBox* a, set<tagBox*>& s, bool second)
{
	getRowCell(a, s,second);
	getColCell(a, s, second);
	getGongCell(a, s, second);
}


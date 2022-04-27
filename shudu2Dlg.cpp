
// shudu2Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "shudu2.h"
#include "shudu2Dlg.h"
#include "afxdialogex.h"
#include "_ClassHpp/Cshudu.hpp"
#include "_ClassHpp/Cshudu369.hpp"
#include <chrono>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
	
END_MESSAGE_MAP()


// Cshudu2Dlg 对话框



Cshudu2Dlg::Cshudu2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SHUDU2_DIALOG, pParent)
	, isOk_(0)
	, showStr(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cshudu2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
#define DDXMY(ii,jj,arrid,s1,s2,s3)\
	for(int i = ii,j=jj;i<82;)\
	{\
		DDX_Text(pDX,j, sArry[arrid][i]);\
		if(i== s1 || i==s2 || i==s3)\
		{\
			j+=4;\
			i+=4;\
		}\
		else\
		{\
			j++;\
			i++;\
		}\
	}\

	DDXMY(1, IDC_EDIT1, 2, 999, 998, 997);
	DDXMY(1, IDC_EDIT166, 0, 60, 69, 78);
	DDXMY(1, IDC_EDIT85, 1, 54, 63, 72);
	DDXMY(1, IDC_EDIT328, 3, 6, 15, 24);
	DDXMY(4, IDC_EDIT250, 4, 9, 18, 999);
	DDX_Text(pDX, IDC_EDIT82, showStr);
	DDX_Text(pDX, IDC_EDIT83, sCountAc);
	DDX_Text(pDX, IDC_EDIT84, sTime);
}

BEGIN_MESSAGE_MAP(Cshudu2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &Cshudu2Dlg::OnFinish)
	ON_BN_CLICKED(IDC_BUTTON1, &Cshudu2Dlg::OnReset)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// Cshudu2Dlg 消息处理程序

BOOL Cshudu2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cshudu2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cshudu2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cshudu2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int Cshudu2Dlg::check(BYTE(*arry)[9][9])
{
	int count = 0;//计数,至少满足17个数才有唯一解
	for (int z = 0; z < 5; z++)
	{
		int n = 0;
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
			{
				n++;
				int temp = arry[z][i][j];
				if (temp == 0)
					continue;
				else if (temp < 1 || temp>9)//如果不是0-9的数字
				{
					MessageBoxA(m_hWnd, "输入的数字非[1-9]", "错误：", 0);
					return 0;
				}
				sArry[z][n] = "'";
				count++;
			}
	}
	//369格的太高阶了,如果输入的实在太少就不解了太费事
	//if (count < 50)
	//{
	//	MessageBoxA(0, "至少输入50个", "错误：", 0);
	//	return 0;
	//}
	return 1;
}

int Cshudu2Dlg::showResult(BYTE(*arry)[9][9])
{
	CSHUDU369* p = new CSHUDU369(arry);
	clock_t begin, end;
	begin = clock();
	int res = p->work();
	sCountAc.Format(_T("%d"), res);
	if (res < MAXFALSE)
	{
		showStr = "解题成功.";
		if (!res)
			sCountAc = "不存在具有多义性的格子。";
	}
	else 
		showStr = "解题失败.";
	end = clock();
	sTime.Format(_T("%d"), (end-begin)/ ((clock_t)1) );

	for (int z = 0, n = 1; z < 5; z++)
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
			{
				if (arry[z][i][j])
				{
					CString cc;
					cc.Format(_T("%d"), arry[z][i][j]);
					sArry[z][n++] += cc;
				}
				else
					sArry[z][n++] = "";
				if (i == 8 && j == 8)
					n = 1;
			}
	UpdateData(FALSE);
	return 0;
}

void Cshudu2Dlg::OnFinish()
{
	if (isOk_)
		return;
	//获取369网格中输入的数字
	BYTE arry[5][9][9] = {
		{
			{0,1,6, 3,2,0, 0,0,7},
			{0,7,9, 0,4,0, 0,0,6},
			{0,2,8, 6,0,9, 0,0,4},
			 
			{6,0,2, 0,0,0, 5,4,0},
			{7,0,4, 1,0,0, 0,0,0},
			{0,0,1, 5,0,4, 7,6,2},
			 
			{2,0,3, 0,0,6, 0,7,0},
			{0,0,0, 2,1,0, 0,0,0},
			{0,0,0, 7,0,0, 6,0,3},//左上
		},
		{
			{8,0,9, 2,0,4, 3,0,0},
			{4,7,3, 8,0,1, 0,0,5},
			{0,6,0, 0,9,0, 4,7,0},

			{0,3,7, 0,4,2, 0,0,0},
			{9,0,0, 0,5,0, 0,0,4},
			{5,0,4, 0,0,9, 6,1,0},

			{6,4,0, 9,1,7, 8,0,0},
			{0,0,0, 4,0,0, 9,5,0},
			{0,0,0, 0,0,3, 1,4,0},//右上
		},
		{
			{0,7,0, 0,3,2, 6,4,0},
			{0,0,0, 0,6,7, 0,0,0},
			{6,0,3, 1,0,5, 0,0,0},

			{2,9,4, 7,0,0, 0,0,3},
			{0,1,8, 0,9,0, 0,7,6},
			{0,3,6, 5,0,4, 9,0,0},

			{8,4,0, 0,5,9, 2,0,0},
			{3,0,7, 0,2,0, 0,6,0},
			{9,6,0, 3,0,8, 0,5,0},//中间
		},
		{
			{0,0,6, 0,2,7, 8,4,0},
			{8,0,0, 0,0,0, 3,0,7},
			{0,0,7, 3,8,0, 9,6,0},

			{7,0,9, 0,0,0, 0,0,0},
			{1,3,4, 0,0,0, 7,8,6},
			{0,0,0, 0,0,0, 1,3,0},

			{5,0,8, 1,0,2, 0,9,0},
			{0,4,0, 9,5,0, 2,0,0},
			{0,2,3, 7,6,0, 5,0,8},//左下
		},
		{
			{2,0,0, 0,0,0, 6,1,0},
			{0,6,0, 1,0,0, 3,4,0},
			{0,5,0, 0,8,3, 7,2,9},

			{6,0,0, 0,3,0, 1,0,4},
			{7,1,0, 2,0,0, 0,8,0},
			{4,0,3, 0,0,1, 5,0,0},

			{0,0,1, 4,5,2, 0,0,6},
			{9,0,8, 0,0,0, 4,5,0},
			{5,0,6, 8,0,0, 2,3,1},//右下
		},
	};
	/*上面方便调试初始化了369， 如果需要从MFC接收，执行下面函数*/
	getInput(arry);
	int input = check(arry);
	if (input)
	{
		showResult(arry);
		isOk_ = 1;
	}
}

void Cshudu2Dlg::OnReset()
{
	isOk_ = 0;
	for (int j = 0; j < 5; j++)
		for (int i = 0; i < 82; i++)
			sArry[j][i] = "";
	showStr = "";
	sCountAc = "";
	sTime = "";
	UpdateData(FALSE);
}

//获取手动输入
void Cshudu2Dlg::getInput(BYTE (*arry)[9][9])
{//数组开始下标,控件开始id,总共几个格子,特殊的三个值要+=4
#define MYINPUT(ii,jj,arrid,s1,s2,s3)\
	for(int i = ii,j=jj;i<82;)\
	{\
		GetDlgItemTextW(j, sArry[arrid][i]);\
		if(i== s1 || i==s2 || i==s3)\
		{\
			j+=4;\
			i+=4;\
		}\
		else\
		{\
			j++;\
			i++;\
		}\
	}\

	MYINPUT(1, IDC_EDIT1, 2, 999, 998, 997);
	MYINPUT(1, IDC_EDIT166, 0, 60,69,999);
	MYINPUT(1, IDC_EDIT85, 1, 54,63,999);
	MYINPUT(1, IDC_EDIT328, 3, 6,15,  24);
	MYINPUT(4, IDC_EDIT250, 4, 9,18,  27);
	for (int arId = 0,nId = 1; arId < 5; arId++)
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
			{
				arry[arId][i][j] = _ttoi(sArry[arId][nId++]);
				if (i == 8 && j == 8)
					nId = 1;
			}
	int z = 0;
}


HBRUSH Cshudu2Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	//// TODO:  在此更改 DC 的任何特性
	//if (pWnd->GetDlgCtrlID() == IDC_EDIT1)
	//{
	//	pDC->SetTextColor(RGB(255, 0, 0));//设置字体颜色
	//	pDC->SetBkColor(RGB(0, 255, 0));//设置背景颜色
	//}
	//// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

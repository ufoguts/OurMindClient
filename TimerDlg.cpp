// TimerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OurMindClient.h"
#include "TimerDlg.h"
#include "afxdialogex.h"

#include "EditMindDlg.h"


// TimerDlg 对话框

IMPLEMENT_DYNAMIC(TimerDlg, CDialogEx)

TimerDlg::TimerDlg(MindNode &TheMind, const bool EditLimit, CWnd *pFatherWnd, 
	CCriticalSection &csNetUpdata, CCriticalSection &csDataUpdata)
:
	CDialogEx(TimerDlg::IDD, 0),
	TheMind(TheMind), EditLimit(EditLimit), pFatherWnd(pFatherWnd), 
	csNetUpdata(csNetUpdata), csDataUpdata(csDataUpdata)
{
	
}

TimerDlg::~TimerDlg()
{
}

void TimerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TimerDlg, CDialogEx)
	ON_WM_NCPAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// TimerDlg 消息处理程序


//初始化函数
BOOL TimerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//隐藏对话框
	MoveWindow(0, 0, 0, 0);

	//初始化状态
	UpdataFlag = false;

	//设置定时器
	SetTimer(1, TIMER_VALUE, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


//包含非客户区重绘图函数
void TimerDlg::OnNcPaint()
{
	// TODO: 在此处添加消息处理程序代码

	//持续隐藏对话框
	ShowWindow(SW_HIDE);
	// 不为绘图消息调用 CDialogEx::OnNcPaint()
}


//计时触发函数
void TimerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	PacketType type;
	
	//网络更新互斥开始
	csNetUpdata.Lock();

	//据情况发送请求
	if(EditLimit)
	{
		type = DownMindConnect(OperateList);
		if(type==fail)
			::PostMessage(pFatherWnd->m_hWnd, WM_INTERNET_ERROR, 2, 0);
	}
	else
	{
		//数据更新互斥开始
		csDataUpdata.Lock();

		if(UpdataFlag)
		{
			MindNode CopyMind;
			CopyMind.Copy(TheMind);
			UpdataFlag = false;

			//数据更新互斥结束
			csDataUpdata.Unlock();

			type = UpMindConnect(TheMind, OperateList);
		}
		else
		{
			//数据更新互斥结束
			csDataUpdata.Unlock();

			type = KeepMindConnect(OperateList);
		}
	}

	if(type==error || type==nopermission)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();

		//错误处理，发消息给主线程
		if(type==error)
			::SendMessage(pFatherWnd->m_hWnd, WM_INTERNET_ERROR, 0, 0);
		else if(type==nopermission)
			::SendMessage(pFatherWnd->m_hWnd, WM_INTERNET_ERROR, 1, 0);
	}
	else
	{
		//进行数据更新
		if(OperateList.num>0)
		{
			//数据更新互斥开始
			csDataUpdata.Lock();

			bool MindChange;
			MindChange = OperateData(TheMind, OperateList);
			//发消息给主线程
			::PostMessage(pFatherWnd->m_hWnd, WM_MIND_UPDATA, MindChange, 0);

			//数据更新互斥结束
			csDataUpdata.Unlock();
		}

		//网络更新互斥结束
		csNetUpdata.Unlock();
	}

	//重设时钟
	SetTimer(1, TIMER_VALUE, NULL);
}

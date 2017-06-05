#pragma once

// TimerDlg 对话框


#define TIMER_VALUE 500//时钟触发时间

class TimerDlg : public CDialogEx
{
	//友元
	friend class EditMindDlg;//父类


	DECLARE_DYNAMIC(TimerDlg)

public:
	TimerDlg(MindNode &TheMind, const bool EditLimit, CWnd *pFatherWnd, 
		CCriticalSection &csNetUpdata, CCriticalSection &csDataUpdata);//标准构造函数
	virtual ~TimerDlg();

// 对话框数据
	enum { IDD = IDD_TIMERDLG };

	//变量
protected:
	MindNode &TheMind;//当前思维导图
	const bool EditLimit;//编辑限制
	bool UpdataFlag;//更新标志
	ListTemplet<class OperateNode> OperateList;//接收操作链表
	const CWnd *pFatherWnd;//逻辑父窗口指针
	//线程同步
	CCriticalSection &csNetUpdata;//网络更新临界区
	CCriticalSection &csDataUpdata;//数据更新临界区

	//方法

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnNcPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

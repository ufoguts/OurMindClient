
// OurMindClientDlg.h : 头文件
//

#pragma once


// COurMindClientDlg 对话框
class COurMindClientDlg : public CDialogEx
{
// 构造
public:
	COurMindClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_OURMINDCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	//变量
protected:
	//CString UserId;
	CButton *pButtonDel;//删除按钮
	CEdit *pEditMind;//显示思维导图文本框
	CListBox *pListOwn;//拥有思维导图列表框
	CListBox *pListJoin;//加入思维导图列表框
	ListTemplet<MindNode> MyMindList;//拥有思维导图
	ListTemplet<MindNode> OurMindList;//加入思维导图

	//函数
protected:
	void RefreshList();//刷新列表框显示
	void RefreshEdit(MindNode *pTheMind);//刷新编辑框显示

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonChange();
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnSelchangeListOwn();
	afx_msg void OnDblclkListOwn();
	afx_msg void OnSelchangeListJoin();
	afx_msg void OnDblclkListJoin();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedCancel();
};


#pragma once


// LandDlg 对话框

class LandDlg : public CDialogEx
{
	//友元
	friend class COurMindClientDlg;//父类


	DECLARE_DYNAMIC(LandDlg)

public:
	LandDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~LandDlg();

// 对话框数据
	enum { IDD = IDD_LANDDLG };

	//变量
protected:
	CString UserId;
	CString Password;
	CString Password2;

	//方法
public:
	static bool StringRule(CString &string);//判断字符串合乎规则

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedLogin();
	afx_msg void OnBnClickedRegist();
};

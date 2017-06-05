#pragma once


// GetIpDlg 对话框

class GetIpDlg : public CDialogEx
{
	//友元
	friend class COurMindClientDlg;//父类


	DECLARE_DYNAMIC(GetIpDlg)

public:
	GetIpDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GetIpDlg();

// 对话框数据
	enum { IDD = IDD_GETIPDLG };

	//传递变量
protected:
	CString Ip;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

#pragma once


// ChangeDlg 对话框

class ChangeDlg : public CDialogEx
{
	//友元
	friend class COurMindClientDlg;//父类


	DECLARE_DYNAMIC(ChangeDlg)

public:
	ChangeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ChangeDlg();

// 对话框数据
	enum { IDD = IDD_CHANGEDLG };

	//变量
protected:
	CString Password;
	CString Password2;
	CString Password3;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

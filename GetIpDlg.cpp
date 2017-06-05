// GetIpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OurMindClient.h"
#include "GetIpDlg.h"
#include "afxdialogex.h"


// GetIpDlg 对话框

IMPLEMENT_DYNAMIC(GetIpDlg, CDialogEx)

GetIpDlg::GetIpDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(GetIpDlg::IDD, pParent)
{

}

GetIpDlg::~GetIpDlg()
{
}

void GetIpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GetIpDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &GetIpDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// GetIpDlg 消息处理程序


void GetIpDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	//获取输入字符串
	GetDlgItemText(IDC_EDIT_IP, Ip);

	//测试ip连接
	if(TestServerIp(Ip))
		CDialogEx::OnOK();
	else
	{
		MessageBox(L"IP错误或服务器未开启", L"NO", MB_OK);
	}
}

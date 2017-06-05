
// LandDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OurMindClient.h"
#include "LandDlg.h"
#include "afxdialogex.h"


// LandDlg 对话框

IMPLEMENT_DYNAMIC(LandDlg, CDialogEx)

LandDlg::LandDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(LandDlg::IDD, pParent)
{

}

LandDlg::~LandDlg()
{
}

void LandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LandDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &LandDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_LOGIN, &LandDlg::OnBnClickedLogin)
	ON_BN_CLICKED(ID_REGIST, &LandDlg::OnBnClickedRegist)
END_MESSAGE_MAP()


// LandDlg 消息处理程序


void LandDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void LandDlg::OnBnClickedLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//获取输入字符串
	GetDlgItemText(IDC_EDIT_ID, UserId);
	GetDlgItemText(IDC_EDIT_PW, Password);

	if(StringRule(UserId)==0)
	{
		MessageBox(L"用户名不合法", L"NO", MB_OK);
		return;
	}
	if(StringRule(Password)==0)
	{
		MessageBox(L"密码不合法", L"NO", MB_OK);
		return;
	}

	//连接服务器检验
	PacketType type = LoginConnect(UserId, Password);
	if(type==error)
	{
		MessageBox(L"服务器错误", L"NO", MB_OK);
		return;
	}
	else if(type==fail)
	{
		MessageBox(L"用户名不存在或密码错误", L"NO", MB_OK);
		return;
	}
	else if(type!=success)
		assert(0);

	//返回
	CDialogEx::OnOK();
}


void LandDlg::OnBnClickedRegist()
{
	// TODO: 在此添加控件通知处理程序代码

	//获取输入字符串
	GetDlgItemText(IDC_EDIT_ID, UserId);
	GetDlgItemText(IDC_EDIT_PW, Password);
	GetDlgItemText(IDC_EDIT_PW2, Password2);
	
	if(StringRule(UserId)==0)
	{
		MessageBox(L"用户名不合法", L"NO", MB_OK);
		return;
	}
	if(StringRule(Password)==0)
	{
		MessageBox(L"密码不合法", L"NO", MB_OK);
		return;
	}
	if(Password!=Password2)
	{
		MessageBox(L"密码不一致", L"NO", MB_OK);
		return;
	}
	
	//连接服务器检验
	PacketType type = RegistConnect(UserId, Password);
	if(type==error)
	{
		MessageBox(L"服务器错误", L"NO", MB_OK);
		return;
	}
	else if(type==fail)
	{
		MessageBox(L"用户名已存在", L"NO", MB_OK);
		return;
	}
	else if(type!=success)
		assert(0);

	//返回
	CDialogEx::OnOK();
}


//判断字符串合乎规则
bool LandDlg::StringRule(CString &string)
{
	int len = string.GetLength();
	int i;
	if(len<3 || len>20)
		return false;
	for(i=0; i<len; i++)
	{
		if(!(string[i]>='0' && string[i]<='9' || string[i]>='a' && string[i]<='z' || string[i]>='A' && string[i]<='Z'))
			return false;
	}
	return true;
}
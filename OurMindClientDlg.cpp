
// OurMindClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OurMindClient.h"
#include "OurMindClientDlg.h"
#include "afxdialogex.h"

#include "LandDlg.h"
#include "GetIpDlg.h"
#include "ChangeDlg.h"
#include "EditMindDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COurMindClientDlg 对话框


COurMindClientDlg::COurMindClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COurMindClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COurMindClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COurMindClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &COurMindClientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, &COurMindClientDlg::OnBnClickedButtonChange)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &COurMindClientDlg::OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_NEW, &COurMindClientDlg::OnBnClickedButtonNew)
	ON_LBN_SELCHANGE(IDC_LIST_OWN, &COurMindClientDlg::OnSelchangeListOwn)
	ON_LBN_DBLCLK(IDC_LIST_OWN, &COurMindClientDlg::OnDblclkListOwn)
	ON_LBN_SELCHANGE(IDC_LIST_JOIN, &COurMindClientDlg::OnSelchangeListJoin)
	ON_LBN_DBLCLK(IDC_LIST_JOIN, &COurMindClientDlg::OnDblclkListJoin)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &COurMindClientDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDCANCEL, &COurMindClientDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// COurMindClientDlg 消息处理程序

BOOL COurMindClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//初始化网络环境
	ConnetcInit();

	//创建窗口，获取服务器ip地址
	GetIpDlg ipdlg(this);
	if(ipdlg.DoModal()==IDOK)
	{
		g_ServerIp = ipdlg.Ip;
		g_ServerSocket.sin_family = AF_INET;
		g_ServerSocket.sin_addr.s_addr = inet_addr(CT2A(g_ServerIp));// "127.0.0.1" );
		g_ServerSocket.sin_port = htons(atoi(SERVER_PORT));
	}
	else
	{
		EndDialog(IDOK);
		return TRUE;
	}

	//创建窗口，登录或注册
	LandDlg landdlg(this);
	if(landdlg.DoModal()==IDOK)
		;
	else
	{
		EndDialog(IDOK);
		return TRUE;
	}

	//初始化主窗口
	//记录显示UserId
	g_UserId = landdlg.UserId;
	SetDlgItemText(IDC_STATIC_USER, g_UserId);
	//初始化控件指针
	pButtonDel = (CButton *)GetDlgItem(IDC_BUTTON_DEL);
	pButtonDel->EnableWindow(FALSE);
	pEditMind = (CEdit *)GetDlgItem(IDC_EDIT_MINDMESSAGE);
	pListOwn = (CListBox *)GetDlgItem(IDC_LIST_OWN);
	pListJoin = (CListBox *)GetDlgItem(IDC_LIST_JOIN);

	if(GetListConnect(MyMindList, OurMindList)!=success)
		MessageBox(L"与服务器断开连接", L"NO", MB_OK);
	RefreshList();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COurMindClientDlg::OnPaint()
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
HCURSOR COurMindClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void COurMindClientDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


//修改信息
void COurMindClientDlg::OnBnClickedButtonChange()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//创建窗口修改信息
	ChangeDlg changedlg(this);
	changedlg.DoModal();
}


//刷新按钮
void COurMindClientDlg::OnBnClickedButtonRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//删除按钮失效
	pButtonDel->EnableWindow(FALSE);
	//请求获取思维导图列表
	if(GetListConnect(MyMindList, OurMindList)!=success)
		MessageBox(L"与服务器断开连接", L"NO", MB_OK);
	RefreshList();
	//清空编辑框显示显示
	RefreshEdit(0);
}


//新建思维导图按钮
void COurMindClientDlg::OnBnClickedButtonNew()
{
	// TODO: 在此添加控件通知处理程序代码

	//建立编辑窗口
	EditMindDlg editdlg(false, this);
	//获取新建思维导图信息
	if(NewMindConnect(editdlg.TheMind)!=success)
	{
		MessageBox(L"与服务器断开连接", L"NO", MB_OK);
		return;
	}
	//打开编辑窗口
	editdlg.DoModal();
	//通知结束编辑
	//EditEndConnect();
	//获取思维导图列表
	if(GetListConnect(MyMindList, OurMindList)!=success)
		MessageBox(L"与服务器断开连接", L"NO", MB_OK);
	RefreshList();
	RefreshEdit(0);
}


//删除思维导图按钮
void COurMindClientDlg::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	
	int Index = pListOwn->GetCurSel();
	//获取思维导图列表
	if(GetListConnect(MyMindList, OurMindList)!=success)
		MessageBox(L"与服务器断开连接", L"NO", MB_OK);
	RefreshList();
	pListOwn->SetCurSel(Index);
	//找到此项并显示
	int i;
	MyMindList.pnode = MyMindList.qnode->pt;
	for(i=0; i<Index; i++)
		LIST_GO(MyMindList.pnode, pt);
	RefreshEdit(MyMindList.pnode);
	//弹窗提示
	CString string;
	string.Format(L"当前有%d人正在编辑，确定要删除此思维导图吗？", MyMindList.pnode->EditNum);
	if(MessageBox(string, L"ASK", MB_YESNO)==IDYES)
	{
		//请求删除
		if(DelMindConnect(MyMindList.pnode->MindName)!=success)
			MessageBox(L"与服务器断开连接", L"NO", MB_OK);
		//刷新，删除按钮无效
		pButtonDel->EnableWindow(FALSE);
		if(GetListConnect(MyMindList, OurMindList)!=success)
			MessageBox(L"与服务器断开连接", L"NO", MB_OK);
		RefreshList();
		//清空编辑框显示显示
		RefreshEdit(0);
	}
	else
	{
		//刷新并显示
		if(GetListConnect(MyMindList, OurMindList)!=success)
			MessageBox(L"与服务器断开连接", L"NO", MB_OK);
		RefreshList();
		pListOwn->SetCurSel(Index);
	}
}


//选择拥有列表项
void COurMindClientDlg::OnSelchangeListOwn()
{
	// TODO: 在此添加控件通知处理程序代码

	//确实的选择一项
	int Index = pListOwn->GetCurSel();
	if(Index!=-1)
	{
		//删除按钮有效
		pButtonDel->EnableWindow(TRUE);
		//另一个列表框取消选项
		pListJoin->SetCurSel(-1);
		//显示选项详细信息
		int i;
		MyMindList.pnode = MyMindList.qnode->pt;
		for(i=0; i<Index; i++)
			LIST_GO(MyMindList.pnode, pt);
		RefreshEdit(MyMindList.pnode);
	}
}


//选择参与列表项
void COurMindClientDlg::OnSelchangeListJoin()
{
	// TODO: 在此添加控件通知处理程序代码

	//确实的选择一项
	int Index = pListJoin->GetCurSel();
	if(Index!=-1)
	{
		//删除按钮失效
		pButtonDel->EnableWindow(FALSE);
		//另一个列表框取消选项
		pListOwn->SetCurSel(-1);
		//显示选项详细信息
		int i;
		OurMindList.pnode = OurMindList.qnode->pt;
		for(i=0; i<Index; i++)
			LIST_GO(OurMindList.pnode, pt);
		RefreshEdit(OurMindList.pnode);
	}
}


//双击拥有列表项
void COurMindClientDlg::OnDblclkListOwn()
{
	// TODO: 在此添加控件通知处理程序代码

	//未选择就返回
	int Index = pListOwn->GetCurSel();
	if(Index==-1)
		return;
	//找到思维导图
	int i;
	MyMindList.pnode = MyMindList.qnode->pt;
	for(i=0; i<Index; i++)
		LIST_GO(MyMindList.pnode, pt);
	//建立编辑窗口
	EditMindDlg editdlg(false, this);
	//获取新建思维导图信息
	PacketType type = SelMindConnect(editdlg.TheMind, *(MyMindList.pnode));
	if(type==fail)
	{
		MessageBox(L"无权访问或该思维导图已被删除", L"NO", MB_OK);
	}
	else if(type!=success)
	{
		MessageBox(L"与服务器断开连接", L"NO", MB_OK);
		return;
	}
	else
	{
		//打开编辑窗口
		editdlg.DoModal();
		//通知结束编辑
		//EditEndConnect();
	}
	//获取思维导图列表
	if(GetListConnect(MyMindList, OurMindList)!=success)
		MessageBox(L"与服务器断开连接", L"NO", MB_OK);
	RefreshList();
	RefreshEdit(0);
}


//双击参与列表项
void COurMindClientDlg::OnDblclkListJoin()
{
	// TODO: 在此添加控件通知处理程序代码

	//未选择就返回
	int Index = pListJoin->GetCurSel();
	if(Index==-1)
		return;
	//找到思维导图
	int i;
	OurMindList.pnode = OurMindList.qnode->pt;
	for(i=0; i<Index; i++)
		LIST_GO(OurMindList.pnode, pt);
	//建立编辑窗口
	EditMindDlg editdlg(true, this);
	//获取新建思维导图信息
	PacketType type = SelMindConnect(editdlg.TheMind, *(OurMindList.pnode));
	if(type==fail)
	{
		MessageBox(L"无权访问或该思维导图已被删除", L"NO", MB_OK);
	}
	else if(type!=success)
	{
		MessageBox(L"与服务器断开连接", L"NO", MB_OK);
		return;
	}
	else
	{
		//打开编辑窗口
		editdlg.DoModal();
		//通知结束编辑
		//EditEndConnect();
	}
	//获取思维导图列表
	if(GetListConnect(MyMindList, OurMindList)!=success)
		MessageBox(L"与服务器断开连接", L"NO", MB_OK);
	RefreshList();
	RefreshEdit(0);
}


//刷新列表框显示
void COurMindClientDlg::RefreshList()
{
	//显示列表
	pListOwn->ResetContent();
	for(MyMindList.pnode=MyMindList.qnode->pt; MyMindList.pnode; LIST_GO(MyMindList.pnode, pt))
	{
		pListOwn->AddString(MyMindList.pnode->MindName);
	}
	pListJoin->ResetContent();
	int Tab[] = {80};
	pListJoin->SetTabStops(1, Tab);
	for(OurMindList.pnode=OurMindList.qnode->pt; OurMindList.pnode; LIST_GO(OurMindList.pnode, pt))
	{
		pListJoin->AddString(OurMindList.pnode->UserId+L"\t"+OurMindList.pnode->MindName);
	}
}


//刷新编辑框显示
void COurMindClientDlg::RefreshEdit(MindNode *pTheMind)
{
	CString Text;
	Text = L"";
	if(pTheMind)
	{
		int Tab[] = {100};
		pEditMind->SetTabStops(1, Tab);
		CString string;
		Text += L"拥有者："+pTheMind->UserId;
		Text += L"\t思维导图名称："+pTheMind->MindName;
		string.Format(L"\r\n思维导图大小：%d*%d", pTheMind->Length, pTheMind->Width);
		Text += string;
		if(pTheMind->ReadOnly)
			Text += L"\t共享用户只读";
		else
			Text += L"\t共享用户可编辑";
		string.Format(L"\t共享用户人数：%d", pTheMind->ShareNum);
		Text += string;
		Text += L"\r\n编辑时间："+pTheMind->EditTime;
		string.Format(L"\t当前编辑人数：%d", pTheMind->EditNum);
		Text += string;
	}
	pEditMind->SetWindowTextW(Text);
}


void COurMindClientDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

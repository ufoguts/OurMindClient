// EditMindDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OurMindClient.h"
#include "EditMindDlg.h"
#include "afxdialogex.h"

#include "TimerDlg.h"
#include "LandDlg.h"


static int g_IntBuf[10000];


//初始化控件ID列表
const int EditMindDlg::MindFreeItem[] = 
{
	IDC_STATIC_MINDNAME,
	IDC_STATIC_EDITNUM,
	IDC_STATIC_SHARENUM,
	IDC_STATIC_TIP,
};

const int EditMindDlg::MindLimitItem[] = 
{
	IDC_EDIT_MINDNAME,
	IDC_CHECK_READONLY,
	IDC_BUTTON_ADDROOT,
	IDC_BUTTON_SETLOCK,
	IDC_BUTTON_SETUNLOCK,
	IDC_CHECK_SETWITH,
	IDC_LIST_SHARELIST,
	IDC_EDIT_ADDSHARE,
	IDC_BUTTON_DELSHARE,
	IDC_BUTTON_ADDSHARE,
	IDC_STATIC_TIP2,
};

const int EditMindDlg::BlockFreeItem[] = 
{
	IDC_STATIC_BLOCKTEXT,
	IDC_EDIT_BLOCKTEXT,
	IDC_STATIC_SHAP,
	IDC_COMBO_SHAPE,
	IDC_STATIC_COLOR,
	IDC_BUTTON_SETWHITE,
	IDC_STATIC_RED,
	IDC_STATIC_GREEN,
	IDC_STATIC_BULE,
	IDC_SLIDER_RED,
	IDC_SLIDER_GREEN,
	IDC_SLIDER_BLUE,
	IDC_EDIT_RED,
	IDC_EDIT_GREEN,
	IDC_EDIT_BLUE,
	IDC_STATIC_LOCK,
	IDC_BUTTON_DELWITH,
	IDC_BUTTON_DELCOMBINE,
	IDC_BUTTON_ADDCHILD,
	IDC_BUTTON_ADDPARENT,
	IDC_BUTTON_LINETO,
	IDC_STATIC_TIP3,
	IDC_STATIC_TIP4,
	IDC_STATIC_LINEDIR,
	IDC_COMBO_LINEDIR,
};

const int EditMindDlg::BlockLimitItem[] = 
{
	IDC_BUTTON_SEPARATEROOT,
	IDC_BUTTON_DELSEPARETE,
};

const int EditMindDlg::AlwaysHideItem[] = 
{
	IDC_STATIC_SELLOCKBLOCK,
	IDC_STATIC_SELLINEBLOCK,
};

//初始化其他静态常量
const double EditMindDlg::ZoomSize[] = 
{
	0.350, 0.455, 0.592, 0.769, 1, 1.2, 1.44, 1.782, 2.074, 2.488, 2.986, 3.583, 4.300, 5.160, 6.192, 7.430, 8.916
};
const int EditMindDlg::MaxZoomLevel = sizeof(ZoomSize)/sizeof(double);
const int EditMindDlg::DefaultZoom = 6;


// EditMindDlg 对话框

IMPLEMENT_DYNAMIC(EditMindDlg, CDialogEx)

EditMindDlg::EditMindDlg(const bool EditLimit, CWnd* pParent/*=NULL*/):
	CDialogEx(EditMindDlg::IDD, pParent),
	EditLimit(EditLimit)
{

}

EditMindDlg::~EditMindDlg()
{
}

void EditMindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(EditMindDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &EditMindDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &EditMindDlg::OnBnClickedCancel)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_MOUSEWHEEL()
ON_EN_CHANGE(IDC_EDIT_MINDNAME, &EditMindDlg::OnChangeEditMindname)
ON_EN_KILLFOCUS(IDC_EDIT_MINDNAME, &EditMindDlg::OnKillfocusEditMindname)
ON_MESSAGE(WM_MIND_UPDATA, &EditMindDlg::OnMindUpdata)
ON_MESSAGE(WM_INTERNET_ERROR, &EditMindDlg::OnInternetError)
ON_BN_CLICKED(IDC_BUTTON_ADDSHARE, &EditMindDlg::OnBnClickedButtonAddshare)
ON_BN_CLICKED(IDC_BUTTON_DELSHARE, &EditMindDlg::OnBnClickedButtonDelshare)
ON_BN_CLICKED(IDC_CHECK_READONLY, &EditMindDlg::OnClickedCheckReadonly)
ON_BN_CLICKED(IDC_CHECK_SETWITH, &EditMindDlg::OnClickedCheckSetwith)
ON_BN_CLICKED(IDC_BUTTON_ADDROOT, &EditMindDlg::OnBnClickedButtonAddroot)
ON_WM_RBUTTONDBLCLK()
ON_EN_CHANGE(IDC_EDIT_BLOCKTEXT, &EditMindDlg::OnChangeEditBlocktext)
ON_EN_KILLFOCUS(IDC_EDIT_BLOCKTEXT, &EditMindDlg::OnKillfocusEditBlocktext)
ON_BN_CLICKED(IDC_BUTTON_DELWITH, &EditMindDlg::OnBnClickedButtonDelwith)
ON_BN_CLICKED(IDC_BUTTON_DELCOMBINE, &EditMindDlg::OnBnClickedButtonDelcombine)
ON_BN_CLICKED(IDC_BUTTON_ADDCHILD, &EditMindDlg::OnBnClickedButtonAddchild)
ON_BN_CLICKED(IDC_BUTTON_ADDPARENT, &EditMindDlg::OnBnClickedButtonAddparent)
ON_BN_CLICKED(IDC_BUTTON_LINETO, &EditMindDlg::OnBnClickedButtonLineto)
ON_BN_CLICKED(IDC_BUTTON_SEPARATEROOT, &EditMindDlg::OnBnClickedButtonSeparateroot)
ON_BN_CLICKED(IDC_BUTTON_DELSEPARETE, &EditMindDlg::OnBnClickedButtonDelseparete)
ON_EN_CHANGE(IDC_EDIT_RED, &EditMindDlg::OnChangeEditRed)
ON_EN_CHANGE(IDC_EDIT_GREEN, &EditMindDlg::OnChangeEditGreen)
ON_EN_CHANGE(IDC_EDIT_BLUE, &EditMindDlg::OnChangeEditBlue)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_RED, &EditMindDlg::OnCustomdrawSliderRed)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_GREEN, &EditMindDlg::OnCustomdrawSliderGreen)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_BLUE, &EditMindDlg::OnCustomdrawSliderBlue)
ON_BN_CLICKED(IDC_BUTTON_SETWHITE, &EditMindDlg::OnBnClickedButtonSetwhite)
ON_CBN_SELCHANGE(IDC_COMBO_SHAPE, &EditMindDlg::OnSelchangeComboShap)
ON_CBN_SELCHANGE(IDC_COMBO_LINEDIR, &EditMindDlg::OnSelchangeComboLinedir)
ON_BN_CLICKED(IDC_BUTTON_SETLOCK, &EditMindDlg::OnBnClickedButtonSetlock)
ON_BN_CLICKED(IDC_BUTTON_SETUNLOCK, &EditMindDlg::OnBnClickedButtonSetunlock)
END_MESSAGE_MAP()


//窗口初始化函数
BOOL EditMindDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//初始化控件指针
	int i=0;
	for(i=0; i<sizeof(MindFreeItem)/sizeof(int); i++)
		pMindFreeItem[i] = GetDlgItem(MindFreeItem[i]);
	for(i=0; i<sizeof(MindLimitItem)/sizeof(int); i++)
		pMindLimitItem[i] = GetDlgItem(MindLimitItem[i]);
	for(i=0; i<sizeof(BlockFreeItem)/sizeof(int); i++)
		pBlockFreeItem[i] = GetDlgItem(BlockFreeItem[i]);
	for(i=0; i<sizeof(BlockLimitItem)/sizeof(int); i++)
		pBlockLimitItem[i] = GetDlgItem(BlockLimitItem[i]);
	for(i=0; i<sizeof(AlwaysHideItem)/sizeof(int); i++)
		pAlwaysHideItem[i] = GetDlgItem(AlwaysHideItem[i]);

	//初始化控件显示
	for(i=0; i<sizeof(BlockFreeItem)/sizeof(int); i++)
		pBlockFreeItem[i]->ShowWindow(SW_HIDE);
	for(i=0; i<sizeof(BlockLimitItem)/sizeof(int); i++)
		pBlockLimitItem[i]->ShowWindow(SW_HIDE);
	for(i=0; i<sizeof(AlwaysHideItem)/sizeof(int); i++)
		pAlwaysHideItem[i]->ShowWindow(SW_HIDE);

	//初始化控件可用性
	if(EditLimit)
	{
		for(i=0; i<sizeof(MindLimitItem)/sizeof(int); i++)
			pMindLimitItem[i]->EnableWindow(FALSE);
		for(i=0; i<sizeof(BlockLimitItem)/sizeof(int); i++)
			pBlockLimitItem[i]->EnableWindow(FALSE);
	}

	//初始化控件指针及辅助变量
	CString string;
	//导图名字编辑框
	pEditMindName = (CEdit *)GetDlgItem(IDC_EDIT_MINDNAME);
	pEditMindName->SetWindowTextW(TheMind.MindName);
	LastMindName = TheMind.MindName;
	//共享用户列表框
	pListShareList = (CListBox *)GetDlgItem(IDC_LIST_SHARELIST);
	RefreshShareList(false);
	//添加共享用户编辑框
	pEditAddShare = (CEdit *)GetDlgItem(IDC_EDIT_ADDSHARE);
	//编辑人数静态框
	pStaticEditNum = (CStatic *)GetDlgItem(IDC_STATIC_EDITNUM);
	string.Format(L"编辑人数：%d", TheMind.EditNum);
	pStaticEditNum->SetWindowTextW(string);
	//共享人数静态框
	pStaticShareNum = (CStatic *)GetDlgItem(IDC_STATIC_SHARENUM);
	string.Format(L"共享人数：%d", TheMind.ShareNum);
	pStaticShareNum->SetWindowTextW(string);
	//只读复选按钮
	pCheckReadOnly = (CButton *)GetDlgItem(IDC_CHECK_READONLY);
	pCheckReadOnly->SetCheck(TheMind.ReadOnly? BST_CHECKED:BST_UNCHECKED);
	//设置连带复选按钮
	pCheckSetWith = (CButton *)GetDlgItem(IDC_CHECK_SETWITH);
	SetWith = false;
	//设置编辑节点状态
	EditBlockStatus = false;
	//分块文字编辑框
	pEditBlockText = (CEdit *)GetDlgItem(IDC_EDIT_BLOCKTEXT);
	//分块锁定静态框
	pStaticLock = (CStatic *)GetDlgItem(IDC_STATIC_LOCK);
	//设置颜色编辑框
	ColorAcceptFlag = false;
	pEditRed = (CEdit *)GetDlgItem(IDC_EDIT_RED);
	pEditGreen = (CEdit *)GetDlgItem(IDC_EDIT_GREEN);
	pEditBlue = (CEdit *)GetDlgItem(IDC_EDIT_BLUE);
	//设置颜色滑动框
	pSliderRed = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_RED);
	pSliderRed->SetRange(0, 255);
	pSliderGreen = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_GREEN);
	pSliderGreen->SetRange(0, 255);
	pSliderBlue = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_BLUE);
	pSliderBlue->SetRange(0, 255);
	//选择形状组合框
	pComboShape = (CComboBox *)GetDlgItem(IDC_COMBO_SHAPE);
	pComboShape->AddString(CString(L"矩形"));
	pComboShape->AddString(CString(L"圆角矩形"));
	pComboShape->AddString(CString(L"椭圆形"));
	pComboShape->AddString(CString(L"梯形"));
	pComboShape->AddString(CString(L"菱形"));
	pComboShape->AddString(CString(L"平行四边形"));
	pComboShape->AddString(CString(L"八边形"));
	//提示选择静态框
	pStaticSelLineBlock = (CStatic *)GetDlgItem(IDC_STATIC_SELLINEBLOCK);
	pStaticSelLockBlock = (CStatic *)GetDlgItem(IDC_STATIC_SELLOCKBLOCK);
	//选择方向组合框
	pComboLineDir = (CComboBox *)GetDlgItem(IDC_COMBO_LINEDIR);
	pComboLineDir->AddString(CString(L"向上"));
	pComboLineDir->AddString(CString(L"向下"));
	pComboLineDir->AddString(CString(L"向左"));
	pComboLineDir->AddString(CString(L"向右"));


	//初始化绘图变量
	CornerPoint.SetPoint(0, 0);
	ZoomLevel = DefaultZoom;
	GetClientRect(&rectEditArea);
	rectEditArea.right = 870;
	rectBackArea = rectEditArea;
	rectBackArea.right ++;
	rectBackArea.bottom ++;

	//初始化状态变量
	LBottonStatus = false;
	RBottonStatus = false;
	EditBlockStatus = false;
	ChooseLineBlock = false;
	ChooseLockBlock = false;
	ChooseUnlockBlock = false;

	//打开同步用工作者线程
	pThread = AfxBeginThread(ThreadFunc, this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void EditMindDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码

	PaintMind(&dc);

	// 不为绘图消息调用 CDialogEx::OnPaint()
}


//同步工作者线程回调函数
UINT EditMindDlg::ThreadFunc(LPVOID pParam)
{
	EditMindDlg *pthis = (EditMindDlg *)pParam;
	pthis->pTimerDlg = new TimerDlg(pthis->TheMind, pthis->EditLimit, pthis, 
		pthis->csNetUpdata, pthis->csDataUpdata);
	TimerDlg *pDlg = pthis->pTimerDlg;
	pthis->pTimerDlg->DoModal();
	delete pDlg;
	return 0;
}


// EditMindDlg 消息处理程序


//鼠标左键按下事件
void EditMindDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//取消任何控件的输入焦点
	SetFocus();

	//若正在编辑状态
	if(EditBlockStatus && ChooseLineBlock==false)
	{
		if(!rectEditArea.PtInRect(point))
			return;

		//数据更新互斥开始
		csDataUpdata.Lock();

		//判断点击在哪个分块区域内
		BlockNode *pBlock;
		CPoint point2 = RealToImage(point);
		pBlock = ChoiseBlock(TheMind.BlockTree.qnode->pch, point2);
		//若在编辑分块区域内
		if(pBlock==pEditBlock)
		{
			LBottonStatus = true;
			LBottonPoint = CPoint(pBlock->PosX, pBlock->PosY)-point2;
			SetCapture();
		}

		//数据更新互斥结束
		csDataUpdata.Unlock();
	}
	//若选择连接到分块
	else if(ChooseLineBlock)
	{
		//网络更新互斥开始
		csNetUpdata.Lock();

		//判断点击在哪个分块区域内
		BlockNode *pBlock;
		CPoint point2 = RealToImage(point);
		pBlock = ChoiseBlock(TheMind.BlockTree.qnode->pch, point2);

		//若在编辑分块区域内
		if(pBlock==pEditBlock)
		{
			//网络更新互斥结束
			csNetUpdata.Unlock();
			MessageBox(L"分块不能连接自己", L"NO", MB_OK);
			return;
		}
		//若点击空白处
		else if(pBlock==0)
		{
			//网络更新互斥结束
			csNetUpdata.Unlock();
			ChooseLineBlock = false;
			pStaticSelLineBlock->ShowWindow(SW_HIDE);
			return;
		}

		TheMind.BlockTree.pnode = pBlock;
		int deep = TheMind.BlockTree.GetLocal(g_IntBuf);
		//申请并继续编辑
		PacketType Type = BlockLineToConnect(*pEditBlock, OperateList, deep, g_IntBuf);

		if(Type==fail)
		{
			//失去权限则转换状态并绘图
			TurnMindStatus();
			CClientDC dc(this);
			PaintMind(&dc);

			//网络更新互斥结束
			csNetUpdata.Unlock();

			MessageBox(L"失去编辑权限，修改失败", L"NO", MB_OK);
			return;
		}
		else if(Type==clash)
		{
			//网络更新互斥结束
			csNetUpdata.Unlock();
			MessageBox(L"连向分块不可访问或连向自身子分块", L"NO", MB_OK);
			return;
		}
		else if(Type==nopermission)
		{
			//网络更新互斥结束
			csNetUpdata.Unlock();
			MessageBox(L"思维导图被删除或已被移出共享用户组", L"NO", MB_OK);
			Exit();
			return;
		}
		else if(Type!=success)
		{
			//网络更新互斥结束
			csNetUpdata.Unlock();
			//断开连接，退出
			MessageBox(L"与服务器断开连接", L"NO", MB_OK);
			Exit();
			return;
		}
		//操作并显示
		OperateData(TheMind, OperateList);
		ChooseLineBlock = false;
		pStaticSelLineBlock->ShowWindow(SW_HIDE);
		OnMindUpdata(0, 0);

		//网络更新互斥结束
		csNetUpdata.Unlock();
	}
	//若选择锁定分块
	else if(ChooseLockBlock || ChooseUnlockBlock)
	{
		//网络更新互斥开始
		csNetUpdata.Lock();

		//判断点击在哪个分块区域内
		BlockNode *pBlock;
		CPoint point2 = RealToImage(point);
		pBlock = ChoiseBlock(TheMind.BlockTree.qnode->pch, point2);

		//若点击空白处
		if(pBlock==0)
		{
			//网络更新互斥结束
			csNetUpdata.Unlock();
			ChooseLockBlock = false;
			ChooseUnlockBlock = false;
			pStaticSelLockBlock->ShowWindow(SW_HIDE);
			return;
		}

		TheMind.BlockTree.pnode = pBlock;
		int deep = TheMind.BlockTree.GetLocal(g_IntBuf);
		//申请并继续编辑
		PacketType Type = LockBlockConnect(deep, g_IntBuf, OperateList, ChooseLockBlock, SetWith);

		if(Type!=success && Type!=fail)
		{
			//网络更新互斥结束
			csNetUpdata.Unlock();
			//断开连接，退出
			MessageBox(L"与服务器断开连接", L"NO", MB_OK);
			Exit();
			return;
		}
		else
		{
			//失败弹窗
			if(Type==fail)
			{
				//网络更新互斥结束
				csNetUpdata.Unlock();

				MessageBox(L"所选分块已被删除", L"NO", MB_OK);
				return;
			}

			//改变
			OperateData(TheMind, OperateList);
			ChooseLockBlock = false;
			ChooseUnlockBlock = false;
			pStaticSelLockBlock->ShowWindow(SW_HIDE);

			//刷新所有显示
			OnMindUpdata(0, 0);

			//网络更新互斥结束
			csNetUpdata.Unlock();
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


//鼠标左键抬起事件
void EditMindDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(EditBlockStatus && LBottonStatus)
	{
		LBottonStatus = false;
		CPoint point2 = RealToImage(point);
		pEditBlock->PosX = point2.x+LBottonPoint.x;
		pEditBlock->PosY = point2.y+LBottonPoint.y;
		
		//数据更新互斥开始
		CClientDC dc(this);

		//限制区域
		if(pEditBlock->PosX<0)
			pEditBlock->PosX = 0;
		else if(pEditBlock->PosX>TheMind.Length-pEditBlock->Length)
			pEditBlock->PosX = TheMind.Length-pEditBlock->Length;
		if(pEditBlock->PosY<0)
			pEditBlock->PosY = 0;
		else if(pEditBlock->PosY>TheMind.Width-pEditBlock->Width)
			pEditBlock->PosY = TheMind.Width-pEditBlock->Width;

		csDataUpdata.Lock();
		PaintMind(&dc);
		//数据更新互斥结束
		csDataUpdata.Unlock();

		if(!RBottonStatus)
			::ReleaseCapture();
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}


//鼠标左键双击事件
void EditMindDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(!rectEditArea.PtInRect(point))
		return;
	BlockNode *pBlock;
	CPoint point2 = RealToImage(point);
	
	//网络更新互斥开始
	csNetUpdata.Lock();
	pBlock = ChoiseBlock(TheMind.BlockTree.qnode->pch, point2);

	//判断点击在哪个分块区域内
	//若为自己区域
	if(EditBlockStatus && pBlock==pEditBlock)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();
		return;
	}
	else
	{
		//若为分块编辑状态
		if(EditBlockStatus)
		{
			//申请退出分块编辑状态
			PacketType Type = FinishBlockConnect(*pEditBlock, OperateList);

			if(Type==fail)
			{
				//失去权限则转换状态并绘图
				TurnMindStatus();
				CClientDC dc(this);
				PaintMind(&dc);

				//网络更新互斥结束
				csNetUpdata.Unlock();

				MessageBox(L"失去编辑权限，修改失败", L"NO", MB_OK);
				return;
			}
			else if(Type==nopermission)
			{
				//网络更新互斥结束
				csNetUpdata.Unlock();
				MessageBox(L"思维导图被删除或已被移出共享用户组", L"NO", MB_OK);
				Exit();
				return;
			}
			else if(Type!=success)
			{
				//网络更新互斥结束
				csNetUpdata.Unlock();
				//断开连接，退出
				MessageBox(L"与服务器断开连接", L"NO", MB_OK);
				Exit();
				return;
			}
			else
			{
				//操作并显示，转换状态
				OperateData(TheMind, OperateList);
				TurnMindStatus();
				OnMindUpdata(0, 0);
			}
		}

		//若未选中分块
		if(pBlock==0)
		{
			//网络更新互斥结束
			csNetUpdata.Unlock();
			return;
		}

		//若在锁定区域内
		if(EditLimit && pBlock->Lock && pBlock->LockUser.GetLength()==0)
		{
			//网络更新互斥结束
			csNetUpdata.Unlock();

			MessageBox(L"此分块禁止共享用户编辑", L"NO", MB_OK);
			return;
		}
		else if(pBlock->Lock && pBlock->LockUser.GetLength()>0)
		{
			//网络更新互斥结束
			csNetUpdata.Unlock();

			MessageBox(L"此分块正在被其他用户编辑", L"NO", MB_OK);
			return;
		}
		//若在新区域内
		else
		{
			TheMind.BlockTree.pnode = pBlock;
			int deep = TheMind.BlockTree.GetLocal(g_IntBuf);
			//申请新分块权限
			PacketType Type = ApplyBlockConnect(g_IntBuf, deep, OperateList);

			if(Type==fail)
			{
				//网络更新互斥结束
				csNetUpdata.Unlock();
				MessageBox(L"没有编辑权限或该分块已被删除", L"NO", MB_OK);
				return;
			}
			else if(Type==nopermission)
			{
				//网络更新互斥结束
				csNetUpdata.Unlock();
				MessageBox(L"思维导图被删除或已被移出共享用户组", L"NO", MB_OK);
				Exit();
				return;
			}
			else if(Type!=success)
			{
				//网络更新互斥结束
				csNetUpdata.Unlock();
				//断开连接，退出
				MessageBox(L"与服务器断开连接", L"NO", MB_OK);
				Exit();
				return;
			}
			else
			{
				//操作并显示，转换状态
				OperateData(TheMind, OperateList);
				TurnBlockStatus(pBlock);
				OnMindUpdata(0, 0);
		
				//网络更新互斥结束
				csNetUpdata.Unlock();
			}
		}
	}

	CDialogEx::OnLButtonDblClk(nFlags, point);
}


//鼠标右键按下事件
void EditMindDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(!rectEditArea.PtInRect(point))
		return;
	RBottonStatus = true;
	RBottonPoint = CornerPoint-point;
	LBottonZoom = ZoomSize[ZoomLevel];
	SetCapture();

	CDialogEx::OnRButtonDown(nFlags, point);
}


//鼠标右键抬起事件
void EditMindDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(RBottonStatus)
	{
		RBottonStatus = false;
		CornerPoint.x = point.x+(int)(RBottonPoint.x*ZoomSize[ZoomLevel]/LBottonZoom);
		CornerPoint.y = point.y+(int)(RBottonPoint.y*ZoomSize[ZoomLevel]/LBottonZoom);

		//数据更新互斥开始
		CClientDC dc(this);
		csDataUpdata.Lock();
		PaintMind(&dc);
		//数据更新互斥结束
		csDataUpdata.Unlock();

		if(!LBottonStatus)
			::ReleaseCapture();
	}

	CDialogEx::OnRButtonUp(nFlags, point);
}


//鼠标右键双击事件
void EditMindDlg::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	if(!rectEditArea.PtInRect(point))
		return;
	CornerPoint.SetPoint(0, 0);
	ZoomLevel = DefaultZoom;
	
	CClientDC dc(this);
	//数据更新互斥开始
	csDataUpdata.Lock();
	PaintMind(&dc);
	//数据更新互斥结束
	csDataUpdata.Unlock();

	CDialogEx::OnRButtonDblClk(nFlags, point);
}


//鼠标移动事件
void EditMindDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	bool change = false;
	CClientDC dc(this);

	if(RBottonStatus)
	{
		CornerPoint.x = point.x+(int)(RBottonPoint.x*ZoomSize[ZoomLevel]/LBottonZoom);
		CornerPoint.y = point.y+(int)(RBottonPoint.y*ZoomSize[ZoomLevel]/LBottonZoom);
		change = true;
	}
	if(EditBlockStatus && LBottonStatus)
	{
		CPoint point2 = RealToImage(point);
		pEditBlock->PosX = point2.x+LBottonPoint.x;
		pEditBlock->PosY = point2.y+LBottonPoint.y;
		change = true;

		//限制区域
		if(pEditBlock->PosX<0)
			pEditBlock->PosX = 0;
		else if(pEditBlock->PosX>TheMind.Length-pEditBlock->Length)
			pEditBlock->PosX = TheMind.Length-pEditBlock->Length;
		if(pEditBlock->PosY<0)
			pEditBlock->PosY = 0;
		else if(pEditBlock->PosY>TheMind.Width-pEditBlock->Width)
			pEditBlock->PosY = TheMind.Width-pEditBlock->Width;
	}

	if(change)
	{
		//数据更新互斥开始
		csDataUpdata.Lock();
		PaintMind(&dc);
		//数据更新互斥结束
		csDataUpdata.Unlock();
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


//鼠标滚轮事件
BOOL EditMindDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	ScreenToClient(&pt);
	if(!rectEditArea.PtInRect(pt))
		return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
	double lastzoom = ZoomSize[ZoomLevel];
	if(zDelta>0)
	{
		if(ZoomLevel>=MaxZoomLevel-1)
			return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
		ZoomLevel ++;
	}
	else
	{
		if(ZoomLevel<=0)
			return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
		ZoomLevel --;
	}
	CornerPoint.x = pt.x+(int)((CornerPoint.x-pt.x)*ZoomSize[ZoomLevel]/lastzoom);
	CornerPoint.y = pt.y+(int)((CornerPoint.y-pt.y)*ZoomSize[ZoomLevel]/lastzoom);

	//数据更新互斥开始
	csDataUpdata.Lock();
	CClientDC dc(this);
	PaintMind(&dc);
	//数据更新互斥结束
	csDataUpdata.Unlock();

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


//消息预处理，这里用来挑拣击键消息
BOOL EditMindDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN
		&& (pMsg->wParam==VK_RIGHT || pMsg->wParam==VK_LEFT || pMsg->wParam==VK_DOWN || pMsg->wParam==VK_UP))
	{
		ProcessKeyDown(pMsg->wParam);
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


//键盘消息处理
void EditMindDlg::ProcessKeyDown(WPARAM Key)
{
	if(EditLimit && EditBlockStatus==false)
		return;

	//数据更新互斥开始
	csDataUpdata.Lock();
	CClientDC dc(this);

	if(EditBlockStatus)
	{
		switch(Key)
		{
		case VK_RIGHT:
			pEditBlock->Length += (int)(5/ZoomSize[ZoomLevel]+1);
			if(pEditBlock->Length>TheMind.Length-pEditBlock->PosX)
				pEditBlock->Length = TheMind.Length-pEditBlock->PosX;
			break;
		case VK_LEFT:
			pEditBlock->Length -= (int)(5/ZoomSize[ZoomLevel]+1);
			if(pEditBlock->Length<MIN_BLOCK_LENGTH)
				pEditBlock->Length = MIN_BLOCK_LENGTH;
			break;
		case VK_DOWN:
			pEditBlock->Width += (int)(5/ZoomSize[ZoomLevel]+1);
			if(pEditBlock->Width>TheMind.Width-pEditBlock->PosY)
				pEditBlock->Width = TheMind.Width-pEditBlock->PosY;
			break;
		case VK_UP:
			pEditBlock->Width -= (int)(5/ZoomSize[ZoomLevel]+1);
			if(pEditBlock->Width<MIN_BLOCK_WIDTH)
				pEditBlock->Width = MIN_BLOCK_WIDTH;
			break;
		}
	}
	else
	{
		switch(Key)
		{
		case VK_RIGHT:
			TheMind.Length += (int)(10/ZoomSize[ZoomLevel]+1);
			break;
		case VK_LEFT:
			TheMind.Length -= (int)(10/ZoomSize[ZoomLevel]+1);
			if(TheMind.Length<TheMind.Pos_X)
				TheMind.Length = TheMind.Pos_X;
			if(TheMind.Length<MIN_LENGTH)
				TheMind.Length = MIN_LENGTH;
			break;
		case VK_DOWN:
			TheMind.Width += (int)(10/ZoomSize[ZoomLevel]+1);
			break;
		case VK_UP:
			TheMind.Width -= (int)(10/ZoomSize[ZoomLevel]+1);
			if(TheMind.Width<TheMind.Pos_Y)
				TheMind.Width = TheMind.Pos_Y;
			if(TheMind.Width<MIN_WIDTH)
				TheMind.Width = MIN_WIDTH;
			break;
		}
		pTimerDlg->UpdataFlag = true;
	}

	PaintMind(&dc);
	//数据更新互斥结束
	csDataUpdata.Unlock();
}


//OK消息事件
void EditMindDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//拒绝响应
	//CDialogEx::OnOK();
}


//关闭按钮点击事件
void EditMindDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//退出
	Exit();
}


//更新思维导图消息，若调用只能在网络互斥中调用
afx_msg LRESULT EditMindDlg::OnMindUpdata(WPARAM wParam, LPARAM lParam)
{
	//数据更新互斥开始
	csDataUpdata.Lock();

	CString string;
	//仅在限制编辑时更新某些信息，防止正在编辑被破坏
	if(EditLimit==true)
	{
		//导图名字编辑框
		pEditMindName->SetWindowTextW(TheMind.MindName);
	}
	//常规更新
	//共享用户列表框
	RefreshShareList(true);
	//编辑人数静态框
	string.Format(L"编辑人数：%d", TheMind.EditNum);
	pStaticEditNum->SetWindowTextW(string);
	//共享人数静态框
	string.Format(L"共享人数：%d", TheMind.ShareNum);
	pStaticShareNum->SetWindowTextW(string);
	//只读复选按钮
	pCheckReadOnly->SetCheck(TheMind.ReadOnly? BST_CHECKED:BST_UNCHECKED);

	//重绘图
	CClientDC dc(this);
	PaintMind(&dc);

	//数据更新互斥结束
	csDataUpdata.Unlock();
	return 0;
}


//网络错误或没有权限消息
afx_msg LRESULT EditMindDlg::OnInternetError(WPARAM wParam, LPARAM lParam)
{
	if(wParam==0)
	{
		MessageBox(L"与服务器断开连接", L"NO", MB_OK);
		Exit();
	}
	else if(wParam==1)
	{
		MessageBox(L"思维导图被删除或已被移出共享用户组", L"NO", MB_OK);
		Exit();
	}
	else if(wParam==2)
	{
		MessageBox(L"没有编辑权限", L"NO", MB_OK);
		TurnMindStatus();
		CClientDC dc(this);
		PaintMind(&dc);
	}
	return 0;
}


//导图名称编辑框字符改变事件
void EditMindDlg::OnChangeEditMindname()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	CString string;
	pEditMindName->GetWindowTextW(string);
	//若为回车则取消输入焦点
	if(string==LastMindName)
	{
		SetFocus();
	}
	LastMindName = string;
}


//导图名称编辑框失去输入焦点，过滤窗口切换，可被回车触发
void EditMindDlg::OnKillfocusEditMindname()
{
	// TODO: 在此添加控件通知处理程序代码

	//若由于切换窗口引起则不做处理
	if(GetFocus()==0)
		return;

	CString string;
	pEditMindName->GetWindowTextW(string);
	if(NameRule(string))
	{
		//合法
		if(string==TheMind.MindName)
			//未改变退出
			return;

		//网络更新互斥开始
		csNetUpdata.Lock();

		//同步服务器
		PacketType Type = ChangeNameConnect(string);
		if(Type==fail)
		{
			//网络更新互斥结束
			csNetUpdata.Unlock();
			//重名，恢复
			MessageBox(L"该名称已存在", L"NO", MB_OK);
			pEditMindName->SetWindowTextW(TheMind.MindName);
			LastMindName = TheMind.MindName;
		}
		else if(Type!=success)
		{
			//网络更新互斥结束
			csNetUpdata.Unlock();
			//断开连接，退出
			MessageBox(L"与服务器断开连接", L"NO", MB_OK);
			Exit();
		}
		else
		{
			//改变
			TheMind.MindName = string;
			pTimerDlg->TheMind.MindName = string;
			//网络更新互斥结束
			csNetUpdata.Unlock();
		}
	}
	else
	{
		//不合法，恢复
		MessageBox(L"名称不合法", L"NO", MB_OK);
		pEditMindName->SetWindowTextW(TheMind.MindName);
		LastMindName = TheMind.MindName;
	}
}


//加入共享用户按钮点击事件
void EditMindDlg::OnBnClickedButtonAddshare()
{
	// TODO: 在此添加控件通知处理程序代码

	CString string;
	pEditAddShare->GetWindowTextW(string);
	//单击判断合法性
	if(LandDlg::StringRule(string)==false)
	{
		MessageBox(L"用户名不合法", L"NO", MB_OK);
		return;
	}

	//数据更新互斥开始
	csDataUpdata.Lock();

	int result;
	if(BinarySearch(TheMind.ShareUser, string, 0, TheMind.ShareNum-1, result))
	{
		//数据更新互斥结束
		csDataUpdata.Unlock();
		
		MessageBox(L"该用户已被共享", L"NO", MB_OK);
		return;
	}
	else if(g_UserId==string)
	{
		//数据更新互斥结束
		csDataUpdata.Unlock();
		
		MessageBox(L"不能共享自己", L"NO", MB_OK);
		return;
	}
	else
	{
		//数据更新互斥结束
		csDataUpdata.Unlock();
	}

	//网络更新互斥开始
	csNetUpdata.Lock();

	//同步服务器
	PacketType Type = AddShareConnect(string);
	if(Type==fail)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();
		MessageBox(L"该用户名不存在", L"NO", MB_OK);
	}
	else if(Type!=success)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();
		//断开连接，退出
		MessageBox(L"与服务器断开连接", L"NO", MB_OK);
		Exit();
	}
	else
	{
		//改变
		int i;
		for(i=TheMind.ShareNum-1; i>result; i--)
		{
			TheMind.ShareUser[i+1] = TheMind.ShareUser[i];
			TheMind.UserStatus[i+1] = TheMind.UserStatus[i];
		}
		TheMind.ShareUser[result+1] = string;
		TheMind.UserStatus[result+1] = false;
		TheMind.ShareNum ++;
		RefreshShareList(false);
		//网络更新互斥结束
		csNetUpdata.Unlock();
	}
}


//删除共享用户按钮点击事件
void EditMindDlg::OnBnClickedButtonDelshare()
{
	// TODO: 在此添加控件通知处理程序代码

	int Index = pListShareList->GetCurSel();
	if(Index==-1)
	{
		MessageBox(L"请选择一项", L"NO", MB_OK);
		return;
	}
	
	//网络更新互斥开始
	csNetUpdata.Lock();

	//同步服务器
	PacketType Type = DelShareConnect(TheMind.ShareUser[Index]);
	if(Type!=success)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();
		//断开连接，退出
		MessageBox(L"与服务器断开连接", L"NO", MB_OK);
		Exit();
	}
	else
	{
		//改变
		int i;
		for(i=Index; i<TheMind.ShareNum-1; i++)
		{
			TheMind.ShareUser[i] = TheMind.ShareUser[i+1];
			TheMind.UserStatus[i] = TheMind.UserStatus[i+1];
		}
		TheMind.ShareNum --;
		RefreshShareList(false);
		//网络更新互斥结束
		csNetUpdata.Unlock();
	}
}


//只读复选按钮点击事件
void EditMindDlg::OnClickedCheckReadonly()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//数据更新互斥开始
	csDataUpdata.Lock();

	pTimerDlg->UpdataFlag = true;
	TheMind.ReadOnly = pCheckReadOnly->GetCheck()==BST_CHECKED? true:false;

	//数据更新互斥结束
	csDataUpdata.Unlock();
}


//设置连带复选按钮点击事件
void EditMindDlg::OnClickedCheckSetwith()
{
	// TODO: 在此添加控件通知处理程序代码

	SetWith = pCheckSetWith->GetCheck()==BST_CHECKED? true:false;
}


//添加根节点按钮点击事件
void EditMindDlg::OnBnClickedButtonAddroot()
{
	// TODO: 在此添加控件通知处理程序代码

	CPoint point = RealToImage(CPoint(0, 0));
	
	//网络更新互斥开始
	csNetUpdata.Lock();
	
	//同步服务器
	PacketType Type = BlockAddRootConnect(point.x, point.y, OperateList);

	if(Type!=success && Type!=fail)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();
		//断开连接，退出
		MessageBox(L"与服务器断开连接", L"NO", MB_OK);
		Exit();
		return;
	}
	else
	{
		//改变
		OperateData(TheMind, OperateList);

		//失败弹窗
		if(Type==fail)
		{
			OnMindUpdata(0, 0);
			//网络更新互斥结束
			csNetUpdata.Unlock();

			MessageBox(L"思维导图没有足够尺寸添加新分块", L"NO", MB_OK);
			return;
		}

		//转换节点编辑状态
		TurnBlockStatus(TheMind.BlockTree.qnode->pch);

		//刷新所有显示
		OnMindUpdata(0, 0);

		//网络更新互斥结束
		csNetUpdata.Unlock();
	}
}


//分块文字编辑框字符改变事件
void EditMindDlg::OnChangeEditBlocktext()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	CString string;
	pEditBlockText->GetWindowTextW(string);
	//若为回车则取消输入焦点
	if(string==LastBlockText)
	{
		SetFocus();
	}
	LastBlockText = string;
}


//分块文字编辑框失去输入焦点，过滤窗口切换，可被回车触发
void EditMindDlg::OnKillfocusEditBlocktext()
{
	// TODO: 在此添加控件通知处理程序代码

	//若由于切换窗口引起则不做处理
	if(GetFocus()==0)
		return;

	CString string;
	pEditBlockText->GetWindowTextW(string);

	//数据更新互斥开始
	csDataUpdata.Lock();

	//改变
	pEditBlock->Text = string;
	
	//重绘图
	CClientDC dc(this);
	PaintMind(&dc);

	//数据更新互斥结束
	csDataUpdata.Unlock();
}


//删除连带子分块按钮点击事件
void EditMindDlg::OnBnClickedButtonDelwith()
{
	// TODO: 在此添加控件通知处理程序代码

	if(EditLimit && pEditBlock->ppr->ppr==0)
	{
		MessageBox(L"共享用户没有进行涉及根分块关系的操作权限", L"NO", MB_OK);
		return;
	}

	//网络更新互斥开始
	csNetUpdata.Lock();

	//申请退出分块编辑状态
	PacketType Type = BlockDelWithConnect(*pEditBlock, OperateList);

	if(Type==fail)
	{
		//失去权限则转换状态并绘图
		TurnMindStatus();
		CClientDC dc(this);
		PaintMind(&dc);

		//网络更新互斥结束
		csNetUpdata.Unlock();

		MessageBox(L"失去编辑权限，修改失败", L"NO", MB_OK);
		return;
	}
	else if(Type==clash)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();
		MessageBox(L"操作涉及他人正在操作或拥有者锁定的分块", L"NO", MB_OK);
		return;
	}
	else if(Type==nopermission)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();
		MessageBox(L"思维导图被删除或已被移出共享用户组", L"NO", MB_OK);
		Exit();
		return;
	}
	else if(Type!=success)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();
		//断开连接，退出
		MessageBox(L"与服务器断开连接", L"NO", MB_OK);
		Exit();
		return;
	}
	//操作并转换状态显示
	OperateData(TheMind, OperateList);
	TurnMindStatus();
	OnMindUpdata(0, 0);

	//网络更新互斥结束
	csNetUpdata.Unlock();
}


//删除合并子分块按钮点击事件
void EditMindDlg::OnBnClickedButtonDelcombine()
{
	// TODO: 在此添加控件通知处理程序代码

	if(EditLimit && pEditBlock->ppr->ppr==0)
	{
		MessageBox(L"共享用户没有进行涉及根分块关系的操作权限", L"NO", MB_OK);
		return;
	}

	//网络更新互斥开始
	csNetUpdata.Lock();

	//申请退出分块编辑状态
	PacketType Type = BlockDelCombineConnect(*pEditBlock, OperateList);

	if(Type==fail)
	{
		//失去权限则转换状态并绘图
		TurnMindStatus();
		CClientDC dc(this);
		PaintMind(&dc);

		//网络更新互斥结束
		csNetUpdata.Unlock();

		MessageBox(L"失去编辑权限，修改失败", L"NO", MB_OK);
		return;
	}
	else if(Type==clash)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();
		MessageBox(L"操作涉及他人正在操作或拥有者锁定的分块", L"NO", MB_OK);
		return;
	}
	else if(Type==nopermission)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();
		MessageBox(L"思维导图被删除或已被移出共享用户组", L"NO", MB_OK);
		Exit();
		return;
	}
	else if(Type!=success)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();
		//断开连接，退出
		MessageBox(L"与服务器断开连接", L"NO", MB_OK);
		Exit();
		return;
	}
	//操作并转换状态显示
	OperateData(TheMind, OperateList);
	TurnMindStatus();
	OnMindUpdata(0, 0);

	//网络更新互斥结束
	csNetUpdata.Unlock();
}


//添加子分块按钮点击事件
void EditMindDlg::OnBnClickedButtonAddchild()
{
	// TODO: 在此添加控件通知处理程序代码

	//网络更新互斥开始
	csNetUpdata.Lock();

	//申请并继续编辑
	PacketType Type = BlockAddChildConnect(*pEditBlock, OperateList, pEditBlock->PosX, pEditBlock->PosY);

	if(Type==fail)
	{
		//失去权限则转换状态并绘图
		TurnMindStatus();
		CClientDC dc(this);
		PaintMind(&dc);

		//网络更新互斥结束
		csNetUpdata.Unlock();

		MessageBox(L"失去编辑权限，修改失败", L"NO", MB_OK);
		return;
	}
	else if(Type==clash)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();
		MessageBox(L"操作涉及他人正在操作或拥有者锁定的分块", L"NO", MB_OK);
		return;
	}
	else if(Type==nopermission)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();
		MessageBox(L"思维导图被删除或已被移出共享用户组", L"NO", MB_OK);
		Exit();
		return;
	}
	else if(Type!=success)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();
		//断开连接，退出
		MessageBox(L"与服务器断开连接", L"NO", MB_OK);
		Exit();
		return;
	}
	//操作并显示
	OperateData(TheMind, OperateList);
	OnMindUpdata(0, 0);

	//网络更新互斥结束
	csNetUpdata.Unlock();
}


//添加父分块按钮点击事件
void EditMindDlg::OnBnClickedButtonAddparent()
{
	// TODO: 在此添加控件通知处理程序代码

	if(EditLimit && pEditBlock->ppr->ppr==0)
	{
		MessageBox(L"共享用户没有进行涉及根分块关系的操作权限", L"NO", MB_OK);
		return;
	}

	//网络更新互斥开始
	csNetUpdata.Lock();
	
	//申请并继续编辑
	PacketType Type = BlockAddParentConnect(*pEditBlock, OperateList, pEditBlock->PosX, pEditBlock->PosY);

	if(Type==fail)
	{
		//失去权限则转换状态并绘图
		TurnMindStatus();
		CClientDC dc(this);
		PaintMind(&dc);

		//网络更新互斥结束
		csNetUpdata.Unlock();

		MessageBox(L"失去编辑权限，修改失败", L"NO", MB_OK);
		return;
	}
	else if(Type==clash)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();
		MessageBox(L"操作涉及他人正在操作或拥有者锁定的分块", L"NO", MB_OK);
		return;
	}
	else if(Type==nopermission)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();
		MessageBox(L"思维导图被删除或已被移出共享用户组", L"NO", MB_OK);
		Exit();
		return;
	}
	else if(Type!=success)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();
		//断开连接，退出
		MessageBox(L"与服务器断开连接", L"NO", MB_OK);
		Exit();
		return;
	}
	//操作并显示
	OperateData(TheMind, OperateList);
	OnMindUpdata(0, 0);

	//网络更新互斥结束
	csNetUpdata.Unlock();
}


//连接到已有分块按钮点击事件
void EditMindDlg::OnBnClickedButtonLineto()
{
	// TODO: 在此添加控件通知处理程序代码

	if(EditLimit && pEditBlock->ppr->ppr==0)
	{
		MessageBox(L"共享用户没有进行涉及根分块关系的操作权限", L"NO", MB_OK);
		return;
	}

	ChooseLineBlock = true;
	pStaticSelLineBlock->ShowWindow(SW_SHOW);
}


//分离为根分块按钮点击事件
void EditMindDlg::OnBnClickedButtonSeparateroot()
{
	// TODO: 在此添加控件通知处理程序代码

	//网络更新互斥开始
	csNetUpdata.Lock();
	
	//申请并继续编辑
	PacketType Type = BlockSeparateRootConnect(*pEditBlock, OperateList);

	if(Type==fail)
	{
		//失去权限则转换状态并绘图
		TurnMindStatus();
		CClientDC dc(this);
		PaintMind(&dc);

		//网络更新互斥结束
		csNetUpdata.Unlock();

		MessageBox(L"失去编辑权限，修改失败", L"NO", MB_OK);
		return;
	}
	else if(Type==clash)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();
		MessageBox(L"操作涉及他人正在操作或拥有者锁定的分块", L"NO", MB_OK);
		return;
	}
	else if(Type==nopermission)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();
		MessageBox(L"思维导图被删除或已被移出共享用户组", L"NO", MB_OK);
		Exit();
		return;
	}
	else if(Type!=success)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();
		//断开连接，退出
		MessageBox(L"与服务器断开连接", L"NO", MB_OK);
		Exit();
		return;
	}
	//操作并显示
	OperateData(TheMind, OperateList);
	OnMindUpdata(0, 0);

	//网络更新互斥结束
	csNetUpdata.Unlock();
}


//删除分离子分块按钮点击事件
void EditMindDlg::OnBnClickedButtonDelseparete()
{
	// TODO: 在此添加控件通知处理程序代码

	//网络更新互斥开始
	csNetUpdata.Lock();

	//申请退出分块编辑状态
	PacketType Type = BlockDelSeparateConnect(*pEditBlock, OperateList);

	if(Type==fail)
	{
		//失去权限则转换状态并绘图
		TurnMindStatus();
		CClientDC dc(this);
		PaintMind(&dc);

		//网络更新互斥结束
		csNetUpdata.Unlock();

		MessageBox(L"失去编辑权限，修改失败", L"NO", MB_OK);
		return;
	}
	else if(Type==clash)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();
		MessageBox(L"操作涉及他人正在操作或拥有者锁定的分块", L"NO", MB_OK);
		return;
	}
	else if(Type==nopermission)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();
		MessageBox(L"思维导图被删除或已被移出共享用户组", L"NO", MB_OK);
		Exit();
		return;
	}
	else if(Type!=success)
	{
		//网络更新互斥结束
		csNetUpdata.Unlock();
		//断开连接，退出
		MessageBox(L"与服务器断开连接", L"NO", MB_OK);
		Exit();
		return;
	}
	//操作并转换状态显示
	OperateData(TheMind, OperateList);
	TurnMindStatus();
	OnMindUpdata(0, 0);

	//网络更新互斥结束
	csNetUpdata.Unlock();
}


//设置红色编辑框字符更改事件
void EditMindDlg::OnChangeEditRed()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	if(ColorAcceptFlag==false)
		return;

	CString string, string2;
	pEditRed->GetWindowTextW(string);
	int num = CStringTo255(string);
	
	//数据更新互斥开始
	csDataUpdata.Lock();

	//改变
	ColorAcceptFlag = false;
	SETR(num, pEditBlock->Color);
	string2.Format(L"%d", num);
	pSliderRed->SetPos(num);
	if(string!=string2)
	{
		pEditRed->SetWindowTextW(string2);
	}
	ColorAcceptFlag = true;
	
	//重绘图
	CClientDC dc(this);
	PaintMind(&dc);

	//数据更新互斥结束
	csDataUpdata.Unlock();
}


//设置绿色编辑框字符更改事件
void EditMindDlg::OnChangeEditGreen()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	if(ColorAcceptFlag==false)
		return;

	CString string, string2;
	pEditGreen->GetWindowTextW(string);
	int num = CStringTo255(string);
	
	//数据更新互斥开始
	csDataUpdata.Lock();

	//改变
	ColorAcceptFlag = false;
	SETG(num, pEditBlock->Color);
	string2.Format(L"%d", num);
	pSliderGreen->SetPos(num);
	if(string!=string2)
	{
		pEditGreen->SetWindowTextW(string2);
	}
	ColorAcceptFlag = true;
	
	//重绘图
	CClientDC dc(this);
	PaintMind(&dc);

	//数据更新互斥结束
	csDataUpdata.Unlock();
}


//设置蓝色编辑框字符更改事件
void EditMindDlg::OnChangeEditBlue()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	if(ColorAcceptFlag==false)
		return;

	CString string, string2;
	pEditBlue->GetWindowTextW(string);
	int num = CStringTo255(string);
	
	//数据更新互斥开始
	csDataUpdata.Lock();

	//改变
	ColorAcceptFlag = false;
	SETB(num, pEditBlock->Color);
	string2.Format(L"%d", num);
	pSliderBlue->SetPos(num);
	if(string!=string2)
	{
		pEditBlue->SetWindowTextW(string2);
	}
	ColorAcceptFlag = true;
	
	//重绘图
	CClientDC dc(this);
	PaintMind(&dc);

	//数据更新互斥结束
	csDataUpdata.Unlock();
}


//设置红色滑动框重绘事件
void EditMindDlg::OnCustomdrawSliderRed(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int num = pSliderRed->GetPos();
	if(pEditBlock && num!=GETR(pEditBlock->Color))
	{
		//数据更新互斥开始
		csDataUpdata.Lock();

		//改变
		ColorAcceptFlag = false;
		SETR(num, pEditBlock->Color);
		CString string;
		string.Format(L"%d", num);
		pEditRed->SetWindowTextW(string);
		ColorAcceptFlag = true;

		//重绘图
		CClientDC dc(this);
		PaintMind(&dc);

		//数据更新互斥结束
		csDataUpdata.Unlock();
	}
}


//设置绿色滑动框重绘事件
void EditMindDlg::OnCustomdrawSliderGreen(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int num = pSliderGreen->GetPos();
	if(pEditBlock && num!=GETG(pEditBlock->Color))
	{
		//数据更新互斥开始
		csDataUpdata.Lock();

		//改变
		ColorAcceptFlag = false;
		SETG(num, pEditBlock->Color);
		CString string;
		string.Format(L"%d", num);
		pEditGreen->SetWindowTextW(string);
		ColorAcceptFlag = true;

		//重绘图
		CClientDC dc(this);
		PaintMind(&dc);

		//数据更新互斥结束
		csDataUpdata.Unlock();
	}
}


//设置蓝色滑动框重绘事件
void EditMindDlg::OnCustomdrawSliderBlue(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int num = pSliderBlue->GetPos();
	if(pEditBlock && num!=GETB(pEditBlock->Color))
	{
		//数据更新互斥开始
		csDataUpdata.Lock();

		//改变
		ColorAcceptFlag = false;
		SETB(num, pEditBlock->Color);
		CString string;
		string.Format(L"%d", num);
		pEditBlue->SetWindowTextW(string);
		ColorAcceptFlag = true;

		//重绘图
		CClientDC dc(this);
		PaintMind(&dc);

		//数据更新互斥结束
		csDataUpdata.Unlock();
	}
}


//恢复为白色按钮点击事件
void EditMindDlg::OnBnClickedButtonSetwhite()
{
	// TODO: 在此添加控件通知处理程序代码

	//数据更新互斥开始
	csDataUpdata.Lock();

	//改变
	ColorAcceptFlag = false;
	pEditBlock->Color = RGB(255, 255, 255);
	CString string;
	string.Format(L"%d", 255);
	pEditRed->SetWindowTextW(string);
	pEditGreen->SetWindowTextW(string);
	pEditBlue->SetWindowTextW(string);
	pSliderRed->SetPos(255);
	pSliderGreen->SetPos(255);
	pSliderBlue->SetPos(255);
	ColorAcceptFlag = true;

	//重绘图
	CClientDC dc(this);
	PaintMind(&dc);

	//数据更新互斥结束
	csDataUpdata.Unlock();
}


//选择形状组合框选项改变事件
void EditMindDlg::OnSelchangeComboShap()
{
	// TODO: 在此添加控件通知处理程序代码

	int num = pComboShape->GetCurSel();
	if(num==-1)
		assert(0);

	//数据更新互斥开始
	csDataUpdata.Lock();

	pEditBlock->Shape = (enumShape)num;

	//重绘图
	CClientDC dc(this);
	PaintMind(&dc);

	//数据更新互斥结束
	csDataUpdata.Unlock();
}


//选择方向组合框选项改变事件
void EditMindDlg::OnSelchangeComboLinedir()
{
	// TODO: 在此添加控件通知处理程序代码

	int num = pComboLineDir->GetCurSel();
	if(num==-1)
		assert(0);

	//数据更新互斥开始
	csDataUpdata.Lock();

	pEditBlock->Dir = (enumDir)num;

	//重绘图
	CClientDC dc(this);
	PaintMind(&dc);

	//数据更新互斥结束
	csDataUpdata.Unlock();
}


//锁定分块编辑按钮点击事件
void EditMindDlg::OnBnClickedButtonSetlock()
{
	// TODO: 在此添加控件通知处理程序代码

	ChooseLockBlock = true;
	pStaticSelLockBlock->ShowWindow(SW_SHOW);
}


//启用分块编辑按钮点击事件
void EditMindDlg::OnBnClickedButtonSetunlock()
{
	// TODO: 在此添加控件通知处理程序代码

	ChooseUnlockBlock = true;
	pStaticSelLockBlock->ShowWindow(SW_SHOW);
}


//检查命名规则
bool EditMindDlg::NameRule(CString &string)
{
	int len = string.GetLength();
	int i;
	if(len<1 || len>20)
		return false;
	for(i=0; i<len; i++)
	{
		if(string[i]<=256)
		{
			if(!(string[i]>='0' && string[i]<='9' || string[i]>='a' && string[i]<='z' || string[i]>='A' && string[i]<='Z'))
				return false;
		}
	}
	return true;
}


//字符串压缩数字到255
int EditMindDlg::CStringTo255(CString &string)
{
	int n = string.GetLength();
	if(n>3)
		return 255;
	else if(n==3)
	{
		if(string[0]>'2' || string[0]=='2' && string[1]>'5' 
			|| string[0]=='2' && string[1]=='5' && string[2]>'5')
			return 255;
		return (string[0]-'0')*100+(string[1]-'0')*10+(string[2]-'0');
	}
	else if(n==2)
		return (string[0]-'0')*10+(string[1]-'0');
	else if(n==1)
		return (string[0]-'0');
	else
		return 0;
}


//刷新共享用户列表框显示
void EditMindDlg::RefreshShareList(bool KeepSel)
{
	int Index = pListShareList->GetCurSel();
	pListShareList->ResetContent();
	int i;
	CString string;
	int Tab[] = {80};
	pListShareList->SetTabStops(1, Tab);
	for(i=0; i<TheMind.ShareNum; i++)
	{
		string = TheMind.ShareUser[i];
		if(TheMind.UserStatus[i])
			string += L"\t在线";
		pListShareList->AddString(string);
	}
	if(KeepSel)
		pListShareList->SetCurSel(Index);
}


//思维导图绘图
void EditMindDlg::PaintMind(CDC *pdc)
{
	CBrush brush;
	CPen pen;
	CFont font;
	CRect rect;
	CPoint point1, point2;

	//定义两个缓冲位图
	CDC dcMem;
	CBitmap bitmap;
	dcMem.CreateCompatibleDC(pdc);
	bitmap.CreateCompatibleBitmap(pdc, rectEditArea.Width(), rectEditArea.Height());
	dcMem.SelectObject(&bitmap);
	CDC dcMem2;
	CBitmap bitmap2;
	dcMem2.CreateCompatibleDC(pdc);
	bitmap2.CreateCompatibleBitmap(pdc, rectEditArea.Width(), rectEditArea.Height());
	dcMem2.SelectObject(&bitmap2);

	//分别填充两个位图
	dcMem2.FillSolidRect(rectEditArea.left, rectEditArea.top, 
		rectEditArea.Width(), rectEditArea.Height(), RGB(201,211,227));
	
	dcMem.FillSolidRect(rectEditArea.left, rectEditArea.top, 
		rectEditArea.Width(), rectEditArea.Height(), RGB(255,255,255));

	//设置坐标映射
	dcMem.SetMapMode(MM_ANISOTROPIC);
	dcMem.SetWindowExt(rectEditArea.right, rectEditArea.bottom);
	dcMem.SetViewportExt((int)(rectEditArea.right*ZoomSize[ZoomLevel]), 
		(int)(rectEditArea.bottom*ZoomSize[ZoomLevel]));
	dcMem.SetViewportOrg(CornerPoint);

	//绘制连线
	pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	brush.CreateSolidBrush(RGB(0, 0, 0));
	dcMem.SelectObject(pen);
	dcMem.SelectObject(brush);
	PaintLine(&dcMem, TheMind.BlockTree.qnode->pch);
	pen.DeleteObject();
	brush.DeleteObject();

	//绘制分块
	font.CreatePointFont((int)(160/**ZoomSize[ZoomLevel]*/), _T("Times New Roman"));
	dcMem.SelectObject(font);
	dcMem.SetBkMode(TRANSPARENT);
	PaintBlock(&dcMem, TheMind.BlockTree.qnode->pch, brush, pen);
	font.DeleteObject();

	//绘制正在编辑分块轮廓线
	if(EditBlockStatus)
	{
		pen.CreatePen(PS_SOLID, 4, RGB(0, 0, 255));
		dcMem.SelectObject(pen);
		dcMem.SelectStockObject(NULL_BRUSH);
		PaintShape(&dcMem, pEditBlock->Shape, CRect(pEditBlock->PosX, pEditBlock->PosY, 
			pEditBlock->PosX+pEditBlock->Length, pEditBlock->PosY+pEditBlock->Width));
		pen.DeleteObject();
	}

	//两次位图转换
	dcMem2.StretchBlt(CornerPoint.x, CornerPoint.y, (int)(TheMind.Length*ZoomSize[ZoomLevel]), 
		(int)(TheMind.Width*ZoomSize[ZoomLevel]), &dcMem, 0, 0, TheMind.Length, TheMind.Width, SRCCOPY);

	pdc->BitBlt(rectEditArea.left, rectEditArea.top, rectEditArea.Width(), rectEditArea.Height(), 
		&dcMem2, 0, 0, SRCCOPY);
	//pdc->BitBlt(0, 0, rectEditArea.Width(), rectEditArea.Height(), &dcMem, 0, 0, SRCCOPY);

	////设置剪切区域
	//pdc->SetMapMode(MM_TEXT);
	//CRgn rgn;
	//rgn.CreateRectRgnIndirect(rectEditArea);
	//pdc->SelectClipRgn(&rgn);

	////覆盖背景为原始色调
	//pdc->SelectStockObject(NULL_PEN);
	//brush.CreateSolidBrush(RGB(201,211,227));
	//pdc->SelectObject(&brush);
	//pdc->Rectangle(rectBackArea);
	//brush.DeleteObject();

	////设置坐标映射
	//pdc->SetMapMode(MM_ISOTROPIC);
	//pdc->SetWindowExt(rectEditArea.right, rectEditArea.bottom);
	//pdc->SetViewportExt((int)(rectEditArea.right*ZoomSize[ZoomLevel]), 
	//	(int)(rectEditArea.bottom*ZoomSize[ZoomLevel]));
	//pdc->SetViewportOrg(CornerPoint);
	//
	////画导图背景
	//pdc->SelectStockObject(NULL_PEN);
	//brush.CreateSolidBrush(RGB(255,255,255));
	//pdc->SelectObject(&brush);
	//point1.SetPoint(0, 0);
	//point2.SetPoint(TheMind.Length, TheMind.Width);
	//rect.SetRect(point1, point2);
	//pdc->Rectangle(rect);
	//brush.DeleteObject();

	////绘制分块
	//font.CreatePointFont((int)(100/**ZoomSize[ZoomLevel]*/), _T("Times New Roman"));
	//pdc->SelectObject(font);
	//pdc->SetBkMode(TRANSPARENT);
	//PaintBlock(pdc, TheMind.BlockTree.qnode->pch, brush, pen);
	//font.DeleteObject();

	////绘制正在编辑分块轮廓线
	//if(EditBlockStatus)
	//{
	//	pen.CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
	//	pdc->SelectObject(pen);
	//	pdc->SelectStockObject(NULL_BRUSH);
	//	PaintShape(pdc, pEditBlock->Shape, CRect(pEditBlock->PosX, pEditBlock->PosY, 
	//		pEditBlock->PosX+pEditBlock->Length, pEditBlock->PosY+pEditBlock->Width));
	//	pen.DeleteObject();
	//}
}


//思维导图分块递归绘图
void EditMindDlg::PaintBlock(CDC *pdc, BlockNode *pBlock, CBrush &brush, CPen &pen)
{
	CRect rect;
	if(pBlock==0)
		return;
	while(pBlock->pt)
		LIST_GO(pBlock, pt);
	while(pBlock)
	{
		//绘制子分块
		PaintBlock(pdc, pBlock->pch, brush, pen);

		//绘制自身
		//根据锁定状态选择线条样式
		if(pBlock->Lock && pBlock->LockUser.GetLength()==0)
			pen.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
		else if(pBlock->Lock)
			pen.CreatePen(PS_SOLID, 4, RGB(128, 128, 128));
		else
			pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		brush.CreateSolidBrush(pBlock->Color);
		pdc->SelectObject(pen);
		pdc->SelectObject(brush);
		rect.SetRect(pBlock->PosX, pBlock->PosY, 
			pBlock->PosX+pBlock->Length, pBlock->PosY+pBlock->Width);
		PaintShape(pdc, pBlock->Shape, rect);
		pen.DeleteObject();
		brush.DeleteObject();
		//绘制文字
		pdc->DrawText(pBlock->Text, &rect, DT_SINGLELINE|DT_CENTER|DT_VCENTER);

		//走向前兄弟分块
		LIST_GO(pBlock, qt);
	}
}


//思维导图连线递归绘图
void EditMindDlg::PaintLine(CDC *pdc, BlockNode *pBlock)
{
	CRect rect;
	if(pBlock==0)
		return;
	while(pBlock->pt)
		LIST_GO(pBlock, pt);
	while(pBlock)
	{
		//非根分块则绘制父分块的连线
		if(pBlock->ppr->ppr)
		{
			//贝塞尔曲线连线
			CPoint pointArray[4];
			switch(pBlock->Dir)
			{
			case dirup:
				pointArray[0].SetPoint(pBlock->ppr->PosX+pBlock->ppr->Length/2, pBlock->ppr->PosY+pBlock->ppr->Width);
				pointArray[3].SetPoint(pBlock->PosX+pBlock->Length/2, pBlock->PosY);
				pointArray[1].SetPoint(pointArray[0].x, (pointArray[0].y+pointArray[3].y)/2);
				pointArray[2].SetPoint(pointArray[3].x, (pointArray[0].y+pointArray[3].y)/2);
				break;
			case dirdown:
				pointArray[0].SetPoint(pBlock->ppr->PosX+pBlock->ppr->Length/2, pBlock->ppr->PosY);
				pointArray[3].SetPoint(pBlock->PosX+pBlock->Length/2, pBlock->PosY+pBlock->Width);
				pointArray[1].SetPoint(pointArray[0].x, (pointArray[0].y+pointArray[3].y)/2);
				pointArray[2].SetPoint(pointArray[3].x, (pointArray[0].y+pointArray[3].y)/2);
				break;
			case dirleft:
				if(pBlock->ppr->Shape==eche || pBlock->ppr->Shape==para)
					pointArray[0].SetPoint(pBlock->ppr->PosX+pBlock->ppr->Length*3/4, pBlock->ppr->PosY+pBlock->ppr->Width/2);
				else
					pointArray[0].SetPoint(pBlock->ppr->PosX+pBlock->ppr->Length, pBlock->ppr->PosY+pBlock->ppr->Width/2);
				if(pBlock->Shape==eche || pBlock->Shape==para)
					pointArray[3].SetPoint(pBlock->PosX+pBlock->ppr->Length*1/4, pBlock->PosY+pBlock->Width/2);
				else
					pointArray[3].SetPoint(pBlock->PosX, pBlock->PosY+pBlock->Width/2);
				pointArray[1].SetPoint((pointArray[0].x+pointArray[3].x)/2, pointArray[0].y);
				pointArray[2].SetPoint((pointArray[0].x+pointArray[3].x)/2, pointArray[3].y);
				break;
			case dirright:
				if(pBlock->ppr->Shape==eche || pBlock->ppr->Shape==para)
					pointArray[0].SetPoint(pBlock->ppr->PosX+pBlock->ppr->Length*1/4, pBlock->ppr->PosY+pBlock->ppr->Width/2);
				else
					pointArray[0].SetPoint(pBlock->ppr->PosX, pBlock->ppr->PosY+pBlock->ppr->Width/2);
				if(pBlock->Shape==eche || pBlock->Shape==para)
					pointArray[3].SetPoint(pBlock->PosX+pBlock->Length*3/4, pBlock->PosY+pBlock->Width/2);
				else
					pointArray[3].SetPoint(pBlock->PosX+pBlock->Length, pBlock->PosY+pBlock->Width/2);
				pointArray[1].SetPoint((pointArray[0].x+pointArray[3].x)/2, pointArray[0].y);
				pointArray[2].SetPoint((pointArray[0].x+pointArray[3].x)/2, pointArray[3].y);
				break;
			}
			pdc->PolyBezier(pointArray, 4);
			//求切线画三角形
			double centerX = (double)(pointArray[0].x+pointArray[3].x)/2;
			double centerY = (double)(pointArray[0].y+pointArray[3].y)/2;
			double tempDX = pointArray[3].x+pointArray[2].x-pointArray[1].x-pointArray[0].x;
			double tempDY = pointArray[3].y+pointArray[2].y-pointArray[1].y-pointArray[0].y;
			double tempDR = sqrt(tempDX*tempDX+tempDY*tempDY);
			tempDR = max(tempDR, 1);
			double proportionX = tempDX/tempDR;
			double proportionY = tempDY/tempDR;
			double len = 5;
			double temp, tempX, tempY;
			tempX = centerX+len*proportionX;
			tempY = centerY+len*proportionY;
			pointArray[0].SetPoint((int)(tempX+modf(tempX, &temp)), (int)(tempY+modf(tempY, &temp)));
			tempX = centerX-len*proportionX-len*proportionY;
			tempY = centerY-len*proportionY+len*proportionX;
			pointArray[1].SetPoint((int)(tempX+modf(tempX, &temp)), (int)(tempY+modf(tempY, &temp)));
			tempX = centerX-len*proportionX+len*proportionY;
			tempY = centerY-len*proportionY-len*proportionX;
			pointArray[2].SetPoint((int)(tempX+modf(tempX, &temp)), (int)(tempY+modf(tempY, &temp)));
			pdc->Polygon(pointArray, 3);
		}

		//绘制子分块
		PaintLine(pdc, pBlock->pch);

		//走向前兄弟分块
		LIST_GO(pBlock, qt);
	}
}


//绘制形状
void EditMindDlg::PaintShape(CDC *pdc, enumShape Shape, CRect &rectSize)
{
	CPoint point[8];
	int temp;
	switch(Shape)
	{
	case rect:
		pdc->Rectangle(rectSize);
		break;
	case rore:
		temp = min(rectSize.Width(), rectSize.Height())*2/3;
		point[0].SetPoint(temp, temp);
		pdc->RoundRect(rectSize, point[0]);
		break;
	case round:
		pdc->Ellipse(rectSize);
		break;
	case eche:
		point[0].SetPoint((3*rectSize.left+rectSize.right)/4, rectSize.top);
		point[1].SetPoint((rectSize.left+3*rectSize.right)/4, rectSize.top);
		point[2].SetPoint(rectSize.right, rectSize.bottom);
		point[3].SetPoint(rectSize.left, rectSize.bottom);
		pdc->Polygon(point, 4);
		break;
	case diamond:
		point[0].SetPoint((rectSize.left+rectSize.right)/2, rectSize.top);
		point[1].SetPoint(rectSize.right, (rectSize.top+rectSize.bottom)/2);
		point[2].SetPoint((rectSize.left+rectSize.right)/2, rectSize.bottom);
		point[3].SetPoint(rectSize.left, (rectSize.top+rectSize.bottom)/2);
		pdc->Polygon(point, 4);
		break;
	case para:
		point[0].SetPoint((3*rectSize.left+rectSize.right)/4, rectSize.top);
		point[1].SetPoint(rectSize.right, rectSize.top);
		point[2].SetPoint((rectSize.left+3*rectSize.right)/4, rectSize.bottom);
		point[3].SetPoint(rectSize.left, rectSize.bottom);
		pdc->Polygon(point, 4);
		break;
	case octa:
		point[0].SetPoint((3*rectSize.left+rectSize.right)/4, rectSize.top);
		point[1].SetPoint((rectSize.left+3*rectSize.right)/4, rectSize.top);
		point[2].SetPoint(rectSize.right, (3*rectSize.top+rectSize.bottom)/4);
		point[3].SetPoint(rectSize.right, (rectSize.top+3*rectSize.bottom)/4);
		point[4].SetPoint((rectSize.left+3*rectSize.right)/4, rectSize.bottom);
		point[5].SetPoint((3*rectSize.left+rectSize.right)/4, rectSize.bottom);
		point[6].SetPoint(rectSize.left, (rectSize.top+3*rectSize.bottom)/4);
		point[7].SetPoint(rectSize.left, (3*rectSize.top+rectSize.bottom)/4);
		pdc->Polygon(point, 8);
		break;
	}
}


//思维导图递归选择所在分块
BlockNode *EditMindDlg::ChoiseBlock(BlockNode *pBlock, CPoint &point)
{
	CRect rect;
	BlockNode *MyBlock = 0, *tempBlock;
	if(pBlock==0)
		return 0;
	while(pBlock->pt)
		LIST_GO(pBlock, pt);
	while(pBlock)
	{
		//判断子分块
		tempBlock = ChoiseBlock(pBlock->pch, point);
		if(tempBlock)
			MyBlock = tempBlock;

		//判断自身
		rect.SetRect(pBlock->PosX, pBlock->PosY, 
			pBlock->PosX+pBlock->Length, pBlock->PosY+pBlock->Width);
		if(JustShape(pBlock->Shape, rect, point))
			MyBlock = pBlock;

		//走向前兄弟分块
		LIST_GO(pBlock, qt);
	}
	return MyBlock;
}


//判断形状
bool EditMindDlg::JustShape(enumShape Shape, CRect &rectSize, CPoint &JudgePoint)
{
	CPoint point[8];
	CRgn rgn;
	switch(Shape)
	{
	case rect:
		rgn.CreateRectRgnIndirect(&rectSize);
		break;
	case round:
		rgn.CreateEllipticRgnIndirect(&rectSize);
		break;
	case rore:
		rgn.CreateRoundRectRgn(rectSize.left, rectSize.top, rectSize.right, rectSize.bottom, 
			rectSize.Width()/3, rectSize.Height()/3);
		break;
	case eche:
		point[0].SetPoint((3*rectSize.left+rectSize.right)/4, rectSize.top);
		point[1].SetPoint((rectSize.left+3*rectSize.right)/4, rectSize.top);
		point[2].SetPoint(rectSize.right, rectSize.bottom);
		point[3].SetPoint(rectSize.left, rectSize.bottom);
		rgn.CreatePolygonRgn(point, 4, ALTERNATE);
		break;
	case diamond:
		point[0].SetPoint((rectSize.left+rectSize.right)/2, rectSize.top);
		point[1].SetPoint(rectSize.right, (rectSize.top+rectSize.bottom)/2);
		point[2].SetPoint((rectSize.left+rectSize.right)/2, rectSize.bottom);
		point[3].SetPoint(rectSize.left, (rectSize.top+rectSize.bottom)/2);
		rgn.CreatePolygonRgn(point, 4, ALTERNATE);
		break;
	case para:
		point[0].SetPoint((3*rectSize.left+rectSize.right)/4, rectSize.top);
		point[1].SetPoint(rectSize.right, rectSize.top);
		point[2].SetPoint((rectSize.left+3*rectSize.right)/4, rectSize.bottom);
		point[3].SetPoint(rectSize.left, rectSize.bottom);
		rgn.CreatePolygonRgn(point, 4, ALTERNATE);
		break;
	case octa:
		point[0].SetPoint((3*rectSize.left+rectSize.right)/4, rectSize.top);
		point[1].SetPoint((rectSize.left+3*rectSize.right)/4, rectSize.top);
		point[2].SetPoint(rectSize.right, (3*rectSize.top+rectSize.bottom)/4);
		point[3].SetPoint(rectSize.right, (rectSize.top+3*rectSize.bottom)/4);
		point[4].SetPoint((rectSize.left+3*rectSize.right)/4, rectSize.bottom);
		point[5].SetPoint((3*rectSize.left+rectSize.right)/4, rectSize.bottom);
		point[6].SetPoint(rectSize.left, (rectSize.top+3*rectSize.bottom)/4);
		point[7].SetPoint(rectSize.left, (3*rectSize.top+rectSize.bottom)/4);
		rgn.CreatePolygonRgn(point, 8, ALTERNATE);
		break;
	}
	return (rgn.PtInRegion(JudgePoint)!=0);
}


//图像坐标转真实坐标
CPoint EditMindDlg::ImageToReal(const CPoint &point)
{
	CPoint NewPoint;
	NewPoint.x = CornerPoint.x+(int)(point.x*ZoomSize[ZoomLevel]);
	NewPoint.y = CornerPoint.y+(int)(point.y*ZoomSize[ZoomLevel]);
	return NewPoint;
}
//真实坐标转图像坐标
CPoint EditMindDlg::RealToImage(const CPoint &point)
{
	CPoint NewPoint;
	NewPoint.x = (int)((point.x-CornerPoint.x)/ZoomSize[ZoomLevel]);
	NewPoint.y = (int)((point.y-CornerPoint.y)/ZoomSize[ZoomLevel]);
	return NewPoint;
}


//转换分块编辑状态
void EditMindDlg::TurnBlockStatus(BlockNode *pBlock)
{
	//转换状态量
	EditBlockStatus = true;
	pEditBlock = pBlock;
	LBottonStatus = false;

	ChooseLockBlock = false;
	ChooseUnlockBlock = false;
	pStaticSelLockBlock->ShowWindow(SW_HIDE);

	//初始化控件内容
	//分块文字编辑框
	pEditBlockText->SetWindowTextW(pEditBlock->Text);
	LastBlockText = pEditBlock->Text;
	//分块锁定静态框
	if(pBlock->LockUser.GetLength()==0)
		pStaticLock->SetWindowTextW(L"此分块共享用户不可修改");
	else
		pStaticLock->SetWindowTextW(L"此分块共享用户可以修改");
	//设置颜色编辑框
	ColorAcceptFlag = false;
	CString string;
	string.Format(L"%d", GETR(pEditBlock->Color));
	pEditRed->SetWindowTextW(string);
	string.Format(L"%d", GETG(pEditBlock->Color));
	pEditGreen->SetWindowTextW(string);
	string.Format(L"%d", GETB(pEditBlock->Color));
	pEditBlue->SetWindowTextW(string);
	ColorAcceptFlag = true;
	//设置颜色滑动框
	pSliderRed->SetPos(GETR(pEditBlock->Color));
	pSliderGreen->SetPos(GETG(pEditBlock->Color));
	pSliderBlue->SetPos(GETB(pEditBlock->Color));
	//设置形状组合框
	pComboShape->SetCurSel(pEditBlock->Shape);
	//设置方向组合框
	pComboLineDir->SetCurSel(pEditBlock->Dir);

	int i;
	//切换控件显示
	for(i=0; i<sizeof(MindFreeItem)/sizeof(int); i++)
		pMindFreeItem[i]->ShowWindow(SW_HIDE);
	for(i=0; i<sizeof(MindLimitItem)/sizeof(int); i++)
		pMindLimitItem[i]->ShowWindow(SW_HIDE);
	for(i=0; i<sizeof(BlockFreeItem)/sizeof(int); i++)
		pBlockFreeItem[i]->ShowWindow(SW_SHOW);
	for(i=0; i<sizeof(BlockLimitItem)/sizeof(int); i++)
		pBlockLimitItem[i]->ShowWindow(SW_SHOW);
}
//转换导图编辑状态
void EditMindDlg::TurnMindStatus()
{
	//转换状态量
	EditBlockStatus = false;
	pEditBlock = 0;

	ChooseLineBlock = false;
	pStaticSelLineBlock->ShowWindow(SW_HIDE);


	int i;
	//切换控件显示
	for(i=0; i<sizeof(MindFreeItem)/sizeof(int); i++)
		pMindFreeItem[i]->ShowWindow(SW_SHOW);
	for(i=0; i<sizeof(MindLimitItem)/sizeof(int); i++)
		pMindLimitItem[i]->ShowWindow(SW_SHOW);
	for(i=0; i<sizeof(BlockFreeItem)/sizeof(int); i++)
		pBlockFreeItem[i]->ShowWindow(SW_HIDE);
	for(i=0; i<sizeof(BlockLimitItem)/sizeof(int); i++)
		pBlockLimitItem[i]->ShowWindow(SW_HIDE);
}


//退出
void EditMindDlg::Exit()
{
	::SendMessage(pTimerDlg->m_hWnd, WM_CLOSE, 0, 0);
	CDialogEx::OnCancel();
}

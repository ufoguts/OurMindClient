#pragma once


// EditMindDlg 对话框

/*
主线程：
响应消息，
主用户编辑思维导图分为两种情况，立即上传形和延迟上传形
立即上传提出网络申请，阻塞主界面，
延迟上传将标志位给定时器线程，等待定时器线程上传
分块编辑为申请和释放两步
申请分块时，申请分块同时请求更新
注意此时申请的分块可能会改变分块定位数组，由服务器操作
返回的新的分块定位数组，使用此进行分块编辑
保存分块编辑指针，使得更新结构不会影响编辑
释放分块同理，释放分块同时请求更新
改变定位数组同上
注意不在本机直接操作分块更新，可能会失败或在前插入其他关系操作
由服务器决定，本机只需处理返回的操作链表即可
关系操作处于分块申请释放之间，此时要想服务器发送请求
之后也会返回操作链表
逻辑由服务器维护，客户端进行操作即可

第二线程：
时钟中断控制0.2s
定时向服务器申请更新
对于主用户，当标志位置位时上传导图信息同时发送更新请求
新建导图副本存储下载更新，保证短时间完成粘贴
导图更新有权限控制，分块更新有申请机制，关系更新不影响指针
故第二线程不会发生编辑冲突

线程同步：
设置两个临界区，网络临界区和数据临界区
网络临界区逻辑上包含数据临界区
对于计时器线程，顺序为：
{
网络临界区锁定
数据临界区锁定
网络行为
数据临界区解锁
网络临界区解锁
数据临界区锁定
数据行为
数据临界区解锁
}
故主线程需要网络调用时，使用网络临界区
需要数据操作时，使用数据临界区

*/


#define WM_MIND_UPDATA WM_USER+1
#define WM_INTERNET_ERROR WM_USER+2

#define GETR(r) ((BYTE)(r))
#define GETG(g) ((BYTE)((g)>>8))
#define GETB(b) ((BYTE)((b)>>16))
#define SETR(r, color) ((color) = (color)&(0xFFFF00)|(BYTE)(r))
#define SETG(g, color) ((color) = (color)&(0xFF00FF)|((BYTE)(g)<<8))
#define SETB(b, color) ((color) = (color)&(0x00FFFF)|((BYTE)(b)<<16))


class EditMindDlg : public CDialogEx
{
	//友元
	friend class COurMindClientDlg;//父类


	DECLARE_DYNAMIC(EditMindDlg)

public:
	EditMindDlg(const bool EditLimit, CWnd* pParent=NULL);   // 标准构造函数
	virtual ~EditMindDlg();

// 对话框数据
	enum { IDD = IDD_EDITMINDDLG };

	//变量
public:
	//思维导图主体变量
	MindNode TheMind;//当前思维导图，为复制而非引用
	const bool EditLimit;//编辑限制
	ListTemplet<class OperateNode> OperateList;//接收操作链表

	//思维导图绘图变量
	CRect rectEditArea;//编辑区域
	CRect rectBackArea;//覆盖区域
	CPoint CornerPoint;//角点坐标
	int ZoomLevel;//缩放等级
	const static double ZoomSize[];//缩放尺寸
	const static int MaxZoomLevel;//最大缩放等级
	const static int DefaultZoom;//默认缩放等级

	//思维导图状态变量
	bool EditBlockStatus;//编辑节点状态
	BlockNode *pEditBlock;//编辑节点
	bool LBottonStatus;//左键点击状态
	CPoint LBottonPoint;//左键点击坐标
	double LBottonZoom;//右键点击缩放记录
	bool RBottonStatus;//右键点击状态
	CPoint RBottonPoint;//右键点击坐标
	bool ChooseLineBlock;//选择连线分块状态
	bool ChooseLockBlock;//选择锁定分块状态
	bool ChooseUnlockBlock;//选择解锁分块状态

	//线程同步
	CWinThread *pThread;//同步工作者线程
	class TimerDlg *pTimerDlg;//开启的同步线程窗口
	CCriticalSection csNetUpdata;//网络更新临界区
	CCriticalSection csDataUpdata;//数据更新临界区

	//控件指针及其辅助变量
	CEdit *pEditMindName;//导图名字编辑框
	CString LastMindName;//导图名字辅助变量
	CListBox *pListShareList;//共享用户列表框
	CEdit *pEditAddShare;//添加共享用户编辑框
	CStatic *pStaticEditNum;//编辑人数静态框
	CStatic *pStaticShareNum;//共享人数静态框
	CButton *pCheckReadOnly;//只读复选按钮
	CButton *pCheckSetWith;//设置连带复选按钮
	bool SetWith;//设置连带状态变量
	CEdit *pEditBlockText;//分块文字编辑框
	CString LastBlockText;//分块文字辅助变量
	CStatic *pStaticLock;//分块锁定静态框
	CEdit *pEditRed;//设置红色编辑框
	CEdit *pEditGreen;//设置绿色编辑框
	CEdit *pEditBlue;//设置蓝色编辑框
	bool ColorAcceptFlag;//颜色接收外部变化标志
	CSliderCtrl *pSliderRed;//设置红色滑动框
	CSliderCtrl *pSliderGreen;//设置绿色滑动框
	CSliderCtrl *pSliderBlue;//设置蓝色滑动框
	CComboBox *pComboShape;//选择形状组合框
	CStatic *pStaticSelLineBlock;//选择连接分块静态框
	CStatic *pStaticSelLockBlock;//选择锁定分块静态框
	CComboBox *pComboLineDir;//选择方向组合框

	//列举控件ID
protected:
	const static int MindFreeItem[];//思维导图无限制控件
	const static int MindLimitItem[];//思维导图限制控件
	const static int BlockFreeItem[];//分块无限制控件
	const static int BlockLimitItem[];//分块限制控件
	const static int AlwaysHideItem[];//常隐藏控件
	CWnd *pMindFreeItem[100];//思维导图无限制控件指针
	CWnd *pMindLimitItem[100];//思维导图限制控件指针
	CWnd *pBlockFreeItem[100];//分块无限制控件指针
	CWnd *pBlockLimitItem[100];//分块限制控件指针
	CWnd *pAlwaysHideItem[100];//常隐藏控件指针

	//方法
protected:
	static UINT ThreadFunc(LPVOID pParam);//同步工作者线程回调函数
	bool NameRule(CString &string);//检查命名规则
	int CStringTo255(CString &string);//字符串压缩数字到255
	void RefreshShareList(bool KeepSel);//刷新共享用户列表框显示
	void PaintMind(CDC *pdc);//思维导图绘图
	void PaintBlock(CDC *pdc, BlockNode *pBlock, CBrush &brush, CPen &pen);//思维导图分块递归绘图
	void PaintLine(CDC *pdc, BlockNode *pBlock);//思维导图连线递归绘图
	void EditMindDlg::PaintShape(CDC *pdc, enumShape Shape, CRect &rect);//绘制形状
	BlockNode *ChoiseBlock(BlockNode *pBlock, CPoint &point);//思维导图递归选择所在分块
	bool JustShape(enumShape Shape, CRect &rectSize, CPoint &JudgePoint);//判断形状
	CPoint ImageToReal(const CPoint &point);//图像坐标转真实坐标
	CPoint RealToImage(const CPoint &point);//真实坐标转图像坐标
	void TurnBlockStatus(BlockNode *pBlock);//转换分块编辑状态
	void TurnMindStatus();//转换导图编辑状态
	void ProcessKeyDown(WPARAM Key);//键盘消息处理
	void Exit();//退出窗口

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnChangeEditMindname();
	afx_msg void OnKillfocusEditMindname();
protected:
	afx_msg LRESULT OnMindUpdata(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInternetError(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonAddshare();
	afx_msg void OnBnClickedButtonDelshare();
	afx_msg void OnClickedCheckReadonly();
	afx_msg void OnClickedCheckSetwith();
	afx_msg void OnBnClickedButtonAddroot();
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnChangeEditBlocktext();
	afx_msg void OnKillfocusEditBlocktext();
	afx_msg void OnBnClickedButtonDelwith();
	afx_msg void OnBnClickedButtonDelcombine();
	afx_msg void OnBnClickedButtonAddchild();
	afx_msg void OnBnClickedButtonAddparent();
	afx_msg void OnBnClickedButtonLineto();
	afx_msg void OnBnClickedButtonSeparateroot();
	afx_msg void OnBnClickedButtonDelseparete();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnChangeEditRed();
	afx_msg void OnChangeEditGreen();
	afx_msg void OnChangeEditBlue();
	afx_msg void OnCustomdrawSliderRed(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCustomdrawSliderGreen(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCustomdrawSliderBlue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSetwhite();
	afx_msg void OnSelchangeComboShap();
	afx_msg void OnSelchangeComboLinedir();
	afx_msg void OnBnClickedButtonSetlock();
	afx_msg void OnBnClickedButtonSetunlock();
};

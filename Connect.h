
#pragma once

//网络处理文件"


extern CString g_ServerIp;
extern sockaddr_in g_ServerSocket;//服务器套接字


/****************************************************************/
/*************************服务器同步开始*************************/
/****************************************************************/

#define SERVER_PORT "12345"


//tcp包上级协议
enum PacketType
{
	//标志位
	defaultvalue,//缺省值
	error,//连接错误
	success,//成功
	fail,//失败
	nopermission,//没有权限
	clash,//冲突

	//用户信息操作
	hello,//连接服务器
	login,//登录
	regist,//注册
	change,//修改信息
	logout,//注销
	getlist,//请求获取思维导图列表
	newmind,//新建思维导图
	selmind,//选择思维导图
	delmind,//删除思维导图
	editend,//结束编辑

	//导图编辑操作
	changename,//更改名字
	addshare,//添加共享用户
	delshare,//删除共享用户
	lockblock,//设置锁定，解锁
	upmind,//上传思维导图
	keepmind,//无改变思维导图
	downmind,//下载思维导图
	applyblock,//申请编辑分块
	finishblock,//完成分块编辑
	blockaddroot,//添加根分块
	blockdelwith,//分块删除连带子分块
	blockdelcombine,//分块删除合并子分块
	blockaddchild,//分块插入子分块
	blockaddparent,//分块插入父分块
	blocklineto,//分块连接到新分块
	blockseparateroot,//分块分离为根分块
	blockdelseparate,//分块删除分离子分块

};


//保证接收长度函数
void CertainRecv(SOCKET sock, void *buf, int len);


//发送CString
void SendCString(SOCKET sock, CString &string);


//接收CString
void RecvCString(SOCKET sock, CString &string);

/****************************************************************/
/*************************服务器同步结束*************************/
/****************************************************************/



//网络初始化函数
void ConnetcInit();


//测试服务器ip函数
bool TestServerIp(CString &Ip);


//登录网络交互
PacketType LoginConnect(CString &UserId, CString &Password);


//注册网络交互
PacketType RegistConnect(CString &UserId, CString &Password);


//修改信息网络交互
PacketType ChangeConnect(CString &UserId, CString &Password, CString &Password2);


//请求获取思维导图列表
PacketType GetListConnect(ListTemplet<MindNode> &MyMindList, ListTemplet<MindNode> &OurMindList);


//新建思维导图
PacketType NewMindConnect(MindNode &RcvMind);


//选择思维导图
PacketType SelMindConnect(MindNode &RcvMind, MindNode &TheMind);


//删除思维导图
PacketType DelMindConnect(CString MindName);


//通知结束编辑
PacketType EditEndConnect();


//通知更改名字
PacketType ChangeNameConnect(CString &Name);


//添加共享用户
PacketType AddShareConnect(CString UserId);


//删除共享用户
PacketType DelShareConnect(CString UserId);


//通知设置锁定解锁
PacketType LockBlockConnect(int Deep, int Local[], ListTemplet<class OperateNode> &OperateList, 
							bool IsLock, bool SetWith);


//上传思维导图
PacketType UpMindConnect(MindNode &TheMind, ListTemplet<class OperateNode> &OperateList);


//保持思维导图
PacketType KeepMindConnect(ListTemplet<class OperateNode> &OperateList);


//下载思维导图
PacketType DownMindConnect(ListTemplet<class OperateNode> &OperateList);


//通知申请分块编辑
PacketType ApplyBlockConnect(int Local[], int Deep, ListTemplet<class OperateNode> &OperateList);


//通知完成分块编辑
PacketType FinishBlockConnect(BlockNode &TheBlock, ListTemplet<class OperateNode> &OperateList);


//通知添加根分块
PacketType BlockAddRootConnect(int x, int y, ListTemplet<class OperateNode> &OperateList);


//通知删除连带子分块
PacketType BlockDelWithConnect(BlockNode &TheBlock, ListTemplet<class OperateNode> &OperateList);


//通知删除合并子分块
PacketType BlockDelCombineConnect(BlockNode &TheBlock, ListTemplet<class OperateNode> &OperateList);


//通知添加子分块
PacketType BlockAddChildConnect(BlockNode &TheBlock, ListTemplet<class OperateNode> &OperateList, 
								int x, int y);


//通知插入父分块
PacketType BlockAddParentConnect(BlockNode &TheBlock, ListTemplet<class OperateNode> &OperateList, 
								 int x, int y);


//通知连接到已有分块
PacketType BlockLineToConnect(BlockNode &TheBlock, ListTemplet<class OperateNode> &OperateList, 
								 int Deep, int Local[]);


//通知分离为根分块
PacketType BlockSeparateRootConnect(BlockNode &TheBlock, ListTemplet<class OperateNode> &OperateList);


//通知分离为根分块
PacketType BlockDelSeparateConnect(BlockNode &TheBlock, ListTemplet<class OperateNode> &OperateList);

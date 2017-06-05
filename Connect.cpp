
//网络处理文件


#include "stdafx.h"
#include "OurMindClient.h"
#include "OurMindClientDlg.h"


CString g_ServerIp;//绑定服务器IP
sockaddr_in g_ServerSocket;//服务器套接字



/****************************************************************/
/*************************服务器同步开始*************************/
/****************************************************************/
//保证接收长度函数
void CertainRecv(SOCKET sock, void *buf, int len)
{
	static char buf2[100000];
	int num = 0;
	int temp;
	while(num<len)
	{
		temp = recv(sock, buf2+num, len-num, 0);
		if(temp>0)
			num += temp;
	}
	memcpy(buf, buf2, len);
}


//发送CString
void SendCString(SOCKET sock, CString &string)
{
	int len = string.GetLength()+1;
	send(sock, (char *)&len, sizeof(int), 0);
	send(sock, (char *)(LPCWSTR)string, sizeof(wchar_t)*len, 0);//用户名
}


//接收CString
void RecvCString(SOCKET sock, CString &string)
{
	static wchar_t buf[100000] = {0};
	int len;
	CertainRecv(sock, &len, sizeof(int));
	CertainRecv(sock, buf, sizeof(wchar_t)*len);
	string = buf;
}
/****************************************************************/
/*************************服务器同步结束*************************/
/****************************************************************/



//网络初始化函数
void ConnetcInit()
{
	WSADATA wsaData;
    int iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
    if ( iResult != NO_ERROR )
        //printf("Error at WSAStartup()\n");
		assert(0);
}


//测试服务器ip函数
bool TestServerIp(CString &Ip)
{
	
    //set up the connection建立连接，输入的是对方的ip端口
    g_ServerSocket.sin_family = AF_INET;
    g_ServerSocket.sin_addr.s_addr = inet_addr(CT2A(Ip));// "127.0.0.1" );
    g_ServerSocket.sin_port = htons(atoi(SERVER_PORT));

	//尝试连接服务器
	SOCKET ConnectSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

	if (ConnectSocket==INVALID_SOCKET)
	{
		//printf( "Error at socket(): %ld\n", WSAGetLastError() );
		assert(0);
	}

	//若连接不上，则返回false
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
	{
		return false;
	}
	
	//发送连接标志包
	PacketType type;
	type = hello;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);

	//接收数据包
	CertainRecv(ConnectSocket, &type, sizeof(type));
	bool result;
	if(type==success)
		result = true;
	else
		result = false;

	//断开连接
	closesocket(ConnectSocket);

	return result;
}


//登录网络交互
PacketType LoginConnect(CString &UserId, CString &Password)
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = login;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, UserId);//用户名
	SendCString(ConnectSocket, Password);//密码
	//接收数据
	CertainRecv(ConnectSocket, &type, sizeof(type));//标志

	//断开连接
	closesocket(ConnectSocket);
	return type;
}


//注册网络交互
PacketType RegistConnect(CString &UserId, CString &Password)
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = regist;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, UserId);//用户名
	SendCString(ConnectSocket, Password);//密码
	//接收数据
	CertainRecv(ConnectSocket, &type, sizeof(type));//标志

	//断开连接
	closesocket(ConnectSocket);
	return type;
}


//修改信息网络交互
PacketType ChangeConnect(CString &UserId, CString &Password, CString &Password2)
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = change;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, UserId);//用户名
	SendCString(ConnectSocket, Password);//密码
	SendCString(ConnectSocket, Password2);//新密码
	//接收数据
	CertainRecv(ConnectSocket, &type, sizeof(type));//标志

	//断开连接
	closesocket(ConnectSocket);
	return type;
}


//请求获取思维导图列表
PacketType GetListConnect(ListTemplet<MindNode> &MyMindList, ListTemplet<MindNode> &OurMindList)
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = getlist;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, g_UserId);//用户名
	//接收数据
	MyMindList.Recv(ConnectSocket);//拥有思维导图列表
	OurMindList.Recv(ConnectSocket);//参与思维导图列表

	//断开连接
	closesocket(ConnectSocket);
	return success;
}


//新建思维导图
PacketType NewMindConnect(MindNode &RcvMind)
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = newmind;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, g_UserId);//用户名
	//接收数据
	RcvMind.Recv(ConnectSocket);//新思维导图
	RcvMind.BlockTree.Recv(ConnectSocket);//新思维导图分块树

	//断开连接
	closesocket(ConnectSocket);
	return success;
}


//选择思维导图
PacketType SelMindConnect(MindNode &RcvMind, MindNode &TheMind)
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = selmind;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, g_UserId);//用户名
	SendCString(ConnectSocket, TheMind.UserId);//拥有者
	SendCString(ConnectSocket, TheMind.MindName);//思维导图名称
	//接收数据
	CertainRecv(ConnectSocket, &type, sizeof(type));//标志
	if(type==success)
	{
		RcvMind.Recv(ConnectSocket);//思维导图
		RcvMind.BlockTree.Recv(ConnectSocket);//思维导图分块树
	}

	//断开连接
	closesocket(ConnectSocket);
	return type;
}


//删除思维导图
PacketType DelMindConnect(CString MindName)
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = delmind;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, g_UserId);//用户名
	SendCString(ConnectSocket, MindName);//思维导图名

	//断开连接
	closesocket(ConnectSocket);
	return success;
}


//通知结束编辑
PacketType EditEndConnect()
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = editend;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, g_UserId);//用户名

	//断开连接
	closesocket(ConnectSocket);
	return success;
}


//通知更改名字
PacketType ChangeNameConnect(CString &Name)
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = changename;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, g_UserId);//用户名
	SendCString(ConnectSocket, Name);//新名称
	//接收数据
	CertainRecv(ConnectSocket, &type, sizeof(type));//标志

	//断开连接
	closesocket(ConnectSocket);
	return type;
}


//添加共享用户
PacketType AddShareConnect(CString UserId)
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = addshare;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, g_UserId);//用户名
	SendCString(ConnectSocket, UserId);//共享用户名
	//接收数据
	CertainRecv(ConnectSocket, &type, sizeof(type));//标志

	//断开连接
	closesocket(ConnectSocket);
	return type;
}


//删除共享用户
PacketType DelShareConnect(CString UserId)
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = delshare;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, g_UserId);//用户名
	SendCString(ConnectSocket, UserId);//共享用户名
	//接收数据
	CertainRecv(ConnectSocket, &type, sizeof(type));//标志

	//断开连接
	closesocket(ConnectSocket);
	return type;
}


//通知设置锁定解锁
PacketType LockBlockConnect(int Deep, int Local[], ListTemplet<class OperateNode> &OperateList, 
							bool IsLock, bool SetWith)
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = lockblock;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, g_UserId);//用户名
	send(ConnectSocket, (char *)&Deep, sizeof(Deep), 0);//定位个数
	send(ConnectSocket, (char *)Local, sizeof(int)*Deep, 0);//定位数组
	send(ConnectSocket, (char *)&IsLock, sizeof(IsLock), 0);//是否锁定
	send(ConnectSocket, (char *)&SetWith, sizeof(SetWith), 0);//设置连带
	//接收数据
	CertainRecv(ConnectSocket, &type, sizeof(type));//标志
	if(type==success)
		OperateList.Recv(ConnectSocket);//操作链表

	//断开连接
	closesocket(ConnectSocket);
	return type;
}


//上传思维导图
PacketType UpMindConnect(MindNode &TheMind, ListTemplet<class OperateNode> &OperateList)
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = upmind;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, g_UserId);//用户名
	TheMind.Send(ConnectSocket);//思维导图
	//接收数据
	CertainRecv(ConnectSocket, &type, sizeof(type));//标志
	if(type==success)
		OperateList.Recv(ConnectSocket);//操作链表

	//断开连接
	closesocket(ConnectSocket);
	return type;
}


//保持思维导图
PacketType KeepMindConnect(ListTemplet<class OperateNode> &OperateList)
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = keepmind;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, g_UserId);//用户名
	//接收数据
	CertainRecv(ConnectSocket, &type, sizeof(type));//标志
	if(type==success)
		OperateList.Recv(ConnectSocket);//操作链表

	//断开连接
	closesocket(ConnectSocket);
	return type;
}


//下载思维导图
PacketType DownMindConnect(ListTemplet<class OperateNode> &OperateList)
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = downmind;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, g_UserId);//用户名
	//接收数据
	CertainRecv(ConnectSocket, &type, sizeof(type));//标志
	if(type==success || type==fail)
		OperateList.Recv(ConnectSocket);//操作链表

	//断开连接
	closesocket(ConnectSocket);
	return type;
}


//通知申请分块编辑
PacketType ApplyBlockConnect(int Local[], int Deep, ListTemplet<class OperateNode> &OperateList)
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = applyblock;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, g_UserId);//用户名
	send(ConnectSocket, (char *)&Deep, sizeof(Deep), 0);//深度
	send(ConnectSocket, (char *)Local, sizeof(int)*Deep, 0);//定位数组
	//接收数据
	CertainRecv(ConnectSocket, &type, sizeof(type));//标志
	if(type==success)
		OperateList.Recv(ConnectSocket);//操作链表

	//断开连接
	closesocket(ConnectSocket);
	return type;
}


//通知完成分块编辑
PacketType FinishBlockConnect(BlockNode &TheBlock, ListTemplet<class OperateNode> &OperateList)
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = finishblock;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, g_UserId);//用户名
	TheBlock.Send(ConnectSocket);//分块数据
	//接收数据
	CertainRecv(ConnectSocket, &type, sizeof(type));//标志
	if(type==success)
		OperateList.Recv(ConnectSocket);//操作链表
	else if(type==fail)
		TheBlock.Recv(ConnectSocket);//恢复分块

	//断开连接
	closesocket(ConnectSocket);
	return type;
}


//通知添加根分块
PacketType BlockAddRootConnect(int x, int y, ListTemplet<class OperateNode> &OperateList)
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = blockaddroot;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, g_UserId);//用户名
	send(ConnectSocket, (char *)&x, sizeof(x), 0);//坐标x
	send(ConnectSocket, (char *)&y, sizeof(y), 0);//坐标y
	//接收数据
	CertainRecv(ConnectSocket, &type, sizeof(type));//标志
	if(type==success)
		OperateList.Recv(ConnectSocket);//操作链表

	//断开连接
	closesocket(ConnectSocket);
	return type;
}


//通知删除连带子分块
PacketType BlockDelWithConnect(BlockNode &TheBlock, ListTemplet<class OperateNode> &OperateList)
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = blockdelwith;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, g_UserId);//用户名
	//接收数据
	CertainRecv(ConnectSocket, &type, sizeof(type));//标志
	if(type==success)
		OperateList.Recv(ConnectSocket);//操作链表
	else if(type==fail)
		TheBlock.Recv(ConnectSocket);//恢复分块

	//断开连接
	closesocket(ConnectSocket);
	return type;
}


//通知删除合并子分块
PacketType BlockDelCombineConnect(BlockNode &TheBlock, ListTemplet<class OperateNode> &OperateList)
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = blockdelcombine;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, g_UserId);//用户名
	//接收数据
	CertainRecv(ConnectSocket, &type, sizeof(type));//标志
	if(type==success)
		OperateList.Recv(ConnectSocket);//操作链表
	else if(type==fail)
		TheBlock.Recv(ConnectSocket);//恢复分块

	//断开连接
	closesocket(ConnectSocket);
	return type;
}


//通知添加子分块
PacketType BlockAddChildConnect(BlockNode &TheBlock, ListTemplet<class OperateNode> &OperateList, 
								int x, int y)
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = blockaddchild;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, g_UserId);//用户名
	send(ConnectSocket, (char *)&x, sizeof(x), 0);//坐标x
	send(ConnectSocket, (char *)&y, sizeof(y), 0);//坐标y
	//接收数据
	CertainRecv(ConnectSocket, &type, sizeof(type));//标志
	if(type==success)
		OperateList.Recv(ConnectSocket);//操作链表
	else if(type==fail)
		TheBlock.Recv(ConnectSocket);//恢复分块

	//断开连接
	closesocket(ConnectSocket);
	return type;
}


//通知插入父分块
PacketType BlockAddParentConnect(BlockNode &TheBlock, ListTemplet<class OperateNode> &OperateList, 
								 int x, int y)
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = blockaddparent;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, g_UserId);//用户名
	send(ConnectSocket, (char *)&x, sizeof(x), 0);//坐标x
	send(ConnectSocket, (char *)&y, sizeof(y), 0);//坐标y
	//接收数据
	CertainRecv(ConnectSocket, &type, sizeof(type));//标志
	if(type==success)
		OperateList.Recv(ConnectSocket);//操作链表
	else if(type==fail)
		TheBlock.Recv(ConnectSocket);//恢复分块

	//断开连接
	closesocket(ConnectSocket);
	return type;
}


//通知连接到已有分块
PacketType BlockLineToConnect(BlockNode &TheBlock, ListTemplet<class OperateNode> &OperateList, 
								 int Deep, int Local[])
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = blocklineto;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, g_UserId);//用户名
	send(ConnectSocket, (char *)&Deep, sizeof(Deep), 0);//定位个数
	send(ConnectSocket, (char *)Local, sizeof(int)*Deep, 0);//定位数组
	//接收数据
	CertainRecv(ConnectSocket, &type, sizeof(type));//标志
	if(type==success)
		OperateList.Recv(ConnectSocket);//操作链表
	else if(type==fail)
		TheBlock.Recv(ConnectSocket);//恢复分块

	//断开连接
	closesocket(ConnectSocket);
	return type;
}


//通知分离为根分块
PacketType BlockSeparateRootConnect(BlockNode &TheBlock, ListTemplet<class OperateNode> &OperateList)
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = blockseparateroot;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, g_UserId);//用户名
	//接收数据
	CertainRecv(ConnectSocket, &type, sizeof(type));//标志
	if(type==success)
		OperateList.Recv(ConnectSocket);//操作链表
	else if(type==fail)
		TheBlock.Recv(ConnectSocket);//恢复分块

	//断开连接
	closesocket(ConnectSocket);
	return type;
}


//通知分离为根分块
PacketType BlockDelSeparateConnect(BlockNode &TheBlock, ListTemplet<class OperateNode> &OperateList)
{
	//连接服务器
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ConnectSocket, (SOCKADDR*)&g_ServerSocket, sizeof(g_ServerSocket))==SOCKET_ERROR)//建立连接：我的socket句柄，对方socket数据
		return error;

	//发送数据
	PacketType type;
	type = blockdelseparate;
	send(ConnectSocket, (char *)&type, sizeof(type), 0);//类型
	SendCString(ConnectSocket, g_UserId);//用户名
	//接收数据
	CertainRecv(ConnectSocket, &type, sizeof(type));//标志
	if(type==success)
		OperateList.Recv(ConnectSocket);//操作链表
	else if(type==fail)
		TheBlock.Recv(ConnectSocket);//恢复分块

	//断开连接
	closesocket(ConnectSocket);
	return type;
}


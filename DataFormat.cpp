
// 数据格式
//

#include "stdafx.h"
#include "OurMindClient.h"
#include "OurMindClientDlg.h"


CString g_UserId;//保存本地用户名


/****************************************************************/
/*************************服务器同步开始*************************/
/****************************************************************/

//整形全局缓存变量
int g_IntBuf[10000];


//ListTemplet方法
template <typename TYPE>
void ListTemplet<TYPE>::Add()
{
	TYPE *p = pnode->pt;
	pnode->pt = new TYPE;
	pnode->pt->qt = pnode;
	pnode->pt->pt = p;
	if(p)
		p->qt = pnode->pt;
	LIST_GO(pnode, pt);
	if(ptail->pt)
		LIST_GO(ptail, pt);
	num ++;
}
template <typename TYPE>
void ListTemplet<TYPE>::Del()
{
	TYPE *p = pnode->pt;
	if(ptail->qt==pnode)
		LIST_GO(ptail, qt);
	pnode->pt = pnode->pt->pt;
	if(pnode->pt)
		pnode->pt->qt = pnode;
	delete p;
	num --;
}
template <typename TYPE>
TYPE *ListTemplet<TYPE>::Cut()
{
	TYPE *p = pnode->pt;
	if(ptail->qt==pnode)
		LIST_GO(ptail, qt);
	pnode->pt = pnode->pt->pt;
	if(pnode->pt)
		pnode->pt->qt = pnode;
	num --;
	return p;
}
template <typename TYPE>
void ListTemplet<TYPE>::Paste(TYPE *p)
{
	p->pt = pnode->pt;
	if(p->pt)
		p->pt->qt = p;
	pnode->pt = p;
	p->qt = pnode;
	pnode = p;
	if(ptail->pt)
		LIST_GO(ptail, pt);
	num ++;
}
template <typename TYPE>
void ListTemplet<TYPE>::Clear()
{
	pnode = ptail;
	while(pnode->qt)
	{
		LIST_GO(pnode, qt);
		delete pnode->pt;
	}
	qnode->pt = 0;
	ptail = qnode;
	num = 0;
}
template <typename TYPE>
void ListTemplet<TYPE>::ReadFile(FILE *fp)
{
	int i;
	int temp;
	fread(&temp, sizeof(num), 1, fp);
	pnode = qnode;
	for(i=0; i<temp; i++)
	{
		Add();
		pnode->ReadFile(fp);
	}
}
template <typename TYPE>
void ListTemplet<TYPE>::WriteFile(FILE *fp)
{
	int i;
	int temp = num;
	fwrite(&temp, sizeof(num), 1, fp);
	pnode = qnode;
	for(i=0; i<temp; i++)
	{
		pnode->pt->WriteFile(fp);
		Del();
	}
}
template <typename TYPE>
void ListTemplet<TYPE>::Send(SOCKET sock)
{
	int i;
	send(sock, (char *)&num, sizeof(num), 0);
	pnode=qnode;
	for(i=0; i<num; i++)
	{
		LIST_GO(pnode, pt);
		pnode->Send(sock);
	}
}
template <typename TYPE>
void ListTemplet<TYPE>::Recv(SOCKET sock)
{
	pnode = qnode;
	while(qnode->pt)
		Del();
	int i;
	int temp;
	CertainRecv(sock, &temp, sizeof(int));
	for(i=0; i<temp; i++)
	{
		Add();
		pnode->Recv(sock);
	}
}


//TreeTemplet方法
template <typename TYPE>
void TreeTemplet<TYPE>::Add()
{
	TYPE *p = pnode->pch;
	pnode->pch = new TYPE;//新节点作为pnode的第一个子节点
	pnode->pch->ppr = pnode;
	LIST_GO(pnode, pch);//pnode转到新节点
	pnode->pt = p;
	pnode->qt = 0;
	if(p)
		p->qt = pnode;
}
template <typename TYPE>
int TreeTemplet<TYPE>::Del()
{
	int num = 0;
	TYPE *p1, *p2;
	p1 = pnode->pch;
	p2 = pnode->ppr;
	//断开pnode的连接
	if(p2->pch==pnode)
		p2->pch = pnode->pt;
	if(pnode->pt)
		pnode->pt->qt = pnode->qt;
	if(pnode->qt)
		pnode->qt->pt = pnode->pt;
	//删除pnode，pnode转到其父节点
	delete pnode;
	pnode = p2;
	//处理pnode原本子节点
	if(p1)
	{
		num ++;
		//将其插入到父节点最前方
		p2 = pnode->pch;
		pnode->pch = p1;
		p1->ppr = pnode;
		while(p1->pt)
		{
			LIST_GO(p1, pt);
			p1->ppr = pnode;
			num ++;
		}
		p1->pt = p2;
		if(p2)
			p2->qt = p1;
	}
	return num;
}
template <typename TYPE>
TYPE *TreeTemplet<TYPE>::Cut()
{
	TYPE *p = pnode;
	LIST_GO(pnode, ppr);
	//断开p的连接
	if(pnode->pch==p)
		pnode->pch = p->pt;
	if(p->pt)
		p->pt->qt = p->qt;
	if(p->qt)
		p->qt->pt = p->pt;
	//返回p
	return p;
}
template <typename TYPE>
void TreeTemplet<TYPE>::Paste(TYPE *p)
{
	//p作为pnode第一个子节点
	p->pt = pnode->pch;
	pnode->pch = p;
	p->ppr = pnode;
	p->qt = 0;
	if(p->pt)
		p->pt->qt = p;
	pnode = p;
}
template <typename TYPE>
int TreeTemplet<TYPE>::GetLocal(int string[])
{
	int num = 0;
	int i;
	for(num=0; pnode->ppr; num++)
	{
		for(i=0; pnode->qt; i++)
			LIST_GO(pnode, qt);
		string[num] = i;
		LIST_GO(pnode, ppr);
	}
	return num;
}
template <typename TYPE>
bool TreeTemplet<TYPE>::SetLocal(int string[], int num)
{
	pnode = qnode;
	int i;
	for(num--; num>=0; num--)
	{
		if(pnode->pch==0)
			return false;
		LIST_GO(pnode, pch);
		for(i=0; i<string[num]; i++)
		{
			if(pnode->pt==0)
				return false;
			LIST_GO(pnode, pt);
		}
	}
	return true;
}
template <typename TYPE>
void TreeTemplet<TYPE>::ReadFile(FILE *fp)
{
	PartReadFile(fp, qnode);
}
template <typename TYPE>
void TreeTemplet<TYPE>::PartReadFile(FILE *fp, TYPE *p)
{
	bool temp;
	fread(&temp, sizeof(temp), 1, fp);
	if(temp==false)
	{
		p->pch = 0;
		return;
	}
	p->pch = new TYPE;
	p->pch->ppr = p;
	LIST_GO(p, pch);
	p->qt = 0;
	PartReadFile(fp, p);
	p->ReadFile(fp);
	fread(&temp, sizeof(temp), 1, fp);
	while(temp)
	{
		p->pt = new TYPE;
		p->pt->qt = p;
		LIST_GO(p, pt);
		p->ppr = p->qt->ppr;
		PartReadFile(fp, p);
		p->ReadFile(fp);
		fread(&temp, sizeof(temp), 1, fp);
	}
	p->pt = 0;
}
template <typename TYPE>
void TreeTemplet<TYPE>::WriteFile(FILE *fp)
{
	PartWriteFile(fp, qnode->pch);
	qnode->pch = 0;
}
template <typename TYPE>
void TreeTemplet<TYPE>::PartWriteFile(FILE *fp, TYPE *p)
{
	bool temp;
	TYPE *p2;
	while(p)
	{
		temp = true;
		fwrite(&temp, sizeof(temp), 1, fp);
		PartWriteFile(fp, p->pch);
		p->WriteFile(fp);
		p2 = p;
		LIST_GO(p, pt);
		delete p2;
	}
	temp = false;
	fwrite(&temp, sizeof(temp), 1, fp);
}
template <typename TYPE>
void TreeTemplet<TYPE>::Send(SOCKET sock)
{
	PartSend(sock, qnode->pch);
}
template <typename TYPE>
void TreeTemplet<TYPE>::PartSend(SOCKET sock, TYPE *p)
{
	bool temp;
	TYPE *p2;
	while(p)
	{
		temp = true;
		send(sock, (char *)&temp, sizeof(temp), 0);
		PartSend(sock, p->pch);
		p->Send(sock);
		p2 = p;
		LIST_GO(p, pt);
	}
	temp = false;
	send(sock, (char *)&temp, sizeof(temp), 0);
}
template <typename TYPE>
void TreeTemplet<TYPE>::Recv(SOCKET sock)
{
	while(qnode->pch)
	{
		pnode = qnode->pch;
		Del();
	}
	PartRecv(sock, qnode);
}
template <typename TYPE>
void TreeTemplet<TYPE>::PartRecv(SOCKET sock, TYPE *p)
{
	bool temp;
	CertainRecv(sock, &temp, sizeof(temp));
	if(temp==false)
	{
		p->pch = 0;
		return;
	}
	p->pch = new TYPE;
	p->pch->ppr = p;
	LIST_GO(p, pch);
	p->qt = 0;
	PartRecv(sock, p);
	p->Recv(sock);
	CertainRecv(sock, &temp, sizeof(temp));
	while(temp)
	{
		p->pt = new TYPE;
		p->pt->qt = p;
		LIST_GO(p, pt);
		p->ppr = p->qt->ppr;
		PartRecv(sock, p);
		p->Recv(sock);
		CertainRecv(sock, &temp, sizeof(temp));
	}
	p->pt = 0;
}


//tUserData方法
tUserData::tUserData()
{
	JoinNum = 0;
	Online = false;
}
void tUserData::ReadFile(FILE *fp)
{
	int i;
	//外存变量
	ReadFileCString(fp, UserId);
	ReadFileCString(fp, Password);
	fread(&JoinNum, sizeof(JoinNum), 1, fp);
	for(i=0; i<JoinNum; i++)
	{
		ReadFileCString(fp, JoinUser[i]);
		ReadFileCString(fp, JoinMind[i]);
	}
}
void tUserData::WriteFile(FILE *fp)
{
	int i;
	//外存变量
	WriteFileCString(fp, UserId);
	WriteFileCString(fp, Password);
	fwrite(&JoinNum, sizeof(JoinNum), 1, fp);
	for(i=0; i<JoinNum; i++)
	{
		WriteFileCString(fp, JoinUser[i]);
		WriteFileCString(fp, JoinMind[i]);
	}
}


//TempletDebug
void MindListDebug()
{
	ListTemplet<MindNode> MindList;
	MindList.Add();
	MindList.Del();
	MindList.Cut();
	MindList.Paste(0);
	MindList.Clear();
	MindList.ReadFile(0);
	MindList.WriteFile(0);
	MindList.Send(0);
	MindList.Recv(0);
}
//MindNode方法
MindNode::MindNode()
{
	Length = DEFAULT_LENGTH;
	Width = DEFAULT_WIDTH;
	Pos_X = 0;
	Pos_X = 0;
	ReadOnly = true;
	ShareNum = 0;
	EditNum = 0;
	EditStatus = false;
}
void MindNode::Copy(const MindNode &Node)
{
	int i;
	//外存变量
	UserId = Node.UserId;
	MindName = Node.MindName;
	Length = Node.Length;
	Width = Node.Width;
	Pos_X = Node.Pos_X;
	Pos_Y = Node.Pos_Y;
	ReadOnly = Node.ReadOnly;
	ShareNum = Node.ShareNum;
	for(i=0; i<ShareNum; i++)
		ShareUser[i] = Node.ShareUser[i];
	EditTime = Node.EditTime;
	//内存变量
	EditNum = Node.EditNum;
	EditStatus = Node.EditStatus;
	memcpy(UserStatus, Node.UserStatus, sizeof(bool)*ShareNum);
}
void MindNode::ReadFile(FILE *fp)
{
	int i;
	//外存变量
	ReadFileCString(fp, UserId);
	ReadFileCString(fp, MindName);
	fread(&Length, sizeof(Length), 1, fp);
	fread(&Width, sizeof(Width), 1, fp);
	fread(&Pos_X, sizeof(Pos_X), 1, fp);
	fread(&Pos_Y, sizeof(Pos_Y), 1, fp);
	fread(&ReadOnly, sizeof(ReadOnly), 1, fp);
	fread(&ShareNum, sizeof(ShareNum), 1, fp);
	for(i=0; i<ShareNum; i++)
		ReadFileCString(fp, ShareUser[i]);
	ReadFileCString(fp, EditTime);
	//内存变量初始化
	EditNum = 0;
	EditStatus = false;
	memset(UserStatus, 0, sizeof(bool)*ShareNum);
}
void MindNode::WriteFile(FILE *fp)
{
	int i;
	//外存变量
	WriteFileCString(fp, UserId);
	WriteFileCString(fp, MindName);
	fwrite(&Length, sizeof(Length), 1, fp);
	fwrite(&Width, sizeof(Width), 1, fp);
	fwrite(&Pos_X, sizeof(Pos_X), 1, fp);
	fwrite(&Pos_Y, sizeof(Pos_Y), 1, fp);
	fwrite(&ReadOnly, sizeof(ReadOnly), 1, fp);
	fwrite(&ShareNum, sizeof(ShareNum), 1, fp);
	for(i=0; i<ShareNum; i++)
		WriteFileCString(fp, ShareUser[i]);
	WriteFileCString(fp, EditTime);
}
void MindNode::Send(SOCKET sock)
{
	int i;
	//外存变量
	SendCString(sock, UserId);
	SendCString(sock, MindName);
	send(sock, (char *)&Length, sizeof(Length), 0);
	send(sock, (char *)&Width, sizeof(Width), 0);
	send(sock, (char *)&Pos_X, sizeof(Pos_X), 0);
	send(sock, (char *)&Pos_Y, sizeof(Pos_Y), 0);
	send(sock, (char *)&ReadOnly, sizeof(ReadOnly), 0);
	send(sock, (char *)&ShareNum, sizeof(ShareNum), 0);
	for(i=0; i<ShareNum; i++)
		SendCString(sock, ShareUser[i]);
	SendCString(sock, EditTime);
	//内存变量初始化
	send(sock, (char *)&EditNum, sizeof(EditNum), 0);
	send(sock, (char *)&EditStatus, sizeof(EditStatus), 0);
	send(sock, (char *)UserStatus, sizeof(bool)*ShareNum, 0);
}
void MindNode::Recv(SOCKET sock)
{
	int i;
	//外存变量
	RecvCString(sock, UserId);
	RecvCString(sock, MindName);
	CertainRecv(sock, &Length, sizeof(Length));
	CertainRecv(sock, &Width, sizeof(Width));
	CertainRecv(sock, &Pos_X, sizeof(Pos_X));
	CertainRecv(sock, &Pos_Y, sizeof(Pos_Y));
	CertainRecv(sock, &ReadOnly, sizeof(ReadOnly));
	CertainRecv(sock, &ShareNum, sizeof(ShareNum));
	for(i=0; i<ShareNum; i++)
		RecvCString(sock, ShareUser[i]);
	RecvCString(sock, EditTime);
	//内存变量初始化
	CertainRecv(sock, &EditNum, sizeof(EditNum));
	CertainRecv(sock, &EditStatus, sizeof(EditStatus));
	CertainRecv(sock, UserStatus, sizeof(bool)*ShareNum);
}


//TempletDebug
void BlockNodeDebug()
{
	TreeTemplet<BlockNode> BlockList;
	BlockList.Add();
	BlockList.Del();
	BlockList.Cut();
	BlockList.Paste(0);
	BlockList.GetLocal(0);
	BlockList.SetLocal(0, 0);
	BlockList.ReadFile(0);
	BlockList.WriteFile(0);
	BlockList.Send(0);
	BlockList.Recv(0);
}
//BlockNode方法
BlockNode::BlockNode()
{
	Shape = rect;
	Dir = dirup;
	Color = RGB(255, 255, 255);
	Length = DEFAULT_BLOCK_LENGTH;
	Width = DEFAULT_BLOCK_WIDTH;
	Lock = false;
}
void BlockNode::Copy(const BlockNode &Node)
{
	PosX = Node.PosX;
	PosY = Node.PosY;
	Shape = Node.Shape;
	Dir = Node.Dir;
	Color = Node.Color;
	Length = Node.Length;
	Width = Node.Width;
	Text = Node.Text;
	Lock = Node.Lock;
	LockUser = Node.LockUser;
}
void BlockNode::ReadFile(FILE *fp)
{
	//外存变量
	fread(&PosX, sizeof(PosX), 1, fp);
	fread(&PosY, sizeof(PosY), 1, fp);
	fread(&Shape, sizeof(Shape), 1, fp);
	fread(&Dir, sizeof(Dir), 1, fp);
	fread(&Color, sizeof(Color), 1, fp);
	fread(&Length, sizeof(Length), 1, fp);
	fread(&Width, sizeof(Width), 1, fp);
	ReadFileCString(fp, Text);
	fread(&Lock, sizeof(Lock), 1, fp);
	//内存变量初始化
	LockUser = L"";
}
void BlockNode::WriteFile(FILE *fp)
{
	//外存变量
	fwrite(&PosX, sizeof(PosX), 1, fp);
	fwrite(&PosY, sizeof(PosY), 1, fp);
	fwrite(&Shape, sizeof(Shape), 1, fp);
	fwrite(&Dir, sizeof(Dir), 1, fp);
	fwrite(&Color, sizeof(Color), 1, fp);
	fwrite(&Length, sizeof(Length), 1, fp);
	fwrite(&Width, sizeof(Width), 1, fp);
	WriteFileCString(fp, Text);
	if(Lock==true && LockUser.GetLength()==0)
		fwrite(&Lock, sizeof(Lock), 1, fp);
	else
	{
		Lock = false;
		fwrite(&Lock, sizeof(Lock), 1, fp);
	}
}
void BlockNode::Send(SOCKET sock)
{
	//外存变量
	send(sock, (char *)&PosX, sizeof(PosX), 0);
	send(sock, (char *)&PosY, sizeof(PosY), 0);
	send(sock, (char *)&Shape, sizeof(Shape), 0);
	send(sock, (char *)&Dir, sizeof(Dir), 0);
	send(sock, (char *)&Color, sizeof(Color), 0);
	send(sock, (char *)&Length, sizeof(Length), 0);
	send(sock, (char *)&Width, sizeof(Width), 0);
	SendCString(sock, Text);
	send(sock, (char *)&Lock, sizeof(Lock), 0);
	//内存变量
	SendCString(sock, LockUser);
}
void BlockNode::Recv(SOCKET sock)
{
	//外存变量
	CertainRecv(sock, &PosX, sizeof(PosX));
	CertainRecv(sock, &PosY, sizeof(PosY));
	CertainRecv(sock, &Shape, sizeof(Shape));
	CertainRecv(sock, &Dir, sizeof(Dir));
	CertainRecv(sock, &Color, sizeof(Color));
	CertainRecv(sock, &Length, sizeof(Length));
	CertainRecv(sock, &Width, sizeof(Width));
	RecvCString(sock, Text);
	CertainRecv(sock, &Lock, sizeof(Lock));
	//内存变量
	RecvCString(sock, LockUser);
}


//TempletDebug
void OperateNodeDebug()
{
	ListTemplet<OperateNode> OperateList;
	OperateList.Add();
	OperateList.Del();
	OperateList.Cut();
	OperateList.Paste(0);
	OperateList.Clear();
	OperateList.Send(0);
	OperateList.Recv(0);
}
//OperateNode方法
void OperateNode::Copy(const OperateNode &Node)
{
	Operate = Node.Operate;
	switch(Operate)
	{
	case updatamind:
		MindData.Copy(Node.MindData);
		break;
	case updatablock:
		BlockData.Copy(Node.BlockData);
		break;
	case updatarelation:
		RelationData.Copy(Node.RelationData);
		break;
	}
}
void OperateNode::Send(SOCKET sock)
{
	send(sock, (char *)&Operate, sizeof(Operate), 0);
	switch(Operate)
	{
	case updatamind:
		MindData.Send(sock);
		break;
	case updatablock:
		BlockData.Send(sock);
		break;
	case updatarelation:
		RelationData.Send(sock);
		break;
	}
}
void OperateNode::Recv(SOCKET sock)
{
	CertainRecv(sock, &Operate, sizeof(Operate));
	switch(Operate)
	{
	case updatamind:
		MindData.Recv(sock);
		break;
	case updatablock:
		BlockData.Recv(sock);
		break;
	case updatarelation:
		RelationData.Recv(sock);
		break;
	}
}


//BlockOperateData方法
void BlockOperateData::Copy(const BlockOperateData &Data)
{
	Deep = Data.Deep;
	memcpy(Local, Data.Local, Deep*sizeof(int));
	Block.Copy(Data.Block);
	SetWith = Data.SetWith;
}
void BlockOperateData::Send(SOCKET sock)
{
	send(sock, (char *)&Deep, sizeof(Deep), 0);
	send(sock, (char *)Local, sizeof(int)*Deep, 0);
	Block.Send(sock);
	send(sock, (char *)&SetWith, sizeof(SetWith), 0);
}
void BlockOperateData::Recv(SOCKET sock)
{
	CertainRecv(sock, &Deep, sizeof(Deep));
	CertainRecv(sock, Local, sizeof(int)*Deep);
	Block.Recv(sock);
	CertainRecv(sock, &SetWith, sizeof(SetWith));
}


//RelationOperateData方法
void RelationOperateData::Copy(const RelationOperateData &Data)
{
	Relation = Data.Relation;
	Deep1 = Data.Deep1;
	Deep2 = Data.Deep2;
	memcpy(Local1, Data.Local1, Deep1*sizeof(int));
	memcpy(Local2, Data.Local2, Deep2*sizeof(int));
	Message = Data.Message;
}
void RelationOperateData::Send(SOCKET sock)
{
	send(sock, (char *)&Relation, sizeof(Relation), 0);
	send(sock, (char *)&Deep1, sizeof(Deep1), 0);
	send(sock, (char *)&Deep2, sizeof(Deep2), 0);
	send(sock, (char *)Local1, sizeof(int)*Deep1, 0);
	send(sock, (char *)Local2, sizeof(int)*Deep2, 0);
	send(sock, (char *)&Message, sizeof(Message), 0);
}
void RelationOperateData::Recv(SOCKET sock)
{
	CertainRecv(sock, &Relation, sizeof(Relation));
	CertainRecv(sock, &Deep1, sizeof(Deep1));
	CertainRecv(sock, &Deep2, sizeof(Deep2));
	CertainRecv(sock, Local1, sizeof(int)*Deep1);
	CertainRecv(sock, Local2, sizeof(int)*Deep2);
	CertainRecv(sock, &Message, sizeof(Message));
}


//读文件导入CString
void ReadFileCString(FILE* fp, CString &string)
{
	int len;
	static wchar_t buf[100000] = {0};
	fread(&len, sizeof(int), 1, fp);
	fread(buf, sizeof(wchar_t), len, fp);
	string = buf;
}


//CString数据写入文件
void WriteFileCString(FILE* fp, CString &string)
{
	int len = string.GetLength()+1;
	fwrite(&len, sizeof(int), 1, fp);
	fwrite(string, sizeof(wchar_t), len, fp);
}


//二分查找
//CString
bool BinarySearch(const CString string[], const CString &target, int start, int end, int &result)
{
	int mid;
	while(start<=end)
	{
		mid = (start+end)/2;
		if(string[mid]==target)
		{
			result = mid;
			return true;
		}
		else if(string[mid]>target)
			end = mid-1;
		else
			start = mid+1;
	}
	result = end;
	return false;
}
//CString、CString
bool BinarySearch(const CString string[], const CString string2[], const CString &target, 
				  const CString &target2, int start, int end, int &result)
{
	int mid;
	while(start<=end)
	{
		mid = (start+end)/2;
		if(string[mid]==target && string2[mid]==target2)
		{
			result = mid;
			return true;
		}
		else if(string[mid]>target || string[mid]==target && string2[mid]>target2)
			end = mid-1;
		else
			start = mid+1;
	}
	result = end;
	return false;
}
//tUserData
bool BinarySearch(tUserData * const string[], const CString &target, int start, int end, int &result)
{
	int mid;
	while(start<=end)
	{
		mid = (start+end)/2;
		if(string[mid]->UserId==target)
		{
			result = mid;
			return true;
		}
		else if(string[mid]->UserId>target)
			end = mid-1;
		else
			start = mid+1;
	}
	result = end;
	return false;
}


//对思维导图进行操作
bool OperateData(MindNode &TheMind, ListTemplet<class OperateNode> &OperateList)
{
	bool MindChange = false;
	while(OperateList.qnode->pt)
	{
		OperateList.pnode = OperateList.qnode->pt;
		//更新导图信息
		if(OperateList.pnode->Operate==updatamind)
		{
			TheMind.Copy(OperateList.pnode->MindData);
			MindChange = true;
		}
		//更新分块信息
		else if(OperateList.pnode->Operate==updatablock)
		{
			if(!TheMind.BlockTree.SetLocal(OperateList.pnode->BlockData.Local, 
				OperateList.pnode->BlockData.Deep))
				assert(0);
			TheMind.BlockTree.pnode->Copy(OperateList.pnode->BlockData.Block);
			bool IsLock;
			if(TheMind.BlockTree.pnode->Lock && TheMind.BlockTree.pnode->LockUser.GetLength()==0)
				IsLock = true;
			else
				IsLock = false;
			if(OperateList.pnode->BlockData.SetWith)
				LockBlockWith(TheMind.BlockTree.pnode->pch, IsLock);
		}
		//更新分块关系
		else if(OperateList.pnode->Operate==updatarelation)
		{
			TheMind.BlockTree.SetLocal(OperateList.pnode->RelationData.Local1, OperateList.pnode->RelationData.Deep1);
			OperateRelation(TheMind.BlockTree, OperateList.pnode->RelationData.Relation, 
				OperateList.pnode->RelationData.Local2, OperateList.pnode->RelationData.Deep2);
		}
		else
			assert(0);
		OperateList.pnode = OperateList.qnode;
		OperateList.Del();
	}
	return MindChange;
}


//对思维导图进行关系操作
int OperateRelation(TreeTemplet<class BlockNode> &BlockTree, enumRelation Relation, int Local[], int Deep)
{
	int Message = 0;
	BlockNode *pTemp, *pTemp2;
	switch(Relation)
	{
	//添加根
	case addroot:
		BlockTree.pnode = BlockTree.qnode;
		BlockTree.Add();
		break;
	//删除连带子
	case delwith:
		while(BlockTree.pnode->pch)
		{
			LIST_GO(BlockTree.pnode, pch);
			BlockTree.Del();
		}
		BlockTree.Del();
		break;
	//删除合并子
	case delcombine:
		Message = BlockTree.Del();
		break;
	//分块插入子
	case addchild:
		BlockTree.Add();
		break;
	//分块插入父
	case addparent:
		pTemp = BlockTree.Cut();
		BlockTree.Add();
		BlockTree.Paste(pTemp);
		break;
	//链接已有
	case lineto:
		pTemp = BlockTree.pnode;
		BlockTree.SetLocal(Local, Deep);
		pTemp2 = BlockTree.pnode;
		BlockTree.pnode = pTemp;
		BlockTree.Cut();
		BlockTree.pnode = pTemp2;
		BlockTree.Paste(pTemp);
		break;
	//分离为根
	case separateroot:
		pTemp = BlockTree.Cut();
		BlockTree.pnode = BlockTree.qnode;
		BlockTree.Paste(pTemp);
		break;
	//删除分离子
	case delseparate:
		pTemp = BlockTree.Cut();
		BlockTree.pnode = BlockTree.qnode;
		BlockTree.Paste(pTemp);
		Message = BlockTree.Del();
		break;
	default:
		assert(0);
		break;
	}
	return Message;
}


//递归设置分块锁定解锁
void LockBlockWith(BlockNode *pBlock, bool IsLock)
{
	while(pBlock)
	{
		//设置子分块
		LockBlockWith(pBlock->pch, IsLock);

		//设置自身
		if(IsLock && (pBlock->Lock && pBlock->LockUser.GetLength()!=0 || pBlock->Lock==false))
		{
			pBlock->Lock = true;
			pBlock->LockUser = L"";
		}
		else if(IsLock==false && pBlock->Lock && pBlock->LockUser.GetLength()==0)
		{
			pBlock->Lock = false;
		}

		//走向兄弟分块
		LIST_GO(pBlock, pt);
	}
}
/****************************************************************/
/*************************服务器同步结束*************************/
/****************************************************************/

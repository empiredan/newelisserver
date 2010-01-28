#ifndef Queue_h
#define Queue_h

#include <deque>
#include <afxtempl.h>
#include <afxmt.h>
#include "Data.h"
using namespace std;
//目前主流编译器不支持分离编译
//也就是不支持模板声明写到h文件中，实现写到cpp文件中
//C++标准倒是规定有分离编译
//不过对于编译器来说实现比较困难所以大多数编译器不支持(包括vc全系列)
template<class DATA_TYPE>
class CDataQueue
{
public:
	CDataQueue();
	~CDataQueue();
public:
	void enQueue(DATA_TYPE* data);
	DATA_TYPE* deQueue();
public:
	CList<DATA_TYPE*, DATA_TYPE*> dataQueue;
public:
	CMutex dataMtx;
	CEvent dataEvt;

};

template<class DATA_TYPE>
CDataQueue<DATA_TYPE>::CDataQueue():dataMtx(), dataEvt()
{
	
}

template<class DATA_TYPE>
CDataQueue<DATA_TYPE>::~CDataQueue()
{
	
	POSITION p=dataQueue.GetHeadPosition();
	
	while (p!=NULL)
	{
		delete dataQueue.GetNext(p);
	}
}

template<class DATA_TYPE>
void CDataQueue<DATA_TYPE>::enQueue(DATA_TYPE* data)
{
	CSingleLock l(&dataMtx);
	if(l.Lock()) {
		dataQueue.AddTail(data);
		dataEvt.SetEvent();
	}
	l.Unlock();
}

template<class DATA_TYPE>
DATA_TYPE* CDataQueue<DATA_TYPE>::deQueue()
{
	   DATA_TYPE* d;
	   CSingleLock l(&dataMtx);
	   while(dataQueue.IsEmpty()) {
		   //AfxMessageBox(_T("MasterDataQueue  empty, waitting!"));
		   ::WaitForSingleObject(dataEvt.m_hObject, 3000);
	   }
	   if(l.Lock()) {
		   //DATA_TYPE* elem = dataQueue.GetHead();
		   d = dataQueue.RemoveHead();
	   }
	   l.Unlock();
	return d;
}
/////////////////////////////////////////////////////////////////
template<class DATA_TYPE>
class MasterDataQueue:public CDataQueue<DATA_TYPE>
{
public:
	MasterDataQueue();
    ~MasterDataQueue();
	
};

template<class DATA_TYPE>
MasterDataQueue<DATA_TYPE>::MasterDataQueue():CDataQueue<DATA_TYPE>()
{
	
}

template<class DATA_TYPE>
MasterDataQueue<DATA_TYPE>::~MasterDataQueue() {
	
}
/////////////////////////////////////////////////////////////////

template<class DATA_TYPE>
class FrontDataQueue:public CDataQueue<DATA_TYPE>
{
public:
	FrontDataQueue();
	~FrontDataQueue();

};

template<class DATA_TYPE>
FrontDataQueue<DATA_TYPE>::FrontDataQueue():CDataQueue<DATA_TYPE>()
{
	
}

template<class DATA_TYPE>
FrontDataQueue<DATA_TYPE>::~FrontDataQueue() {
	
}

#endif
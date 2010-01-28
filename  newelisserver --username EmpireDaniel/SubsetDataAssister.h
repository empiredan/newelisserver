// SubsetDataAssister.h: interface for the CSubsetDataAssister class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUBSETDATAASSISTER_H__628DED63_614B_44BF_8069_0C2A62485016__INCLUDED_)
#define AFX_SUBSETDATAASSISTER_H__628DED63_614B_44BF_8069_0C2A62485016__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataFileBuf.h"


class CSubsetDataAssister  
{
public:
	CSubsetDataAssister();
	virtual ~CSubsetDataAssister();
public:
	typedef struct {
		UINT logTimerElapse;
		UINT *subsetNumPerReturn;//是一个数组，元素个数为actNum，subsetNumPerReturn[i] = lcm/Ti=lcm/CActTable::pSaList[i].timeInterval;
		UINT *totalSizeOfSubsetsPerReturn;//是一个数组，元素个数为actNum，表示一次反馈时每一项subset的总size
		float *shareOfCommonBuffer; //是一个数组，元素个数为actNum，计算得出的每一项应分配的公共文件缓冲区大小比例。根据这个数组里的值计算每个项应分得的缓冲区空间大小。
	} ASSISTER;
	ASSISTER assist;

	typedef struct{
		ULONG	toolAddr;			/*address of downhole tool or surface inst.*/
		ULONG	subset;				/*index of subset*/
		ULONG	subsetCnt;			/*number of subset contained in this block*/
		int	currentDepth;			/*depth at the last sample point of 0.1m acquisition segment*/
		int	dataSize;				/*size of subset data in byte: 32-bit boundary*/
		int	actSwitch;				/* indicate subset is on or off*/
		int	currentTime;			/*depth at the last sample point*/
	} RtcBLOCK_DATA_HEADER;
	RtcBLOCK_DATA_HEADER *heads;

	CDataFileBuf* dataFileBuf;

	UINT actNum;

	BOOL created;
public:
	void clear();
	void create(UINT actNum);

	void setDataFileBuf(CDataFileBuf* cdfb){
		dataFileBuf = cdfb;
	}

	UINT getRTCBlockDataHeaderSize() {
		return sizeof(RtcBLOCK_DATA_HEADER);
	}
	//returns each header + each subsets
	UINT getTotalActDataSize() {
		UINT size = 0;
		UINT i;
		for(i = 0; i < actNum; i++) {
			size += assist.totalSizeOfSubsetsPerReturn[i];
			size += sizeof(RtcBLOCK_DATA_HEADER);
		}
		size += sizeof(ULONG);//总状态,long类型
		return size;
	}
public:
	void Save(CFile &log);
};

#endif // !defined(AFX_SUBSETDATAASSISTER_H__628DED63_614B_44BF_8069_0C2A62485016__INCLUDED_)

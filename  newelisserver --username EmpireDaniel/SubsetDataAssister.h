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
		UINT *subsetNumPerReturn;//��һ�����飬Ԫ�ظ���ΪactNum��subsetNumPerReturn[i] = lcm/Ti=lcm/CActTable::pSaList[i].timeInterval;
		UINT *totalSizeOfSubsetsPerReturn;//��һ�����飬Ԫ�ظ���ΪactNum����ʾһ�η���ʱÿһ��subset����size
		float *shareOfCommonBuffer; //��һ�����飬Ԫ�ظ���ΪactNum������ó���ÿһ��Ӧ����Ĺ����ļ���������С��������������������ֵ����ÿ����Ӧ�ֵõĻ������ռ��С��
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
		size += sizeof(ULONG);//��״̬,long����
		return size;
	}
public:
	void Save(CFile &log);
};

#endif // !defined(AFX_SUBSETDATAASSISTER_H__628DED63_614B_44BF_8069_0C2A62485016__INCLUDED_)

#ifndef ACTTABLE_H
#define ACTTABLE_H

#include "SubsetDataAssister.h"
#include "Utils.h"

//#pragma once
class CELISTestServerDlg;
class RTCSubset
{
public:
	int	actNo;
	int	switchOn;
	int	actDone;
	int	depthSampleRate;
	int	timeInterval;

	//下面的是RtcSUBSET_CMD
	int	toolAddress;
	int	subsetNo;			//0－f，仪器身份标识
	int	delay;
	int	m2Length;
	int	m5Length;
	int	m7Length;
public:
	void ConvertData() {
		actNo = ntohl(actNo);
		switchOn = ntohl(switchOn);
		actDone = ntohl(actDone);
		depthSampleRate = ntohl(depthSampleRate);
		timeInterval = ntohl(timeInterval);

		toolAddress = ntohl(toolAddress);
		subsetNo = ntohl(subsetNo);
		delay = ntohl(delay);
		m2Length = ntohl(m2Length);
		m5Length = ntohl(m5Length);
		m7Length = ntohl(m7Length);
	}
};

class CActTable
{
public:
	ULONG actNum;
	ULONG nDepthInterruptMode;
	RTCSubset *pSaList;
	//CELISTestServerDlg* m_dlg;
	LONG lcm, gcd;
public:
	CActTable::CActTable(void);
	CActTable::~CActTable(void);
public:
	void MaximumCommonSampleRate(int dpthandtime[]);
public:
	static CActTable* AllocateActTable(ULONG actNum, unsigned char *buf, int len);
	static CActTable* AllocateActTable(unsigned char  *buf, int len);
public:
	void ConvertData() {
		ULONG i;
		for( i = 0; i < actNum; i++) {
			pSaList[i].ConvertData();
		}
	}
	
	UINT32 modeSize(UINT32 word) { 
		UINT32 size, rtn; 
		size = word * 2; 

		if (size % 4) { 
			size = size / 4 + 1; 
		} else {
			size /= 4; 
		}

		rtn = size *4;
		return rtn; 
	} 

	UINT32 subsetSize(UINT i) {
		UINT32 m2Length = pSaList[i].m2Length;
		UINT32 m5Length = pSaList[i].m5Length;
		UINT32 m7Length = pSaList[i].m7Length;
		UINT32 m2,m5,m7,ul,rtn;
		m2 = modeSize(m2Length);
		m5 = modeSize(m5Length);
		m7 = modeSize(m7Length);
		ul = 2*sizeof(ULONG);
		rtn = ul + m2 + m5 + m7;
		return rtn;
	}
	UINT32 subsetSizeNoHeader(UINT i) {
		UINT32 m2Length = pSaList[i].m2Length;
		UINT32 m5Length = pSaList[i].m5Length;
		UINT32 m7Length = pSaList[i].m7Length;
		UINT32 m2,m5,m7,ul,rtn;
		m2 = modeSize(m2Length);
		m5 = modeSize(m5Length);
		m7 = modeSize(m7Length);
		ul = 2*sizeof(ULONG);
		rtn = m2 + m5 + m7;
		return rtn;
	}
public:
	void buildSubsetDataAssister(CSubsetDataAssister *assist, float speed, UINT workState, int measure);
	void reBuildSubsetDataAssister(CSubsetDataAssister *assist, float speed, UINT workState, int measure);
	UINT getLogTimerElapse(CSubsetDataAssister *assist, float speed, UINT workState, int measure);
private:
	void calculateLCMGCD(UINT lcmgcd[]);
public:
	//void setParentDlg(CELISTestServerDlg* dlg);
	void Save(CFile &log);
};


#endif
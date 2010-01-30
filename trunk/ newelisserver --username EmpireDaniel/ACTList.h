// ACTList.h: interface for the CACTList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACTLIST_H__63DC1355_573F_444E_BC93_A9264135062D__INCLUDED_)
#define AFX_ACTLIST_H__63DC1355_573F_444E_BC93_A9264135062D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Data.h"
#include "commands.h"

#define NET_TO_HOST_LONG(rtcSubset) \
	rtcSubset.actNo = ntohl(rtcSubset.actNo); \
	rtcSubset.switchOn = ntohl(rtcSubset.switchOn); \
	rtcSubset.actDone = ntohl(rtcSubset.actDone); \
	rtcSubset.depthSampleRate = ntohl(rtcSubset.depthSampleRate); \
	rtcSubset.timeInterval = ntohl(rtcSubset.timeInterval); \
	rtcSubset.toolAddress = ntohl(rtcSubset.toolAddress); \
	rtcSubset.subsetNo = ntohl(rtcSubset.subsetNo); \
	rtcSubset.delay = ntohl(rtcSubset.delay); \
	rtcSubset.m2Length = ntohl(rtcSubset.m2Length); \
	rtcSubset.m5Length = ntohl(rtcSubset.m5Length); \
	rtcSubset.m7Length = ntohl(rtcSubset.m7Length);

//A can be used as the GREAT COMMON DENOMINATOR
#define GREAT_COMMON_DENOMINATOR(A, B) \  
	while (A != B)				\
	{							\	
		B = A+B;				\
		A = B-A;				\
		B = abs(B-A-A);			\
		if (A == 1 || B == 1)	\
		{						\
			A = 1;				\
			break;				\
		}						\
	}

//#define AXB(A, B) A*B

//A can be used as the GREAT COMMON DENOMINATOR
#define LOWEST_COMMON_MULTIPLE(A, B, AXB) \
	GREAT_COMMON_DENOMINATOR(A, B) \
	A = AXB/A;
/*
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
	
	inline void NToHL() {
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
*/

typedef struct{
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
}RTCSubset;

typedef struct{
	ULONG actNum;
	ULONG nDepthInterruptMode;
	RTCSubset * pSaList = NULL;
}ACTList;

class CACTList  
{
//Constructor and Deconstructor
public:
	CACTList();
	virtual ~CACTList();

//Attribute
private:
	ACTList m_actList;
	int m_commonDepthSampleRate;
	int m_commonTimeInterval;

	int m_timeDeltaOfDepthMode;
	int m_timeDeltaOfTimeMode;
	long m_depthDUDelta;
	//float m_depthDelta_toM;
	//float m_depthDelta_toImp;
	//float m_depthDelta_Consistency;

//Operations
public:
	void Init(BUF_TYPE * bodyBuf);
	inline void SetTimeDeltaOfDepthMode(float speed, int measure){
		speed/= 60;
		if (measure == m_actList.nDepthInterruptMode)
		{
			m_timeDeltaOfDepthMode = 1000/(speed*m_commonDepthSampleRate)+0.5;
		} 
		else
		{
			if (m_actList.nDepthInterruptMode)
			{
				m_timeDeltaOfDepthMode = 1000*METRIC_DU/(speed*IMPERIAL_DU*m_commonDepthSampleRate)+0.5;
			} 
			else
			{
				m_timeDeltaOfDepthMode = 1000*IMPERIAL_DU/(speed*METRIC_DU*m_commonDepthSampleRate)+0.5;
			}
		}
	}
	inline void SetDepthDuDeltaWithDirection(int direction){
		if (!direction)
		{
			m_depthDUDelta*= -1;
		} 
		
	}
	inline int GetTimeDelta(UINT32 workMode){
		if (workMode == RtcSYS_STANDBY_CMD)
		{
			return m_timeDeltaOfTimeMode;
		}
		else if(workMode == RtcSYS_RECSTART_CMD)
		{
			return m_timeDeltaOfDepthMode;
		}
		return 0;
	}
	inline long GetDepthDuDelta(){
		return m_depthDUDelta;
	}
};

#endif // !defined(AFX_ACTLIST_H__63DC1355_573F_444E_BC93_A9264135062D__INCLUDED_)

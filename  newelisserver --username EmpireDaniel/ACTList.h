// ACTList.h: interface for the CACTList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACTLIST_H__63DC1355_573F_444E_BC93_A9264135062D__INCLUDED_)
#define AFX_ACTLIST_H__63DC1355_573F_444E_BC93_A9264135062D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cmath>
#include "Data.h"
#include "commands.h"
#include "WorkMode.h"
/*
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
*/

//A can be used as the GREAT COMMON DENOMINATOR
/*
#define GREAT_COMMON_DENOMINATOR(A, B) while(A != B)	
		{								
			B = A+B;				
			A = B-A;				
			B = abs(B-A-A);			
			if (A == 1 || B == 1)	
			{						
				A = 1;				
				break;				
			}						
		}							

*/
inline static void great_common_denominator(int &a, int b)
{
	while(a != b)	
	{								
		b = a+b;				
		a = b-a;				
		b = abs(b-a-a);			
		if (a == 1 || b == 1)	
		{						
			a = 1;				
			break;				
		}						
	}
	
}

//#define AXB(A, B) A*B
//A can be used as the GREAT COMMON DENOMINATOR
/*
#define LOWEST_COMMON_MULTIPLE(A, B, AXB) \
	GREAT_COMMON_DENOMINATOR(A, B) \
	A = AXB/A;
*/
inline static void lowest_common_multiple(int &a, int b)
{
	int axb = a*b;
	great_common_denominator(a, b);
	a = axb/a;
}

#define RETURNED_M_LEN(M, _M) M = 4*( ( (M = _M*2)%4 ? 1 : 0 )+(M/4) )

typedef struct{
//public:
	int	actNo;
	int	switchOn;
	int	actDone;
	int	depthSampleRate;
	int	timeInterval;
	//RtcSUBSET_CMD
	int	toolAddress;
	int	subsetNo;			//tool identifier as defined by a hexadecimal digit form 0 to F
	int	delay;
	int	m2Length;
	int	m5Length;
	int	m7Length;
}RTCSubset;

inline static void net_to_host_long(RTCSubset rtcSubset)
{
	rtcSubset.actNo = ntohl(rtcSubset.actNo);
	rtcSubset.switchOn = ntohl(rtcSubset.switchOn); 
	rtcSubset.actDone = ntohl(rtcSubset.actDone); 
	rtcSubset.depthSampleRate = ntohl(rtcSubset.depthSampleRate); 
	rtcSubset.timeInterval = ntohl(rtcSubset.timeInterval); 
	rtcSubset.toolAddress = ntohl(rtcSubset.toolAddress); 
	rtcSubset.subsetNo = ntohl(rtcSubset.subsetNo); 
	rtcSubset.delay = ntohl(rtcSubset.delay); 
	rtcSubset.m2Length = ntohl(rtcSubset.m2Length); 
	rtcSubset.m5Length = ntohl(rtcSubset.m5Length); 
	rtcSubset.m7Length = ntohl(rtcSubset.m7Length);
	
}

typedef struct{
//public:
	ULONG actNum;
	ULONG nDepthInterruptMode;
	RTCSubset * pSaList;
}ACTList;
/*
extern ACTList& ACTList::operator = (const ACTList& actList)
{
	this->actNum
}
*/
typedef struct{
//public:
	ULONG	toolAddr;			//address of tool
	ULONG	subset;				//index of subset
	ULONG	subsetCnt;			//number of subset contained in this block
	int	currentDepth;			//depth at the last sample point of 0.1m acquisition segment
	int	dataSize;				//size of subset data in byte: 32-bit boundary
	int	actSwitch;				//indicate subset is on or off
	int	currentTime;			//depth at the last sample point
}RtcBLOCK_DATA_HEADER;

typedef struct{
	//public:
	RtcBLOCK_DATA_HEADER rtcBlockDataHeader;
	long status;
	long time;
	ULONG allSubsetsLenOfOneToolSubset;
	float percentateOfDataFileBuf;
}SubsetData;

class CACTList  
{
//Constructor and Destructor
public:
	CACTList();
	virtual ~CACTList();

//Attribute
private:
	/**
	* The attributes related to the data received from ELIS Client
	**/

	ACTList m_actList;
	int m_commonDepthSampleRate;
	int m_commonTimeInterval;

	int m_timeMSDeltaOfDepthMode;//ms
	int m_timeMSDeltaOfTimeMode;

	long m_depthDUDeltaOfDepthMode;
	long m_depthDUDeltaOfTimeMode;

	UINT32 m_cWorkMmode;
	
	/**-----------------------------------------------------------------**/

	/**
	* The attributes related to the data returned to ELIS Client
	**/
	
	ULONG m_totalReturnedSubsetDataLenOfDepthMode;
	ULONG m_totalReturnedSubsetDataLenOfTimeMode;

	SubsetData * m_subsetOfDepthMode;
	SubsetData * m_subsetOfTimeMode;
	ULONG m_rtcBlockDataHeaderLen;
	ULONG m_allSubsetsLenOfAllToolSubsetOfDepthMode;
	ULONG m_allSubsetsLenOfAllToolSubsetOfTimeMode;

//Operations
public:
	void Init(BUF_TYPE * bodyBuf, ULONG bodyLen);
	inline void SetTimeMSDeltaOfDepthMode(long speedDUPS){
		
		if (speedDUPS)
		{
			if (m_actList.nDepthInterruptMode)
			{
				m_timeMSDeltaOfDepthMode = 1000*METRIC_DU		\
					/(speedDUPS*m_commonDepthSampleRate)+0.5;
			} 
			else
			{
				m_timeMSDeltaOfDepthMode = 1000*IMPERIAL_DU	\
					/(speedDUPS*m_commonDepthSampleRate)+0.5;
			}
		} 
		else
		{
			m_timeMSDeltaOfDepthMode = 0;
		}
		
		
	}
	inline void SetDpethDUDeltaOfTimeMode(long speedDUPS){
		m_depthDUDeltaOfTimeMode = (m_timeMSDeltaOfTimeMode*speedDUPS)/1000;
	}
	inline void SetDepthDuDeltaWithDirection(int direction){
		if (!direction)
		{
			m_depthDUDeltaOfDepthMode*= -1;
		} 
		
	}
	inline int SetOneSubsetLenOfOneToolSubset(ULONG i){
		int m2, m5, m7;
		RETURNED_M_LEN(m2, m_actList.pSaList[i].m2Length);
		RETURNED_M_LEN(m5, m_actList.pSaList[i].m5Length);
		RETURNED_M_LEN(m7, m_actList.pSaList[i].m7Length);
		return 2*sizeof(long)+m2+m5+m7;
	}
	inline long SetStatus(ULONG i){
		return	((m_actList.pSaList[i].m2Length>0 ? RtcM2NVM : 0)	\
		| (m_actList.pSaList[i].m5Length>0 ? RtcM5NVM : 0))	\
		| (m_actList.pSaList[i].m7Length>0 ? RtcM7NVM : 0);	
	}
	inline void SetWorkMode(UINT32 workMode){
		m_cWorkMmode = workMode;
	}
	inline void SetCurrentDepthOfSubsetData(ULONG i, int currentdpeth){
		if (m_cWorkMmode == RtcSYS_STANDBY_CMD)
		{
			m_subsetOfTimeMode[i].rtcBlockDataHeader.currentDepth = currentdpeth;
		}
		else if(m_cWorkMmode == RtcSYS_RECSTART_CMD)
		{
			m_subsetOfDepthMode[i].rtcBlockDataHeader.currentDepth = currentdpeth;
		}
	}
	inline void SetCurrentTimeOfSubsetData(ULONG i, int currenttime){
		if (m_cWorkMmode == RtcSYS_STANDBY_CMD)
		{
			m_subsetOfTimeMode[i].rtcBlockDataHeader.currentTime = currenttime;
		}
		else if(m_cWorkMmode == RtcSYS_RECSTART_CMD)
		{
			m_subsetOfDepthMode[i].rtcBlockDataHeader.currentTime = currenttime;
		}
	}
	inline ULONG GetACTNum(){
		return m_actList.actNum;
	}
	inline int GetTimeMSDelta(){
		if (m_cWorkMmode == RtcSYS_STANDBY_CMD)
		{
			return m_timeMSDeltaOfTimeMode;
		}
		else if(m_cWorkMmode == RtcSYS_RECSTART_CMD)
		{
			return m_timeMSDeltaOfDepthMode;
		}
		return 0;
	}
	inline long GetDepthDuDelta(){
		if(m_cWorkMmode == RtcSYS_STANDBY_CMD){

			return m_depthDUDeltaOfTimeMode;
		}
		else if(m_cWorkMmode == RtcSYS_RECSTART_CMD)
		{
			return m_depthDUDeltaOfDepthMode;
		}
		return 0;
		
	}
	/*----------------------------------------------------------------*/
	inline ULONG GetTotalSubsetDataLen(){
		if (m_cWorkMmode == RtcSYS_STANDBY_CMD)
		{
			return m_totalReturnedSubsetDataLenOfTimeMode;
		}
		else if(m_cWorkMmode == RtcSYS_RECSTART_CMD)
		{
			return m_totalReturnedSubsetDataLenOfDepthMode;
		}
		return 0;
	}
	inline RtcBLOCK_DATA_HEADER GetRtcBlockDataHeader(ULONG i){
		if (m_cWorkMmode == RtcSYS_STANDBY_CMD)
		{
			return m_subsetOfTimeMode[i].rtcBlockDataHeader;
		}
		else if(m_cWorkMmode == RtcSYS_RECSTART_CMD)
		{
			return m_subsetOfDepthMode[i].rtcBlockDataHeader;
		}
		
	}
	inline ULONG GetRtcBlockDataHeaderLen(){
		return m_rtcBlockDataHeaderLen;
	}
	inline ULONG GetAllSubsetsLenOfOneToolSubset(ULONG i){
		if (m_cWorkMmode == RtcSYS_STANDBY_CMD)
		{
			return m_subsetOfTimeMode[i].allSubsetsLenOfOneToolSubset;
		}
		else if(m_cWorkMmode == RtcSYS_RECSTART_CMD)
		{
			return m_subsetOfDepthMode[i].allSubsetsLenOfOneToolSubset;
		}
		return 0;
	}
	inline float GetPercentageOfDataFileBuf(ULONG i){
		if (m_cWorkMmode == RtcSYS_STANDBY_CMD)
		{
			return m_subsetOfTimeMode[i].percentateOfDataFileBuf;
		}
		else if(m_cWorkMmode == RtcSYS_RECSTART_CMD)
		{
			return m_subsetOfDepthMode[i].percentateOfDataFileBuf;
		}
		return 0;
	}
	inline SubsetData * GetSubsetData(){
		if (m_cWorkMmode == RtcSYS_STANDBY_CMD)
		{
			return m_subsetOfTimeMode;
		}
		else if(m_cWorkMmode == RtcSYS_RECSTART_CMD)
		{
			return m_subsetOfDepthMode;
		}
		return (SubsetData *)0;

	}
	inline int GetOneSubsetLen(ULONG i){
		
		return m_subsetOfTimeMode[i].rtcBlockDataHeader.dataSize;
		
	}
};

#endif // !defined(AFX_ACTLIST_H__63DC1355_573F_444E_BC93_A9264135062D__INCLUDED_)

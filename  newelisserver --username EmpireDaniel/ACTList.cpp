// ACTList.cpp: implementation of the CACTList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "elistestserver.h"
#include "ACTList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CACTList::CACTList()
{

}

CACTList::~CACTList()
{
	if (m_actList.pSaList)
	{
		delete []m_actList.pSaList;
		m_actList.pSaList = NULL;
	}
	if (m_subsetOfDepthMode)
	{
		delete []m_subsetOfDepthMode;
		m_subsetOfDepthMode = NULL;
	}
	if (m_subsetOfTimeMode)
	{
		delete []m_subsetOfTimeMode;
		m_subsetOfTimeMode = NULL;
	}
}

void CACTList::Init(BUF_TYPE * bodyBuf, ULONG bodyLen)
{
	/**
	* Resolve the body of received ACT command to 
	* fill the ACT list
	**/
	ULONG * bodyBufHead = (ULONG *)bodyBuf;
	m_actList.actNum = ntohl(bodyBufHead[0]);
	m_actList.nDepthInterruptMode = ntohl(bodyBufHead[1]);

	if (m_actList.pSaList)
	{
		delete []m_actList.pSaList;
		m_actList.pSaList = NULL;
	}
	m_actList.pSaList = new BUF_TYPE[m_actList.actNum];
	memcpy(m_actList.pSaList, bodyBuf+sizeof(ULONG)*2, bodyLen-sizeof(ULONG)*2);
	UINT i;
	for (i = 0; i < m_actList.actNum; i++)
	{
		NET_TO_HOST_LONG(m_actList.pSaList[i])
	}
	/**---------------------------------------------------------**/

	/**
	* For Depth Mode
	* Because the value of speed has not been received from Dialog
	* We can't calculate the value of time delta
	**/
	int m_commonDepthSampleRate = m_actList.pSaList[0].depthSampleRate;
	for (i = 1; i < m_actList.actNum; i++)
	{
		int b = m_actList.pSaList[i].depthSampleRate;
		GREAT_COMMON_DENOMINATOR(m_commonDepthSampleRate, b)
	}
	if (m_actList.nDepthInterruptMode)
	{
		m_depthDUDelta = METRIC_DU/m_commonDepthSampleRate;
	} 
	else
	{
		m_depthDUDelta = IMPERIAL_DU/m_commonDepthSampleRate;
	}
	/**---------------------------------------------------------**/

	/**
	* For Time Mode
	**/
	int m_commonTimeInterval = m_actList.pSaList[0].timeInterval;
	for (i = 1; i < m_actList.actNum; i++)
	{
		int b = m_actList.pSaList[i].timeInterval;
		int axb = m_commonTimeInterval * b;
		LOWEST_COMMON_MULTIPLE(m_commonTimeInterval, b, axb)
	}
	m_timeDeltaOfTimeMode = m_commonTimeInterval;
	/**---------------------------------------------------------**/

	/**
	* For the subset data returned to ELIS CLient 
	**/

	m_rtcBlockDataHeaderLen = sizeof(RtcBLOCK_DATA_HEADER);

	if (m_subsetOfDepthMode)
	{
		delete []m_subsetOfDepthMode;
		m_subsetOfDepthMode = NULL;
	}

	if (m_subsetOfTimeMode)
	{
		delete []m_subsetOfTimeMode;
		m_subsetOfTimeMode = NULL;
	}

	m_subsetOfDepthMode = new SubsetData[m_actList.actNum];
	m_subsetOfTimeMode = new SubsetData[m_actList.actNum];


	/*
	* For Depth Mode
	*/
	m_totalReturnedSubsetDataLenOfDepthMode = 0;

	m_allSubsetsLenOfAllToolSubsetOfDepthMode = 0;
	
	// A head with 2-ULONG-size plus a general state
	// with 1-ULONG-size
	m_totalReturnedSubsetDataLenOfDepthMode+= 3*sizeof(ULONG);

	m_totalReturnedSubsetDataLenOfDepthMode+= m_rtcBlockDataHeaderLen	\
	*m_actList.actNum;

	for (i = 0 ;i < m_actList.actNum; i++)
	{
		m_subsetOfDepthMode[i].rtcBlockDataHeader.toolAddr =	\
		m_actList.pSaList[i].toolAddress;

		m_subsetOfDepthMode[i].rtcBlockDataHeader.subset =		\
		m_actList.pSaList[i].subsetNo;

		m_subsetOfDepthMode[i].rtcBlockDataHeader.subsetCnt =	\ 
		(ULONG)	(m_actList.pSaList[i].depthSampleRate			\
		/m_commonDepthSampleRate);

		m_subsetOfDepthMode[i].rtcBlockDataHeader.currentDepth = 0;

		m_subsetOfDepthMode[i].rtcBlockDataHeader.dataSize =	\
		SetOneSubsetLenOfOneToolSubset(i);

		m_subsetOfDepthMode[i].rtcBlockDataHeader.actSwitch = 1;

		m_subsetOfDepthMode[i].rtcBlockDataHeader.currentTime = 0;
		/*
		* Two variables below will be updated with 
		* the appropriate value afterward.
		* They are currentDepth and currentTime.
		*/

		m_subsetOfDepthMode[i].allSubsetsLenOfOneToolSubset =		\
		(ULONG)m_subsetOfDepthMode[i].rtcBlockDataHeader.dataSize	\
		* m_subsetOfDepthMode[i].rtcBlockDataHeader.subsetCnt;
		
		m_totalReturnedSubsetDataLenOfDepthMode+=								\
		m_subsetOfDepthMode[i].allSubsetsLenOfOneToolSubset;
		
		m_allSubsetsLenOfAllToolSubsetOfDepthMode+=							\
		m_subsetOfDepthMode[i].allSubsetsLenOfOneToolSubset;
	}


	/*
	* For Time Mode
	*/
	m_totalReturnedSubsetDataLenOfTimeMode = 0;

	m_allSubsetsLenOfAllToolSubsetOfTimeMode = 0;

	// A head with 2-ULONG-size plus a general state
	// with 1-ULONG-size
	
	m_totalReturnedSubsetDataLenOfTimeMode+= 3*sizeof(ULONG);
	m_totalReturnedSubsetDataLenOfTimeMode+= m_rtcBlockDataHeaderLen	\
	*m_actList.actNum;

	for (i = 0 ;i < m_actList.actNum; i++)
	{
		m_subsetOfTimeMode[i].rtcBlockDataHeader.toolAddr =		\
		m_actList.pSaList[i].toolAddress;
	
		m_subsetOfTimeMode[i].rtcBlockDataHeader.subset =		\
		m_actList.pSaList[i].subsetNo;

		m_subsetOfTimeMode[i].rtcBlockDataHeader.subsetCnt =	\
		(ULONG)	(m_commonTimeInterval							\
		/ m_actList.pSaList[i].timeInterval);

		m_subsetOfTimeMode[i].rtcBlockDataHeader.currentDepth = 0;

		m_subsetOfTimeMode[i].rtcBlockDataHeader.dataSize =		\
		SetOneSubsetLenOfOneToolSubset(i);

		m_subsetOfTimeMode[i].rtcBlockDataHeader.actSwitch = 1;

		m_subsetOfTimeMode[i].rtcBlockDataHeader.currentTime = 0;
		/*
		* Two variables below will be updated with 
		* the appropriate value afterward.
		* They are currentDepth and currentTime.
		*/

		m_subsetOfTimeMode[i].allSubsetsLenOfOneToolSubset =		\
		(ULONG)m_subsetOfTimeMode[i].rtcBlockDataHeader.dataSize	\
		* m_subsetOfTimeMode[i].rtcBlockDataHeader.subsetCnt;
		
		m_totalReturnedSubsetDataLenOfTimeMode+=								\
		m_subsetOfTimeMode[i].allSubsetsLenOfOneToolSubset;
		
		m_allSubsetsLenOfAllToolSubsetOfTimeMode+=							\
		m_subsetOfTimeMode[i].allSubsetsLenOfOneToolSubset;
	}


	/*
	* For Both Mode
	*/
	for (ULONG i = 0; i < m_actList.actNum; i++)
	{
		m_subsetOfDepthMode[i].percentateOfDataFileBuf =				\					\
		(float)m_subsetOfDepthMode[i].allSubsetsLenOfOneToolSubset		\
		/(float)m_allSubsetsLenOfAllToolSubsetOfDepthMode;
		
		m_subsetOfTimeMode[i].percentateOfDataFileBuf =				\					\
		(float)m_subsetOfTimeMode[i].allSubsetsLenOfOneToolSubset	\
		/(float)m_allSubsetsLenOfAllToolSubsetOfTimeMode;
			
	}

}

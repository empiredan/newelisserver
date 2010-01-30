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
	}
}

void CACTList::Init(BUF_TYPE * bodyBuf, ULONG bodyLen)
{
	ULONG * bodyBufHead = (ULONG *)bodyBuf;
	m_actList.actNum = ntohl(bodyBufHead[0]);
	m_actList.nDepthInterruptMode = ntohl(bodyBufHead[1]);

	if (m_actList.pSaList)
	{
		delete []m_actList.pSaList;
	}
	m_actList.pSaList = new BUF_TYPE[m_actList.actNum];
	memcpy(m_actList.pSaList, bodyBuf+sizeof(ULONG)*2, bodyLen-sizeof(ULONG)*2);
	UINT i;
	for (i = 0; i < m_actList.actNum; i++)
	{
		NET_TO_HOST_LONG(m_actList.pSaList[i])
	}

	//For Depth Mode
	//Because the value of speed has not been received from Dialog
	//We can't calculate the value of time delta
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
	//For Time Mode
	int m_commonTimeInterval = m_actList.pSaList[0].timeInterval;
	for (i = 1; i < m_actList.actNum; i++)
	{
		int b = m_actList.pSaList[i].timeInterval;
		int axb = m_commonTimeInterval * b;
		LOWEST_COMMON_MULTIPLE(m_commonTimeInterval, b, axb)
	}
	m_timeDeltaOfTimeMode = m_commonTimeInterval;

}

// Calib.cpp: implementation of the CCalib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "elistestserver.h"
#include "Calib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalib::CCalib()
{
	m_calibData.blockNo = -1;
}

CCalib::~CCalib()
{
	
}

void CCalib::Init(BUF_TYPE * bodyBuf, ULONG bodyLen, ACTList actList)
{
	memcpy(&m_calibParameter, bodyBuf, bodyLen);
	net_to_host_long(m_calibParameter);

	m_calibData.status = SetStatus();
	m_calibData.time = 10000;

	m_calibData.subsetLen = SetSubsetLen();

	int toolADDR = (m_calibParameter.m_nCalibAcqCmd>>8)&255;
	int	subsetNo = m_calibParameter.m_nCalibAcqCmd&15;
	
	for (ULONG i = 0; i < actList.actNum; i++)
	{
		if (toolADDR == actList.pSaList[i].toolAddress
			&& subsetNo == actList.pSaList[i].subsetNo)
		{
			m_calibData.rtcSubset = actList.pSaList[i];
			m_calibData.blockNo = i;
			break;
		}
	}


}
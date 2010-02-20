// WorkMode.cpp: implementation of the CWorkMode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "elistestserver.h"
#include "WorkMode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorkMode::CWorkMode()
{
	m_totalReturnedWorkModeDataLen = 2*sizeof(ULONG)+2*sizeof(UINT32);
}

CWorkMode::~CWorkMode()
{

}

void CWorkMode::Init(BUF_TYPE * bodyBuf)
{
	UINT32 * bodyBufHead;
	bodyBufHead = (UINT32 *)bodyBuf;

	if (m_workMode != RtcSYS_IDLE_CMD)
	{
		m_oldWorkMode = m_workMode;
	}

	m_workMode = ntohl(bodyBufHead[0]);
	m_direction = ntohl(bodyBufHead[1]);
}

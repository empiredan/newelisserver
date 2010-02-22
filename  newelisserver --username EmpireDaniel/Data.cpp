// Data.cpp: implementation of the CData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "elistestserver.h"
#include "Data.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CData::CData()
{

}

CData::~CData()
{

}

//CMasterData----------------
CMasterData::CMasterData()
:CData() 
{

}

CMasterData::CMasterData(ULONG cmdtype, ULONG totallen, ULONG headlen, BUF_TYPE * bodybuf, ULONG bodylen)
{
	m_cmdType = cmdtype;
	m_totalLen = totallen;
	m_headLen = headlen;
	m_bodyBuf = new BUF_TYPE[bodylen+10];
	memcpy(m_bodyBuf, bodybuf, bodylen);
	m_bodyLen = bodylen;
}

CMasterData::~CMasterData() {
	if (m_bodyBuf)
	{
		delete []m_bodyBuf;
	}
}

//CFrontData----------------
CFrontData::CFrontData()
:CData() 
{

}

CFrontData::CFrontData(ULONG totalLen) 
{
	m_totalBuf = new BUF_TYPE[totalLen+10];
	m_pBuf = m_totalBuf;
	m_totalLen = totalLen;
}

CFrontData::~CFrontData() {
	if (m_totalBuf)
	{
		delete [] m_totalBuf;
	}
}
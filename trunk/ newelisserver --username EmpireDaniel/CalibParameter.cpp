// CalibParameter.cpp: implementation of the CCalibParameter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ELISTestServer.h"
#include "CalibParameter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalibParameter::CCalibParameter(BUF_TYPE *buf, ULONG len)
{
	ASSERT(len == sizeof(CALIB_PARA));
	memcpy(&cpara, buf, len);
	ConvertData();
}

CCalibParameter::~CCalibParameter()
{
	
}
int CCalibParameter::getToolADDR()
{
	int cmd=cpara.m_nCalibAcqCmd;
	cmd=cmd>>8;
	int toolADDR=cmd&255;
	return toolADDR;
}
int CCalibParameter::getSubsetNo()
{
	int cmd=cpara.m_nCalibAcqCmd;
	int subsetNo=cmd&15;
	return subsetNo;
}

UINT CCalibParameter::calulcateSubsetFrontSize() {
	UINT m2 = CUtils::modeSize(cpara.m_nM2Length);
	UINT m5 = CUtils::modeSize(cpara.m_nM2Length);
	UINT m7 = CUtils::modeSize(cpara.m_nM2Length);

	subsetFrontSize = (m2 + m5 + m7 + 2*sizeof(short));

	return subsetFrontSize;
	//2*sizeof(short)是CALIB数据返回时，那个subset前面两个short类型变量：
	//status和time, 跟log的subset不同这两个地方用的不是两个long，而是两个
	//short即，1个long
}
UINT CCalibParameter::calulcateSubsetMasterSize() {
	UINT m2 = CUtils::modeSize(cpara.m_nM2Length);
	UINT m5 = CUtils::modeSize(cpara.m_nM2Length);
	UINT m7 = CUtils::modeSize(cpara.m_nM2Length);

	subsetMasterSize = (m2 + m5 + m7 + 2*sizeof(ULONG));

	return subsetMasterSize;
	//2*sizeof(ULONG)是主控机记录CALIB数据时，那个subset前面两个long类型变量：
	//status和time, 跟calib的反馈的subset不同这两个地方用的是两个long，而不是两个
	//short
}

UINT CCalibParameter::getTotalCalibSubsetDataSize() {
	return calulcateSubsetFrontSize();
}
UINT CCalibParameter::getRawCalibSubsetDataSize() {
	UINT m2 = CUtils::modeSize(cpara.m_nM2Length);
	UINT m5 = CUtils::modeSize(cpara.m_nM2Length);
	UINT m7 = CUtils::modeSize(cpara.m_nM2Length);

	subsetRawSize = (m2 + m5 + m7);

	return subsetRawSize;
}
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
	//2*sizeof(short)��CALIB���ݷ���ʱ���Ǹ�subsetǰ������short���ͱ�����
	//status��time, ��log��subset��ͬ�������ط��õĲ�������long����������
	//short����1��long
}
UINT CCalibParameter::calulcateSubsetMasterSize() {
	UINT m2 = CUtils::modeSize(cpara.m_nM2Length);
	UINT m5 = CUtils::modeSize(cpara.m_nM2Length);
	UINT m7 = CUtils::modeSize(cpara.m_nM2Length);

	subsetMasterSize = (m2 + m5 + m7 + 2*sizeof(ULONG));

	return subsetMasterSize;
	//2*sizeof(ULONG)�����ػ���¼CALIB����ʱ���Ǹ�subsetǰ������long���ͱ�����
	//status��time, ��calib�ķ�����subset��ͬ�������ط��õ�������long������������
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
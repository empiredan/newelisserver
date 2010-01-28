// CalibParameter.h: interface for the CCalibParameter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALIBPARAMETER_H__74E69434_4E47_4DB2_B4EB_EAA103C12C95__INCLUDED_)
#define AFX_CALIBPARAMETER_H__74E69434_4E47_4DB2_B4EB_EAA103C12C95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "commands.h"
#include "Data.h"

#include "SubsetDataAssister.h"
#include "Utils.h"

class CCalibParameter  
{
public:
	typedef struct {
		int		m_nCalibAcqCmd;
		int		m_nCalibDelay;
		int		m_nCalibSendCmd;
		int		m_nM2Length;
		int		m_nM5Length;
		int		m_nM7Length;
	} CALIB_PARA;
	CALIB_PARA cpara;
	UINT subsetFrontSize;
	UINT subsetMasterSize;
	UINT subsetRawSize;
public:
	CCalibParameter(BUF_TYPE *buf, ULONG len);
	virtual ~CCalibParameter();
public:
	void ConvertData() {
		cpara.m_nCalibAcqCmd = ntohl(cpara.m_nCalibAcqCmd);
		cpara.m_nCalibDelay = ntohl(cpara.m_nCalibDelay);
		cpara.m_nCalibSendCmd = ntohl(cpara.m_nCalibSendCmd);
		cpara.m_nM2Length = ntohl(cpara.m_nM2Length);
		cpara.m_nM5Length = ntohl(cpara.m_nM5Length);
		cpara.m_nM7Length = ntohl(cpara.m_nM7Length);
	}

	int getToolADDR();
	int getSubsetNo(); 
	
	UINT calulcateSubsetFrontSize();
	UINT calulcateSubsetMasterSize();
	UINT getTotalCalibSubsetDataSize();
	UINT getRawCalibSubsetDataSize();
};

#endif // !defined(AFX_CALIBPARAMETER_H__74E69434_4E47_4DB2_B4EB_EAA103C12C95__INCLUDED_)

// Calib.h: interface for the CCalib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALIB_H__64BE2AE0_6810_4C7C_ABBE_C8806738B816__INCLUDED_)
#define AFX_CALIB_H__64BE2AE0_6810_4C7C_ABBE_C8806738B816__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "commands.h"
#include "ACTList.h"

typedef struct{
	int	m_nCalibAcqCmd;
	int	m_nCalibDelay;
	int	m_nCalibSendCmd;
	int	m_nM2Length;
	int	m_nM5Length;
	int	m_nM7Length;
}CalibParameter;

#define RETURNED_M_LEN(M, _M) M = 4*( ( (M = _M*2)%4 ? 1 : 0 )+(M/4) )

inline static void net_to_host_long(CalibParameter& calibParameter)
{
	calibParameter.m_nCalibAcqCmd = ntohl(calibParameter.m_nCalibAcqCmd);
	calibParameter.m_nCalibDelay = ntohl(calibParameter.m_nCalibDelay);
	calibParameter.m_nCalibSendCmd = ntohl(calibParameter.m_nCalibSendCmd);
	calibParameter.m_nM2Length = ntohl(calibParameter.m_nM2Length);
	calibParameter.m_nM5Length = ntohl(calibParameter.m_nM5Length);
	calibParameter.m_nM7Length = ntohl(calibParameter.m_nM7Length);
}

typedef struct{
	short status;
	short time;
	int subsetLen;
	//int mLenOfSubset;

	RTCSubset rtcSubset;
	ULONG blockNo;

}CalibData;

class CCalib  
{
//Constructor and deconstructor
public:
	CCalib();
	virtual ~CCalib();

//Attributes
private:
	CalibParameter m_calibParameter;
	CalibData m_calibData;

	ULONG m_totalReturnedCalibDataLen;

//Operations
public:
	void Init(BUF_TYPE * bodyBuf, ULONG bodyLen, ACTList * actList);
	inline ULONG GetBlockNo(){return m_calibData.blockNo;}
	inline CalibData GetCalibData(){return m_calibData;}
	inline ULONG GetTotalCalibDataLen(){return m_totalReturnedCalibDataLen;}
	inline ULONG GetSubsetLen(){return m_calibData.subsetLen;}
	inline int SetSubsetLen(){
		int m2, m5, m7;
		RETURNED_M_LEN(m2, m_calibParameter.m_nM2Length);
		RETURNED_M_LEN(m5, m_calibParameter.m_nM5Length);
		RETURNED_M_LEN(m7, m_calibParameter.m_nM7Length);
		return 2*sizeof(short)+m2+m5+m7;
		
	}
	inline short SetStatus(){
		return	((m_calibParameter.m_nM2Length>0 ? RtcM2NVM : 0)	\
			| (m_calibParameter.m_nM5Length>0 ? RtcM5NVM : 0))	\
			| (m_calibParameter.m_nM7Length>0 ? RtcM7NVM : 0);	
	}
};

#endif // !defined(AFX_CALIB_H__64BE2AE0_6810_4C7C_ABBE_C8806738B816__INCLUDED_)

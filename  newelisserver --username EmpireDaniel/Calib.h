// Calib.h: interface for the CCalib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALIB_H__64BE2AE0_6810_4C7C_ABBE_C8806738B816__INCLUDED_)
#define AFX_CALIB_H__64BE2AE0_6810_4C7C_ABBE_C8806738B816__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct{
	short status;
	short time;
}CalibData;

class CCalib  
{
public:
	CCalib();
	virtual ~CCalib();

};

#endif // !defined(AFX_CALIB_H__64BE2AE0_6810_4C7C_ABBE_C8806738B816__INCLUDED_)

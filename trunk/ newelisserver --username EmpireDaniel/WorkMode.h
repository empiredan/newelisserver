// WorkMode.h: interface for the CWorkMode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKMODE_H__98BCA7D2_5534_4680_8CF0_EECB49ACEF90__INCLUDED_)
#define AFX_WORKMODE_H__98BCA7D2_5534_4680_8CF0_EECB49ACEF90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "commands.h"

class CWorkMode  
{
public:
	CWorkMode();
	virtual ~CWorkMode();
//Attributes
private:
	UINT32 m_oldWorkMode;
	UINT32 m_workMode;
	UINT32 m_direction;
//Operations
public:
	void Init(BUF_TYPE * bodyBuf);
	inline UINT32 GetOldWorkMode(){return m_oldWorkMode;}
	inline UINT32 GetWorkMode(){return m_workMode;}
	inline UINT32 GetDirection(){return m_direction;}
};

#endif // !defined(AFX_WORKMODE_H__98BCA7D2_5534_4680_8CF0_EECB49ACEF90__INCLUDED_)

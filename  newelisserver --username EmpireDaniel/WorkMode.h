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
	int m_direction;

	ULONG m_totalReturnedWorkModeDataLen;
//Operations
public:
	void Init(BUF_TYPE * bodyBuf);
	inline void SetOldWorkMode(UINT32 oldWorkMode){m_oldWorkMode = oldWorkMode;}
	inline void SetWorkMode(UINT32 workMode){m_workMode = workMode;}
	inline UINT32 GetOldWorkMode(){return m_oldWorkMode;}
	inline UINT32 GetWorkMode(){return m_workMode;}
	inline int GetDirection(){return m_direction;}
	inline ULONG GetTotalWorkModeDataLen(){return m_totalReturnedWorkModeDataLen;}
};

#endif // !defined(AFX_WORKMODE_H__98BCA7D2_5534_4680_8CF0_EECB49ACEF90__INCLUDED_)

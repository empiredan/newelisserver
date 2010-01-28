// ELISTestServer.h : main header file for the ELISTESTSERVER application
//

#if !defined(AFX_ELISTESTSERVER_H__35170907_2EF8_48C5_8351_B5A6386B4390__INCLUDED_)
#define AFX_ELISTESTSERVER_H__35170907_2EF8_48C5_8351_B5A6386B4390__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CELISTestServerApp:
// See ELISTestServer.cpp for the implementation of this class
//

class CELISTestServerApp : public CWinApp
{
public:
	CELISTestServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CELISTestServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CELISTestServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELISTESTSERVER_H__35170907_2EF8_48C5_8351_B5A6386B4390__INCLUDED_)

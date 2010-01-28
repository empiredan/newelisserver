#if !defined(AFX_MYTABCTRL_H__F64180F2_F6DE_4FAC_AAF1_FC9754304A65__INCLUDED_)
#define AFX_MYTABCTRL_H__F64180F2_F6DE_4FAC_AAF1_FC9754304A65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyTabCtrl.h : header file
//
#include "TabAct.h"
#include "TabCalVer.h"
/////////////////////////////////////////////////////////////////////////////
// MyTabCtrl window

class MyTabCtrl : public CTabCtrl
{
// Construction
public:
	MyTabCtrl();
	
	//CDialog *m_tabPages[2];
	TabAct* m_dlgAct;
	TabCalVer* m_dlgCalVer;
	//int m_tabCurrent;
	//int m_nNumberOfPages;

// Attributes
public:

// Operations
public:
	void Init();
	void SetRectangle();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MyTabCtrl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~MyTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(MyTabCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTABCTRL_H__F64180F2_F6DE_4FAC_AAF1_FC9754304A65__INCLUDED_)

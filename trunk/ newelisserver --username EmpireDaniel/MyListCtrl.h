#if !defined(AFX_MYLISTCTRL_H__A8529872_EF8B_471F_B006_E2CA1BD861DA__INCLUDED_)
#define AFX_MYLISTCTRL_H__A8529872_EF8B_471F_B006_E2CA1BD861DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MyListCtrl window

class MyListCtrl : public CListCtrl
{
// Construction
public:
	MyListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MyListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	//void OnPaint();
	virtual ~MyListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(MyListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLISTCTRL_H__A8529872_EF8B_471F_B006_E2CA1BD861DA__INCLUDED_)

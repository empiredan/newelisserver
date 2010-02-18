#if !defined(AFX_MYTABCTRL_H__48DEEBB8_C0CD_46CE_A65E_CF7C53238A2B__INCLUDED_)
#define AFX_MYTABCTRL_H__48DEEBB8_C0CD_46CE_A65E_CF7C53238A2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyTabCtrl.h : header file
//
#include "ACTDialog.h"
#include "CALVERDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl window
class CELISTestServerDlg;
class CMyTabCtrl : public CTabCtrl
{
// Construction
public:
	CMyTabCtrl();

// Attributes
public:
	CELISTestServerDlg * m_elisTestServerDlg;
	CACTDialog * m_actDialog;
	CCALVERDialog * m_calverDialog;
// Operations
public:
	inline void SetElisTestServerDlg(CELISTestServerDlg * dlg;){
		m_elisTestServerDlg = dlg;
	}
	inline void Init(){
		m_actDialog->Create(IDD_ACT_DIALOG, this);
		m_calverDialog->Create(IDD_CALVER_DIALOG, this);
		SetRectangle();
	}
	void SetRectangle();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyTabCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTABCTRL_H__48DEEBB8_C0CD_46CE_A65E_CF7C53238A2B__INCLUDED_)

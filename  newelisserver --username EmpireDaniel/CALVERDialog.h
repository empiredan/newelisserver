#if !defined(AFX_CALVERDIALOG_H__9BD43DFD_041C_419F_8FAE_91C89B44888D__INCLUDED_)
#define AFX_CALVERDIALOG_H__9BD43DFD_041C_419F_8FAE_91C89B44888D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CALVERDialog.h : header file
//
#include "Resource.h"
#include "MyListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CCALVERDialog dialog
class CELISTestServerDlg;
class CMyTabCtrl;
class CCALVERDialog : public CDialog
{
// Construction
public:
	CCALVERDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCALVERDialog)
	enum { IDD = IDD_CALVER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	CMyListCtrl m_calverListCtrl;
	CMyTabCtrl * m_myTabCtrl;
	//}}AFX_DATA

//Operations
public:
	inline void SetTabCtrl(CMyTabCtrl * myTabCtrl){
		m_myTabCtrl = myTabCtrl;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCALVERDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCALVERDialog)
	afx_msg void OnDblclkListCalver(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALVERDIALOG_H__9BD43DFD_041C_419F_8FAE_91C89B44888D__INCLUDED_)

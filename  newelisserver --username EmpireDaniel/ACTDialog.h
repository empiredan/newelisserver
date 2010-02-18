#if !defined(AFX_ACTDIALOG_H__C70B253E_5E25_4080_9964_A43F294BB140__INCLUDED_)
#define AFX_ACTDIALOG_H__C70B253E_5E25_4080_9964_A43F294BB140__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ACTDialog.h : header file
//
#include "resource.h"
#include "MyListCtrl.h"
#include "MyTabCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CACTDialog dialog

class CACTDialog : public CDialog
{
// Construction
public:
	CACTDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CACTDialog)
	enum { IDD = IDD_ACT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	CMyListCtrl m_actListCtrl;
	CMyTabCtrl * m_myTabCtrl;
	//}}AFX_DATA

//Operations
public:
	inline void SetTabCtrl(CMyTabCtrl * myTabCtrl){
		m_myTabCtrl = m_myTabCtrl;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACTDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CACTDialog)
	afx_msg void OnDblclkListAct(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTDIALOG_H__C70B253E_5E25_4080_9964_A43F294BB140__INCLUDED_)

// CALVERDialog.cpp : implementation file
//

#include "stdafx.h"
#include "elistestserver.h"
#include "CALVERDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCALVERDialog dialog


CCALVERDialog::CCALVERDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCALVERDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCALVERDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCALVERDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCALVERDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCALVERDialog, CDialog)
	//{{AFX_MSG_MAP(CCALVERDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCALVERDialog message handlers

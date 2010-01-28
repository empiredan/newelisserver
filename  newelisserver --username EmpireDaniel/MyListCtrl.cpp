// MyListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ELISTestServer.h"
#include "MyListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MyListCtrl

MyListCtrl::MyListCtrl()
{
}

MyListCtrl::~MyListCtrl()
{
}


BEGIN_MESSAGE_MAP(MyListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(MyListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MyListCtrl message handlers
/*
void MyListCtrl::OnPaint()
{
	CRect r;
	int n,n0,n1;
	CListCtrl::OnPaint();
	n0=GetTopIndex();
	n1=GetCountPerPage();
	n1+=n0;
	CDC*dc=GetDC();//Don't use the CPaintDC
	for(n=n0;n!=n1;n++)
	{
		if(GetItemState(n,LVIS_SELECTED)!=LVIS_SELECTED) continue;
		GetItemRect(n,&r,LVIR_BOUNDS);
		r.InflateRect(-2,0);
		dc->SelectStockObject(HOLLOW_BRUSH);
		dc->Rectangle(&r);
   }
}
*/
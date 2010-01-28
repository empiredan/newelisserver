// MyTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ELISTestServer.h"
#include "MyTabCtrl.h"

//#include "TabAct.h"
//#include "TabCalVer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MyTabCtrl

MyTabCtrl::MyTabCtrl()
{
	//m_tabPages[0]=new TabAct;
	//m_tabPages[1]=new TabCalVer;
	
	//m_nNumberOfPages=2;
	
	m_dlgAct=new TabAct;
	m_dlgCalVer=new TabCalVer;
	m_dlgAct->setTabCtrl(this);
	m_dlgCalVer->setTabCtrl(this);

}

MyTabCtrl::~MyTabCtrl()
{
	/*for(int nCount=0; nCount < m_nNumberOfPages; nCount++){
		delete m_tabPages[nCount];
	}*/
	delete m_dlgAct;
	delete m_dlgCalVer;

}

void MyTabCtrl::Init()
{
	//m_tabCurrent=0;

	//m_tabPages[0]->Create(IDD_TAB_ACT, this);
	//m_tabPages[1]->Create(IDD_TAB_CALVER, this);
	
	//m_tabPages[0]->ShowWindow(SW_SHOW);
	//m_tabPages[1]->ShowWindow(SW_HIDE);

	m_dlgAct->Create(IDD_TAB_ACT, this);
	m_dlgCalVer->Create(IDD_TAB_CALVER, this);
/*	
	//点击一个ITEM就可使CListCtrl一整行被选择
	this->m_dlgAct->m_listctrlAct.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	//this->m_dlgAct->m_listctrlAct.EnsureVisible(6,FALSE);
	
	
	
	

	//The First Tab 
	LVCOLUMN lvcol;
	lvcol.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcol.fmt=LVCFMT_CENTER;
	lvcol.iSubItem=0;
	lvcol.cx=50;
	lvcol.pszText="ACT #";
	this->m_dlgAct->m_listctrlAct.InsertColumn(0,&lvcol);
	
	lvcol.iSubItem=1;
	lvcol.cx=90;
	lvcol.pszText="Tool Series";
	this->m_dlgAct->m_listctrlAct.InsertColumn(1,&lvcol);
	
	lvcol.iSubItem=2;
	lvcol.cx=80;
	lvcol.pszText="Subset #";
	this->m_dlgAct->m_listctrlAct.InsertColumn(2,&lvcol);
	
	lvcol.iSubItem=3;
	lvcol.cx=120;
	lvcol.pszText="Depth Samplerate";
	this->m_dlgAct->m_listctrlAct.InsertColumn(3,&lvcol);
	
	lvcol.iSubItem=4;
	lvcol.cx=100;
	lvcol.pszText="Time Interval";
	this->m_dlgAct->m_listctrlAct.InsertColumn(4,&lvcol);
	
	lvcol.iSubItem=5;
	lvcol.cx=130;
	lvcol.pszText="Change Mapping Mode";
	this->m_dlgAct->m_listctrlAct.InsertColumn(5,&lvcol);

	lvcol.iSubItem=6;
	lvcol.cx=130;
	lvcol.pszText="Data File";
	this->m_dlgAct->m_listctrlAct.InsertColumn(6,&lvcol);

	this->m_dlgAct->m_listctrlAct.InsertItem(0,"0");
	this->m_dlgAct->m_listctrlAct.SetItemText(0,1,"1");
	this->m_dlgAct->m_listctrlAct.SetItemText(0,2,"2");
	this->m_dlgAct->m_listctrlAct.SetItemText(0,3,"3");
	this->m_dlgAct->m_listctrlAct.SetItemText(0,4,"4");
	this->m_dlgAct->m_listctrlAct.SetItemText(0,5,"5");
	this->m_dlgAct->m_listctrlAct.SetItemText(0,6,"6");
	//this->m_dlgAct->m_listctrlAct.SetItemText(0,1,new CButton);
	
	//The Second Tab
	this->m_dlgCalVer->m_listctrlCalVer.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	//this->m_dlgCalVer->m_listctrlCalVer.EnsureVisible(8,FALSE);
	LVCOLUMN lvcol;
	lvcol.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcol.fmt=LVCFMT_CENTER;
	lvcol.iSubItem=0;
	lvcol.cx=90;
	lvcol.pszText="Series ID";
	this->m_dlgCalVer->m_listctrlCalVer.InsertColumn(0,&lvcol);

	lvcol.iSubItem=1;
	lvcol.cx=75;
	lvcol.pszText="CV Name";
	this->m_dlgCalVer->m_listctrlCalVer.InsertColumn(1,&lvcol);

	lvcol.iSubItem=2;
	lvcol.cx=110;
	lvcol.pszText="Tool Asset No";
	this->m_dlgCalVer->m_listctrlCalVer.InsertColumn(2,&lvcol);

	lvcol.iSubItem=3;
	lvcol.cx=90;
	lvcol.pszText="CP";
	this->m_dlgCalVer->m_listctrlCalVer.InsertColumn(3,&lvcol);

	lvcol.iSubItem=4;
	lvcol.cx=90;
	lvcol.pszText="VP";
	this->m_dlgCalVer->m_listctrlCalVer.InsertColumn(4,&lvcol);

	lvcol.iSubItem=5;
	lvcol.cx=90;
	lvcol.pszText="VB";
	this->m_dlgCalVer->m_listctrlCalVer.InsertColumn(5,&lvcol);

	lvcol.iSubItem=6;
	lvcol.cx=90;
	lvcol.pszText="VA";
	this->m_dlgCalVer->m_listctrlCalVer.InsertColumn(6,&lvcol);

	lvcol.iSubItem=7;
	lvcol.cx=90;
	lvcol.pszText="Trip No";
	this->m_dlgCalVer->m_listctrlCalVer.InsertColumn(7,&lvcol);

	lvcol.iSubItem=8;
	lvcol.cx=120;
	lvcol.pszText="Data File";
	this->m_dlgCalVer->m_listctrlCalVer.InsertColumn(8,&lvcol);
	
	this->m_dlgCalVer->m_listctrlCalVer.InsertItem(0,"0");
	this->m_dlgCalVer->m_listctrlCalVer.SetItemText(0,1,"1");
	this->m_dlgCalVer->m_listctrlCalVer.SetItemText(0,2,"2");
	this->m_dlgCalVer->m_listctrlCalVer.SetItemText(0,3,"3");
	this->m_dlgCalVer->m_listctrlCalVer.SetItemText(0,4,"4");
	this->m_dlgCalVer->m_listctrlCalVer.SetItemText(0,5,"5");
	this->m_dlgCalVer->m_listctrlCalVer.SetItemText(0,6,"6");
	this->m_dlgCalVer->m_listctrlCalVer.SetItemText(0,7,"7");
	this->m_dlgCalVer->m_listctrlCalVer.SetItemText(0,8,"8");

	m_dlgAct->ShowWindow(SW_SHOW);
	m_dlgCalVer->ShowWindow(SW_HIDE);
*/


	SetRectangle();
}


void MyTabCtrl::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;
	/*
	m_tabPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	for(int nCount=1; nCount < m_nNumberOfPages; nCount++){
		m_tabPages[nCount]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	}
	*/
	m_dlgAct->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	m_dlgCalVer->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	
}


BEGIN_MESSAGE_MAP(MyTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(MyTabCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MyTabCtrl message handlers

void MyTabCtrl::DoDataExchange(CDataExchange* pDX) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CTabCtrl::DoDataExchange(pDX);

	DDX_Control(pDX, IDD_TAB_ACT, *m_dlgAct);
	DDX_Control(pDX, IDD_TAB_CALVER, *m_dlgCalVer);
}

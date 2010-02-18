// ACTDialog.cpp : implementation file
//

#include "stdafx.h"
#include "elistestserver.h"
#include "ACTDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CACTDialog dialog


CACTDialog::CACTDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CACTDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CACTDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CACTDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CACTDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_LIST_ACT, m_actListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CACTDialog, CDialog)
	//{{AFX_MSG_MAP(CACTDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ACT, OnDblclkListAct)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACTDialog message handlers

void CACTDialog::OnDblclkListAct(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CACTDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	SCROLLINFO hScrollInfo;
	GetScrollInfo(SB_HORZ, &hScrollInfo);
	int oPos = hScrollInfo.nPos;
	int min = hScrollInfo.nMin;
	int max = hScrollInfo.nMax;
	int thumbwidth = hScrollInfo.nPage;
	
	switch(nSBCode){
	case SB_THUMBTRACK:
		ScrollWindow(-(nPos-oPos),0);//�����������ƶ�ʱ,dlg������ʵ���������ƶ�.pos��ʾԭʼλ��,nPos��ʾ��λ��
		//RedrawWindow();
		SetScrollPos(SB_HORZ,nPos);
		break;
	case SB_LINELEFT://�����������ƶ�
		if (oPos!=0)
		{
			ScrollWindow(10,0);//dlg���������ƶ�
			SetScrollPos(SB_HORZ,oPos-10);//�����������ƶ�
		}
		break;
	case SB_LINERIGHT://�����������ƶ�
		if (oPos+thumbwidth<=max)
		{
			ScrollWindow(-10,0);//dlg���������ƶ�
			SetScrollPos(SB_HORZ,oPos+10);//�����������ƶ�
		}
		break;
	case SB_PAGELEFT:
		if (oPos>=thumbwidth)
		{
			ScrollWindow(thumbwidth,0);//dlg���������ƶ�
			SetScrollPos(SB_HORZ,oPos-thumbwidth);//�����������ƶ�
		}else{
			ScrollWindow(oPos,0);//dlg���������ƶ�
			SetScrollPos(SB_HORZ,0);//�������Ƶ������
		}
		break;
	case SB_PAGERIGHT:
		if (oPos+thumbwidth<=max-thumbwidth)
		{
			ScrollWindow(-thumbwidth,0);//dlg���������ƶ�
			SetScrollPos(SB_HORZ,oPos+thumbwidth);//�����������ƶ�
		}else{
			ScrollWindow(-(max-(oPos+thumbwidth)),0);//dlg���������ƶ�
			SetScrollPos(SB_HORZ,max-thumbwidth);//�������Ƶ����Ҷ�
		}
		break;
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CACTDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	SCROLLINFO vScrollInfo;
	GetScrollInfo(SB_VERT, &vScrollInfo);
	int oPos = vScrollInfo.nPos;
	int min = vScrollInfo.nMin;
	int max = vScrollInfo.nMax;
	int thumbwidth = vScrollInfo.nPage;
	
	switch(nSBCode){
	case SB_THUMBTRACK:
		ScrollWindow(0,-(nPos-oPos));//�����������ƶ�ʱ,dlg������ʵ���������ƶ�.pos��ʾԭʼλ��,nPos��ʾ��λ��
		//RedrawWindow();
		SetScrollPos(SB_VERT,nPos);
		break;
	case SB_LINEUP://�����������ƶ�
		if (oPos!=0)
		{
			ScrollWindow(0,1);//dlg���������ƶ�
			//RedrawWindow();
			SetScrollPos(SB_VERT,oPos-1);//�����������ƶ�
		}
		break;
	case SB_LINEDOWN://�����������ƶ�
		if (oPos+thumbwidth<=max)
		{
			ScrollWindow(0,-1);//dlg���������ƶ�
			SetScrollPos(SB_VERT,oPos+1);//�����������ƶ�
		}
		break;
	case SB_PAGEUP:
		if (oPos>=thumbwidth)
		{
			ScrollWindow(0,thumbwidth);//dlg���������ƶ�
			SetScrollPos(SB_VERT,oPos-thumbwidth);//�����������ƶ�
		}else{
			ScrollWindow(0,oPos);//dlg���������ƶ�
			SetScrollPos(SB_VERT,0);//�������Ƶ������
		}
		break;
	case SB_PAGEDOWN:
		if (oPos+thumbwidth<=max-thumbwidth)
		{
			ScrollWindow(0,-thumbwidth);//dlg���������ƶ�
			SetScrollPos(SB_VERT,oPos+thumbwidth);//�����������ƶ�
		}else{
			ScrollWindow(0,-(max-(oPos+thumbwidth)));//dlg���������ƶ�
			SetScrollPos(SB_VERT,max-thumbwidth);//�������Ƶ����Ҷ�
		}
		break;
	}
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CACTDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect tabCtrlRect;
	this->m_ptabCtrl->GetClientRect(&tabCtrlRect);
	//CRect dlgRect;
	//GetClientRect(&dlgRect);
	CRect listRect;
	this->m_listctrlAct.GetWindowRect(&listRect);
	SCROLLINFO hScrollInfo;
	hScrollInfo.fMask=SIF_ALL;
	hScrollInfo.nPage=56;
	hScrollInfo.nMax=560-tabCtrlRect.Width()+43;
	hScrollInfo.nMin=0;
	hScrollInfo.nPos=0;
	hScrollInfo.nTrackPos=0;
	hScrollInfo.cbSize=sizeof(hScrollInfo);
	SetScrollInfo(SB_HORZ,&hScrollInfo);
	
	
	
	
	//���һ��ITEM�Ϳ�ʹCListCtrlһ���б�ѡ��
	m_actListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	//this->m_dlgAct->m_listctrlAct.EnsureVisible(6,FALSE);
	
	//The First Tab 
	LVCOLUMN lvcol;
	lvcol.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcol.fmt=LVCFMT_CENTER;
	lvcol.iSubItem=0;
	lvcol.cx=50;
	lvcol.pszText="ACT #";
	m_actListCtrl.InsertColumn(0,&lvcol);
	
	lvcol.iSubItem=1;
	lvcol.cx=90;
	lvcol.pszText="Tool Address";
	m_actListCtrl.InsertColumn(1,&lvcol);
	
	lvcol.iSubItem=2;
	lvcol.cx=80;
	lvcol.pszText="Subset #";
	m_actListCtrl.InsertColumn(2,&lvcol);
	
	lvcol.iSubItem=3;
	lvcol.cx=120;
	lvcol.pszText="Depth Samplerate";
	m_actListCtrl.InsertColumn(3,&lvcol);
	
	lvcol.iSubItem=4;
	lvcol.cx=100;
	lvcol.pszText="Time Interval";
	m_actListCtrl.InsertColumn(4,&lvcol);
	
	
	lvcol.iSubItem=5;
	lvcol.cx=120;
	lvcol.pszText="Data File";
	m_actListCtrl.InsertColumn(5,&lvcol);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

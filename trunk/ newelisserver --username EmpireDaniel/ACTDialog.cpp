// ACTDialog.cpp : implementation file
//

#include "stdafx.h"
#include "elistestserver.h"
#include "ACTDialog.h"

#include "ELISTestServerDlg.h"
#include "MyTabCtrl.h"

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
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACTDialog message handlers

void CACTDialog::OnDblclkListAct(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMLISTVIEW* pNMLISTVIEW=(NMLISTVIEW*)pNMHDR;
	int rowNo=pNMLISTVIEW->iItem;
	int columeNo=pNMLISTVIEW->iSubItem;

	if (m_actListCtrl.GetItemText(rowNo, 0) != "")
	{
		if (columeNo == 5)
		{
			WIN32_FIND_DATA fd;
			CString actDataFileRootPath = m_myTabCtrl->m_elisTestServerDlg->m_actDataFileRootPath;
			HANDLE hFind = FindFirstFile(actDataFileRootPath, &fd);
			if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
				//目录存在
				CFileDialog openActDataFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST, "All Files(*.*)|*.*||", this);
				openActDataFileDlg.m_ofn.lpstrInitialDir = actDataFileRootPath;
				CString strFilePath;
				if (openActDataFileDlg.DoModal()==IDOK)
				{
					strFilePath = openActDataFileDlg.GetPathName();
					m_myTabCtrl->m_elisTestServerDlg->SetDataFilePath(rowNo, strFilePath, m_actListCtrl, 0);
					/*
					if (m_myTabCtrl->m_elisTestServerDlg->SetDataFilePath(rowNo, strFilePath, m_actListCtrl, 0))
					{
						::PostThreadMessage(m_myTabCtrl->m_elisTestServerDlg->m_cmdHandlerThread->m_nThreadID,
						WM_ACT_DATAFILE_PATH, (WPARAM)rowNo, (LPARAM)&strFilePath);
					}*/
					
				}
			}else{
				char t[50];
				sprintf(t, "%s", "此目录已不存在!");
				AfxMessageBox(_T(t));
			}
		}
	}

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
		ScrollWindow(-((int)nPos-oPos),0);//滚动条向右移动时,dlg窗口其实是在向左移动.pos表示原始位置,nPos表示新位置
		//RedrawWindow();
		SetScrollPos(SB_HORZ,(int)nPos);
		break;
	case SB_LINELEFT://滚动条向左移动
		if (oPos!=0)
		{
			ScrollWindow(10,0);//dlg窗口向右移动
			SetScrollPos(SB_HORZ,oPos-10);//滚动条向左移动
		}
		break;
	case SB_LINERIGHT://滚动条向右移动
		if (oPos+thumbwidth<=max)
		{
			ScrollWindow(-10,0);//dlg窗口向左移动
			SetScrollPos(SB_HORZ,oPos+10);//滚动条向右移动
		}
		break;
	case SB_PAGELEFT:
		if (oPos>=thumbwidth)
		{
			ScrollWindow(thumbwidth,0);//dlg窗口向右移动
			SetScrollPos(SB_HORZ,oPos-thumbwidth);//滚动条向左移动
		}else{
			ScrollWindow(oPos,0);//dlg窗口向右移动
			SetScrollPos(SB_HORZ,0);//滚动条移到最左端
		}
		break;
	case SB_PAGERIGHT:
		if (oPos+thumbwidth<=max-thumbwidth)
		{
			ScrollWindow(-thumbwidth,0);//dlg窗口向左移动
			SetScrollPos(SB_HORZ,oPos+thumbwidth);//滚动条向右移动
		}else{
			ScrollWindow(-(max-(oPos+thumbwidth)),0);//dlg窗口向左移动
			SetScrollPos(SB_HORZ,max-thumbwidth);//滚动条移到最右端
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
		ScrollWindow(0,-((int)nPos-oPos));//滚动条向右移动时,dlg窗口其实是在向左移动.pos表示原始位置,nPos表示新位置
		//RedrawWindow();
		SetScrollPos(SB_VERT,(int)nPos);
		break;
	case SB_LINEUP://滚动条向左移动
		if (oPos!=0)
		{
			ScrollWindow(0,1);//dlg窗口向右移动
			//RedrawWindow();
			SetScrollPos(SB_VERT,oPos-1);//滚动条向左移动
		}
		break;
	case SB_LINEDOWN://滚动条向右移动
		if (oPos+thumbwidth<=max)
		{
			ScrollWindow(0,-1);//dlg窗口向左移动
			SetScrollPos(SB_VERT,oPos+1);//滚动条向右移动
		}
		break;
	case SB_PAGEUP:
		if (oPos>=thumbwidth)
		{
			ScrollWindow(0,thumbwidth);//dlg窗口向右移动
			SetScrollPos(SB_VERT,oPos-thumbwidth);//滚动条向左移动
		}else{
			ScrollWindow(0,oPos);//dlg窗口向右移动
			SetScrollPos(SB_VERT,0);//滚动条移到最左端
		}
		break;
	case SB_PAGEDOWN:
		if (oPos+thumbwidth<=max-thumbwidth)
		{
			ScrollWindow(0,-thumbwidth);//dlg窗口向左移动
			SetScrollPos(SB_VERT,oPos+thumbwidth);//滚动条向右移动
		}else{
			ScrollWindow(0,-(max-(oPos+thumbwidth)));//dlg窗口向左移动
			SetScrollPos(SB_VERT,max-thumbwidth);//滚动条移到最右端
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
	m_myTabCtrl->GetClientRect(&tabCtrlRect);
	CRect listRect;
	m_actListCtrl.GetWindowRect(&listRect);
	SCROLLINFO hScrollInfo;
	hScrollInfo.fMask=SIF_ALL;
	hScrollInfo.nPage=56;
	hScrollInfo.nMax=560-tabCtrlRect.Width()+43;
	hScrollInfo.nMin=0;
	hScrollInfo.nPos=0;
	hScrollInfo.nTrackPos=0;
	hScrollInfo.cbSize=sizeof(hScrollInfo);
	SetScrollInfo(SB_HORZ,&hScrollInfo);
	
	//点击一个ITEM就可使CListCtrl一整行被选择
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

void CACTDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

// CALVERDialog.cpp : implementation file
//

#include "stdafx.h"
#include "elistestserver.h"
#include "CALVERDialog.h"

#include "ELISTestServerDlg.h"
#include "MyTabCtrl.h"

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
	DDX_Control(pDX, IDC_LIST_CALVER, m_calverListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCALVERDialog, CDialog)
	//{{AFX_MSG_MAP(CCALVERDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CALVER, OnDblclkListCalver)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCALVERDialog message handlers

void CCALVERDialog::OnDblclkListCalver(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMLISTVIEW* pNMLISTVIEW = (NMLISTVIEW*)pNMHDR;
	int rowNo = pNMLISTVIEW->iItem;
	int columeNo = pNMLISTVIEW->iSubItem;
	
	if (m_calverListCtrl.GetItemText(rowNo, 0) != "")
	{
		if (columeNo == 3)
		{
			WIN32_FIND_DATA fd;
			CString actDataFileRootPath = m_myTabCtrl->m_elisTestServerDlg->m_actDataFileRootPath;
			HANDLE hFind = FindFirstFile(actDataFileRootPath, &fd);
			if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
				//Ŀ¼����
				CFileDialog openActDataFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST, "All Files(*.*)|*.*||", this);
				openActDataFileDlg.m_ofn.lpstrInitialDir = actDataFileRootPath;
				CString strFilePath;
				if (openActDataFileDlg.DoModal()==IDOK)
				{
					strFilePath = openActDataFileDlg.GetPathName();
					m_myTabCtrl->m_elisTestServerDlg->SetDataFilePath(rowNo, strFilePath, m_calverListCtrl, 1);
					m_myTabCtrl->m_elisTestServerDlg->m_cmdHandlerThread->PostThreadMessage(WM_CALVER_DATAFILE_PATH, NULL, (LPARAM)&strFilePath);
					/*
					if (m_myTabCtrl->m_elisTestServerDlg->SetDataFilePath(rowNo, strFilePath, m_calverListCtrl, 1))
					{
						::PostThreadMessage(m_myTabCtrl->m_elisTestServerDlg->m_cmdHandlerThread->m_nThreadID,
							WM_CALVER_DATAFILE_PATH, NULL, (LPARAM)&strFilePath);
					}*/
					
				}
			}else{
				char t[50];
				sprintf(t, "%s", "��Ŀ¼�Ѳ�����!");
				AfxMessageBox(_T(t));
			}
		}
	}

	*pResult = 0;
}

void CCALVERDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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
		ScrollWindow(-((int)nPos-oPos),0);//�����������ƶ�ʱ,dlg������ʵ���������ƶ�.pos��ʾԭʼλ��,nPos��ʾ��λ��
		//RedrawWindow();
		SetScrollPos(SB_HORZ,(int)nPos);
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

void CCALVERDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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
		ScrollWindow(0,-((int)nPos-oPos));//�����������ƶ�ʱ,dlg������ʵ���������ƶ�.pos��ʾԭʼλ��,nPos��ʾ��λ��
		//RedrawWindow();
		SetScrollPos(SB_VERT,(int)nPos);
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

BOOL CCALVERDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect tabCtrlRect;
	m_myTabCtrl->GetClientRect(&tabCtrlRect);
	SCROLLINFO hScrollInfo;
	hScrollInfo.fMask=SIF_ALL;
	hScrollInfo.nPage=34;
	hScrollInfo.nMax=340-tabCtrlRect.Width()+21;//-dlgRect.Width()845
	hScrollInfo.nMin=0;
	hScrollInfo.nPos=0;
	hScrollInfo.nTrackPos=0;
	hScrollInfo.cbSize=sizeof(hScrollInfo);
	SetScrollInfo(SB_HORZ,&hScrollInfo);
	
	//The Second Tab
	m_calverListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	//this->m_dlgCalVer->m_listctrlCalVer.EnsureVisible(8,FALSE);
	LVCOLUMN lvcol;
	lvcol.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcol.fmt=LVCFMT_CENTER;
	lvcol.iSubItem=0;
	lvcol.cx=50;
	lvcol.pszText="ACT #";
	m_calverListCtrl.InsertColumn(0,&lvcol);
	
	lvcol.iSubItem=1;
	lvcol.cx=90;
	lvcol.pszText="Tool Address";
	m_calverListCtrl.InsertColumn(1,&lvcol);
	
	lvcol.iSubItem=2;
	lvcol.cx=80;
	lvcol.pszText="Subset #";
	m_calverListCtrl.InsertColumn(2,&lvcol);
	
	lvcol.iSubItem=3;
	lvcol.cx=120;
	lvcol.pszText="Data File";
	m_calverListCtrl.InsertColumn(8,&lvcol);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCALVERDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

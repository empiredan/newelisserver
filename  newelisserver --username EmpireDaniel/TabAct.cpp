// TabAct.cpp : implementation file
//

#include "stdafx.h"
#include "TabAct.h"
#include "ELISTestServer.h"
#include "ELISTestServerDlg.h"
#include "MyTabCtrl.h"
#include "ActTable.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TabAct dialog


TabAct::TabAct(CWnd* pParent /*=NULL*/)
	: CDialog(TabAct::IDD, pParent)
{
	//{{AFX_DATA_INIT(TabAct)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}

void TabAct::setCElisTestServerDlg(CELISTestServerDlg* dlg)
{
	m_pELISTestServerDlg=dlg;
}

void TabAct::setTabCtrl(MyTabCtrl* tab)
{
	m_ptabCtrl=tab;
}

void TabAct::setDataFilePath(CString rootFolder, UINT i)
{
	if (rootFolder!=""){
		
		//WIN32_FIND_DATA fd;
		//HANDLE hFind = FindFirstFile(actListRootFolder, &fd);
		//if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
		
		CFileFind actDataFileFind;
		BOOL finded=actDataFileFind.FindFile(rootFolder+"\\*.dat");//ת���ַ�!!!	
		BOOL dataFileFinded = finded;
		
		if (finded)
		{
			
			
			while (dataFileFinded)
			{
				dataFileFinded = actDataFileFind.FindNextFile();
				CString actDataFilePath=actDataFileFind.GetFilePath();
				
				
				UINT32 dataFileHeader[3];
				CFile dataFile(actDataFilePath, CFile::modeRead);
				BUF_TYPE dataFileHeaderBuf[sizeof(UINT32)*3];
				dataFile.Read(dataFileHeaderBuf, sizeof(UINT32)*3);
				dataFile.Close();
				
				memcpy(dataFileHeader, dataFileHeaderBuf, sizeof(UINT32)*3);
				UINT32 toolADDR=dataFileHeader[0];
				UINT32 subsetNo=dataFileHeader[1];
				UINT32 dataType=dataFileHeader[2];
				if (toolADDR==atoi(m_listctrlAct.GetItemText(i, 1))
					&& subsetNo==atoi(m_listctrlAct.GetItemText(i, 2))
					&& dataType==0)//�ļ���ʽƥ��
				{
					this->m_listctrlAct.SetItemText(i,5,actDataFilePath);
				}
				
				
				
			}
		}
		
		actDataFileFind.Close();
		
		//}
		}
}

void TabAct::setACTTable(CActTable* acttbl)
{
	this->m_listctrlAct.DeleteAllItems();

	CString actListRootFolder=m_pELISTestServerDlg->m_actListRootFolder;
	

	for (UINT i=0;i<acttbl->actNum;i++)
	{
		char str[50];

		itoa(acttbl->pSaList[i].actNo,str,10);
		this->m_listctrlAct.InsertItem(i,str);

		itoa(acttbl->pSaList[i].toolAddress,str,10);
		this->m_listctrlAct.SetItemText(i,1,str);

		itoa(acttbl->pSaList[i].subsetNo,str,10);
		this->m_listctrlAct.SetItemText(i,2,str);

		itoa(acttbl->pSaList[i].depthSampleRate,str,10);
		CString for_suffix=str;
		if (acttbl->nDepthInterruptMode)
		{
			for_suffix+=" SPM";
			this->m_listctrlAct.SetItemText(i,3,for_suffix);
		}else{
			for_suffix+=" SPF";
			this->m_listctrlAct.SetItemText(i,3,for_suffix);
		}
		
		
		itoa(acttbl->pSaList[i].timeInterval,str,10);
		for_suffix=str;
		for_suffix+=" ms";
		this->m_listctrlAct.SetItemText(i,4,for_suffix);
		//this->m_listctrlAct.SetItemText(i,5,acttbl->pSaList[i]);
		//this->m_listctrlAct.SetItemText(i,6,acttbl->pSaList[i]);

		setDataFilePath(actListRootFolder, i);
		



	}

	
	
}

void TabAct::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TabAct)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_LIST_ACT, m_listctrlAct);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TabAct, CDialog)
	//{{AFX_MSG_MAP(TabAct)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ACT, OnDblclkListAct)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TabAct message handlers
/*
void TabAct::OnItemdblclickListAct(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	NMLISTVIEW* pNMLISTVIEW=(NMLISTVIEW*)pNMHDR;
	int rowNo=pNMLISTVIEW->iItem;
	int columeNo=pNMLISTVIEW->iSubItem;
	AfxMessageBox(_T("Yes!"));
	if (columeNo==6)
	{
		AfxMessageBox(_T("Yes!"));
	}
	*pResult = 0;


}
*/

void TabAct::OnDblclkListAct(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMLISTVIEW* pNMLISTVIEW=(NMLISTVIEW*)pNMHDR;
	int rowNo=pNMLISTVIEW->iItem;
	int columeNo=pNMLISTVIEW->iSubItem;

	/*char t[20];
	sprintf(t, "%d", rowNo);
	AfxMessageBox(_T(t));*/
	//AfxMessageBox(_T("Hello!"));

	if (this->m_listctrlAct.GetItemText(rowNo, 0)!="")
	{
		if (columeNo==5)
		{
			//AfxMessageBox(_T("Yes!"));
			//CELISTestServerDlg* parentDlg=(CELISTestServerDlg*)(this->GetParent());
			m_pELISTestServerDlg->UpdateData(TRUE);
			CString actListRootFolder=m_pELISTestServerDlg->m_actListRootFolder;
			if (actListRootFolder=="")
			{
				char t[50];
				sprintf(t, "%s", "ACT�����ļ�Ŀ¼����Ϊ��!");
				AfxMessageBox(_T(t));
			}else{
				if (this->m_pELISTestServerDlg->m_dataFileBufSize<=0)
				{
					char t[50];
					sprintf(t, "%s", "���ݻ�������С�������0!");
					AfxMessageBox(_T(t));
				}else{

					WIN32_FIND_DATA fd;
					HANDLE hFind = FindFirstFile(actListRootFolder, &fd);
					if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
						//Ŀ¼����
						CFileDialog openActDataFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST, "All Files(*.*)|*.*||", this);
						openActDataFileDlg.m_ofn.lpstrInitialDir=actListRootFolder;
						CString strFilePath;
						if (openActDataFileDlg.DoModal()==IDOK)
						{
							strFilePath=openActDataFileDlg.GetPathName();
							UINT32 dataFileHeader[3];
							CFile dataFile(strFilePath, CFile::modeRead);
							BUF_TYPE dataFileHeaderBuf[sizeof(UINT32)*3];
							dataFile.Read(dataFileHeaderBuf, sizeof(UINT32)*3);
							dataFile.Close();
							//dataFileHeader=(UINT32*)dataFileHeaderBuf;
							memcpy(dataFileHeader, dataFileHeaderBuf, sizeof(UINT32)*3);
							UINT32 toolADDR=dataFileHeader[0];
							UINT32 subsetNo=dataFileHeader[1];
							UINT32 dataType=dataFileHeader[2];
							if (toolADDR==atoi(m_listctrlAct.GetItemText(rowNo, 1))
								&& subsetNo==atoi(m_listctrlAct.GetItemText(rowNo, 2))
								&& dataType==0)//�ļ���ʽƥ��
							{
								if (m_listctrlAct.GetItemText(rowNo, 5)!=strFilePath)
								{
									//this->m_pELISTestServerDlg->m_actDataFilePathChanged=TRUE;
									m_listctrlAct.SetItemText(rowNo, 5, strFilePath);
									m_pELISTestServerDlg->m_subsetAssister->dataFileBuf->fillWithDataFile(rowNo);
								} 
								/*else
								{
									//this->m_pELISTestServerDlg->m_actDataFilePathChanged=FALSE;
									
								}*/
							} 
							else
							{
								char t[50];
								sprintf(t, "%s", "�ļ�ѡ�����,������ѡ��!");
								AfxMessageBox(_T(t));
							}
							
							
							
						}
					}else{
						char t[50];
						sprintf(t, "%s", "��Ŀ¼�Ѳ�����!");
						AfxMessageBox(_T(t));
					}
				}

				
			}
			

		}
	}
	*pResult = 0;
}

void TabAct::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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

void TabAct::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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

void TabAct::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages

	
}


BOOL TabAct::OnInitDialog() 
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
	this->m_listctrlAct.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	//this->m_dlgAct->m_listctrlAct.EnsureVisible(6,FALSE);
	
	//The First Tab 
	LVCOLUMN lvcol;
	lvcol.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcol.fmt=LVCFMT_CENTER;
	lvcol.iSubItem=0;
	lvcol.cx=50;
	lvcol.pszText="ACT #";
	this->m_listctrlAct.InsertColumn(0,&lvcol);
	
	lvcol.iSubItem=1;
	lvcol.cx=90;
	lvcol.pszText="Tool Address";
	this->m_listctrlAct.InsertColumn(1,&lvcol);
	
	lvcol.iSubItem=2;
	lvcol.cx=80;
	lvcol.pszText="Subset #";
	this->m_listctrlAct.InsertColumn(2,&lvcol);
	
	lvcol.iSubItem=3;
	lvcol.cx=120;
	lvcol.pszText="Depth Samplerate";
	this->m_listctrlAct.InsertColumn(3,&lvcol);
	
	lvcol.iSubItem=4;
	lvcol.cx=100;
	lvcol.pszText="Time Interval";
	this->m_listctrlAct.InsertColumn(4,&lvcol);
	
	/*lvcol.iSubItem=5;
	lvcol.cx=130;
	lvcol.pszText="Change Mapping Mode";
	this->m_listctrlAct.InsertColumn(5,&lvcol);
	*/
	lvcol.iSubItem=5;
	lvcol.cx=120;
	lvcol.pszText="Data File";
	this->m_listctrlAct.InsertColumn(5,&lvcol);
/*	
	this->m_listctrlAct.InsertItem(0,"0");
	this->m_listctrlAct.SetItemText(0,1,"1");
	this->m_listctrlAct.SetItemText(0,2,"2");
	this->m_listctrlAct.SetItemText(0,3,"3");
	this->m_listctrlAct.SetItemText(0,4,"4");
	this->m_listctrlAct.SetItemText(0,5,"5");
	this->m_listctrlAct.SetItemText(0,6,"6");
*/
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


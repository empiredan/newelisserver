// TabCalVer.cpp : implementation file
//

#include "stdafx.h"
#include "TabCalVer.h"
#include "ELISTestServer.h"
#include "ELISTestServerDlg.h"

#include "MyTabCtrl.h"
#include "CalibParameter.h"
#include "ActTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TabCalVer dialog


TabCalVer::TabCalVer(CWnd* pParent /*=NULL*/)
	: CDialog(TabCalVer::IDD, pParent)
{
	//{{AFX_DATA_INIT(TabCalVer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}

void TabCalVer::setCElisTestServerDlg(CELISTestServerDlg* dlg)
{
	m_pELISTestServerDlg=dlg;
}

void TabCalVer::setTabCtrl(MyTabCtrl* tab)
{
	m_ptabCtrl=tab;
}

void TabCalVer::setDataFilePath(CString rootFolder, UINT i)
{
	if (rootFolder!=""){
		
		CFileFind calverDataFileFind;
		BOOL finded = calverDataFileFind.FindFile(rootFolder+"\\*.dat");//转义字符!!!	
		BOOL dataFileFinded = finded;
		if (finded)
		{
			
			
			while (dataFileFinded)
			{
				dataFileFinded = calverDataFileFind.FindNextFile();
				CString calverDataFilePath=calverDataFileFind.GetFilePath();
				
				
				UINT32 dataFileHeader[3];
				CFile dataFile(calverDataFilePath, CFile::modeRead);
				BUF_TYPE dataFileHeaderBuf[sizeof(UINT32)*3];
				dataFile.Read(dataFileHeaderBuf, sizeof(UINT32)*3);
				dataFile.Close();
				
				memcpy(dataFileHeader, dataFileHeaderBuf, sizeof(UINT32)*3);
				UINT32 toolADDRFromFile=dataFileHeader[0];
				UINT32 subsetNoFromFile=dataFileHeader[1];
				UINT32 dataType=dataFileHeader[2];
				if (toolADDRFromFile==atoi(m_listctrlCalVer.GetItemText(i, 1))
					&& subsetNoFromFile==atoi(m_listctrlCalVer.GetItemText(i, 2))
					&& dataType==1)//文件格式匹配
				{
					this->m_listctrlCalVer.SetItemText(i,3,calverDataFilePath);
				}
				
				
			}
		}
		
		calverDataFileFind.Close();
	}

}

void TabCalVer::SetCalibParameter(CCalibParameter *clibparam, CActTable* acttbl)
{
	this->m_listctrlCalVer.DeleteAllItems();
	
	UINT32 toolADDR=clibparam->getToolADDR();
	UINT32 subsetNo=clibparam->getSubsetNo();
	int actNo;

	for (int i=0;i<acttbl->actNum;i++)
	{
		
		
		if (acttbl->pSaList[i].toolAddress==toolADDR && acttbl->pSaList[i].subsetNo==subsetNo)
		{
			actNo=acttbl->pSaList[i].actNo;
			break;
		}
		
	}

	char str[50];
		
	itoa(actNo,str,10);
	this->m_listctrlCalVer.InsertItem(0,str);
		
	itoa(toolADDR,str,10);
	this->m_listctrlCalVer.SetItemText(0,1,str);
		
	itoa(subsetNo,str,10);
	this->m_listctrlCalVer.SetItemText(0,2,str);
		
	CString calverListRootFolder=m_pELISTestServerDlg->m_calverListRootFolder;
	setDataFilePath(calverListRootFolder, 0);
	
}

void TabCalVer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TabCalVer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_LIST_CALVER, m_listctrlCalVer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TabCalVer, CDialog)
	//{{AFX_MSG_MAP(TabCalVer)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CALVER, OnDblclkListCalver)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TabCalVer message handlers

void TabCalVer::OnDblclkListCalver(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMLISTVIEW* pNMLISTVIEW=(NMLISTVIEW*)pNMHDR;
	int rowNo=pNMLISTVIEW->iItem;
	int columeNo=pNMLISTVIEW->iSubItem;



	if (this->m_listctrlCalVer.GetItemText(rowNo, 0)!="")
	{
	
		if (columeNo==3)
		{
			//AfxMessageBox(_T("Yes!"));
			//CELISTestServerDlg* parentDlg=(CELISTestServerDlg*)(this->GetParent());
			m_pELISTestServerDlg->UpdateData(TRUE);
			CString calverListRootFolder=m_pELISTestServerDlg->m_calverListRootFolder;
			if (calverListRootFolder=="")
			{
				char t[50];
				sprintf(t, "%s", "Cal/Ver数据文件目录不能为空!");
				AfxMessageBox(_T(t));
			}else{
				if (this->m_pELISTestServerDlg->m_dataFileBufSize<=0)
				{
					char t[50];
					sprintf(t, "%s", "数据缓冲区大小必须大于0!");
					AfxMessageBox(_T(t));
				}else{

					WIN32_FIND_DATA fd;
					HANDLE hFind = FindFirstFile(calverListRootFolder, &fd);
					if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
						//目录存在
						CFileDialog openActDataFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST, "All Files(*.*)|*.*||", this);
						openActDataFileDlg.m_ofn.lpstrInitialDir=calverListRootFolder;
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
							if (toolADDR==atoi(m_listctrlCalVer.GetItemText(rowNo, 1))
								&& subsetNo==atoi(m_listctrlCalVer.GetItemText(rowNo, 2))
								&& dataType==1)//文件格式匹配
							{
								if (m_listctrlCalVer.GetItemText(rowNo, 3)!=strFilePath)
								{
									//this->m_pELISTestServerDlg->m_calverDataFilePathChanged=TRUE;
									m_listctrlCalVer.SetItemText(rowNo, 3, strFilePath);
									m_pELISTestServerDlg->m_subsetAssister->dataFileBuf->fillWithDataFile(rowNo, strFilePath);
								} 
								/*
								else
								{
									//this->m_pELISTestServerDlg->m_calverDataFilePathChanged=FALSE;
								}*/
							} 
							else
							{
								char t[50];
								sprintf(t, "%s", "文件选择错误,请重新选择!");
								AfxMessageBox(_T(t));
								}
							
						}
					}else{
						char t[50];
						sprintf(t, "%s", "此目录已不存在!");
						AfxMessageBox(_T(t));
					}
				}
			}
			

		}
	}
	*pResult = 0;
}

void TabCalVer::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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
		ScrollWindow(-(nPos-oPos),0);//滚动条向右移动时,dlg窗口其实是在向左移动.pos表示原始位置,nPos表示新位置
		//RedrawWindow();
		SetScrollPos(SB_HORZ,nPos);
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

void TabCalVer::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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
		ScrollWindow(0,-(nPos-oPos));//滚动条向右移动时,dlg窗口其实是在向左移动.pos表示原始位置,nPos表示新位置
		//RedrawWindow();
		SetScrollPos(SB_VERT,nPos);
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


BOOL TabCalVer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//setwindthis->m_pELISTestServerDlg->m_tabMyTabCtrl
	CRect tabCtrlRect;
	this->m_ptabCtrl->GetClientRect(&tabCtrlRect);
	//CRect dlgRect;
	//GetClientRect(&dlgRect);
	//CRect listRect;
	//this->m_listctrlCalVer.GetClientRect(&listRect);
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
	this->m_listctrlCalVer.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	//this->m_dlgCalVer->m_listctrlCalVer.EnsureVisible(8,FALSE);
	LVCOLUMN lvcol;
	lvcol.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcol.fmt=LVCFMT_CENTER;
	lvcol.iSubItem=0;
	lvcol.cx=50;
	lvcol.pszText="ACT #";
	this->m_listctrlCalVer.InsertColumn(0,&lvcol);
	
	lvcol.iSubItem=1;
	lvcol.cx=90;
	lvcol.pszText="Tool Address";
	this->m_listctrlCalVer.InsertColumn(1,&lvcol);
	
	lvcol.iSubItem=2;
	lvcol.cx=80;
	lvcol.pszText="Subset #";
	this->m_listctrlCalVer.InsertColumn(2,&lvcol);
	
	lvcol.iSubItem=3;
	lvcol.cx=120;
	lvcol.pszText="Data File";
	this->m_listctrlCalVer.InsertColumn(8,&lvcol);
/*	
	this->m_listctrlCalVer.InsertItem(0,"0");
	this->m_listctrlCalVer.SetItemText(0,1,"1");
	this->m_listctrlCalVer.SetItemText(0,2,"2");
	this->m_listctrlCalVer.SetItemText(0,3,"3");
	this->m_listctrlCalVer.SetItemText(0,4,"4");
	this->m_listctrlCalVer.SetItemText(0,5,"5");
	this->m_listctrlCalVer.SetItemText(0,6,"6");
	this->m_listctrlCalVer.SetItemText(0,7,"7");
	this->m_listctrlCalVer.SetItemText(0,8,"8");
*/
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


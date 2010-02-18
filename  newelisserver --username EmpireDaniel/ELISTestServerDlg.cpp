// ELISTestServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ELISTestServer.h"
#include "ELISTestServerDlg.h"
#include "errno.h"
#include "commands.h"
//#include "stdio.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define   BIF_NEWDIALOGSTYLE    0x0040   
#define   BIF_USENEWUI (BIF_NEWDIALOGSTYLE | BIF_EDITBOX)      
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CELISTestServerDlg dialog

CELISTestServerDlg::CELISTestServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CELISTestServerDlg::IDD, pParent),
	log(".\\commandlist.txt", CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone)
{
	//{{AFX_DATA_INIT(CELISTestServerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_serverPort=0;
	
	//m_pmasterDataQueue=new MasterDataQueue<CMasterData>;

	m_actDataFilePath = NULL;

	m_dataFileBufSize=5*1024*1024;

	m_measure = 1;
	
	m_speedPM = 0.0;
	m_speedPMStr.Format(FLOAT_TO_STRING_FORMAT, m_speedPM);
	m_speedDUPM = 0;

	m_depth = 0.0;
	m_depthStr.Format(FLOAT_TO_STRING_FORMAT, m_depth);
	m_depthDU = 0;

	m_timeS = 0.0;
	m_timeSStr.Format(FLOAT_TO_STRING_FORMAT, m_timeS);
	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CELISTestServerDlg::~CELISTestServerDlg()
{
	//delete this->m_pmasterDataQueue;
	//m_pmasterDataQueue=NULL;

	if (m_actDataFilePath)
	{
		delete [] m_actDataFilePath;
	}

	CString dataConfigFileName = "dataconfig.ini";//D:\\vc6\\MyProjects\\elis\\ELISTestServer6.3.2
	char currentDirectoryChar[1024];
	GetCurrentDirectory(1024, currentDirectoryChar);
	CString dataConfigFilePath = currentDirectoryChar;
	dataConfigFilePath+= "\\";
	dataConfigFilePath+= dataConfigFileName;
	
	//_TCHAR confBuf[1024];
	CString confStr;
	
	//WritePrivateProfileString("Parameter Setting", "TrueDepth", m_trueDepthStr, dataConfigFilePath);
	
	
	//WritePrivateProfileString("Parameter Setting", "Speed", m_speedStr, dataConfigFilePath);
	
	confStr.Format("%d", m_measure);
	WritePrivateProfileString("Parameter Setting", "Measure", confStr, dataConfigFilePath);
	
	confStr.Format("%d", m_serverPort);
	WritePrivateProfileString("Net Connection", "Port", confStr, dataConfigFilePath);
	
	WritePrivateProfileString("Data File", "ACTRoot", m_actDataFileRootPath, dataConfigFilePath);
	
	WritePrivateProfileString("Data File", "CALVERRoot", m_calverListRootFolder, dataConfigFilePath);
	
	confStr.Format("%ld", m_dataFileBufSize/(1024*1024));
	WritePrivateProfileString("Data File", "BufSize", confStr, dataConfigFilePath);


	/*
	if(m_pmasterDataQueue){
		delete m_pmasterDataQueue;
		m_pmasterDataQueue=NULL;
		//char* t="m_pmasterDataQueue deconstructed!";
		//AfxMessageBox(_T(t), MB_YESNO, 0);
	}
	*/	
	log.Close();
}

void CELISTestServerDlg::EnableCreateLog(BOOL enableButton)
{
	GetDlgItem(IDC_BUTTON_CREATE_LOG)->EnableWindow(enableButton);
	
}

void CELISTestServerDlg::EnableStopLog(BOOL enableButton)
{
	GetDlgItem(IDC_BUTTON_STOP_LOG)->EnableWindow(enableButton);
	
}

void CELISTestServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CELISTestServerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_MY_TAB, m_myTabCtrl);

	DDX_Text(pDX, IDC_EDIT_SERVER_PORT, m_serverPort);

	DDX_Text(pDX, IDC_EDIT_ACT_FOLDER, m_actDataFileRootPath);
	DDX_Text(pDX, IDC_EDIT_CALVER_FOLDER, m_calverListRootFolder);

	//}}AFX_DATA_MAP
}

inline BOOL CELISTestServerDlg::SetDataFilePath(ULONG i, CMyListCtrl myListCtrl, UINT32 dataFileType)
{
	CFileFind dataFileFind;
	BOOL isFinded = dataFileFind.FindFile(m_actDataFileRootPath+"\\*.dat");
	BOOL isDataFileFinded = isFinded;

	if (isFinded)
	{
		while(isDataFileFinded)
		{
			dataFileFind = dataFileFind.FindNextFile();
			CString dataFilePath = dataFileFind.GetFilePath();
			
			UINT32 dataFileHeader[3];
			CFile dataFile(dataFilePath, CFile::modeRead);
			BUF_TYPE dataFileHeaderBuf[sizeof(UINT32)*3];
			dataFile.Read(dataFileHeaderBuf, sizeof(UINT32)*3);
			dataFile.Close();
			
			memcpy(dataFileHeader, dataFileHeaderBuf, sizeof(UINT32)*3);
			UINT32 toolADDR=dataFileHeader[0];
			UINT32 subsetNo=dataFileHeader[1];
			UINT32 realDataFileType=dataFileHeader[2];
			if (toolADDR == atoi(myListCtrl.GetItemText(i, 1))
				&& subsetNo == atoi(myListCtrl.GetItemText(i, 2))
				&& realDataFileType == dataFileType)
			{
				dataFileType ? (m_calverDataFilePath = dataFilePath) 
					: (m_actDataFilePath[i] = dataFilePath);
				myListCtrl.SetItemText(i, 5, dataFilePath);
				dataFileFind.Close();
				return TRUE;
			}
		}
	}
	dataFileType ? (m_calverDataFilePath = "") 
	: (m_actDataFilePath[i] = "");
	dataFileFind.Close();
	return FALSE;

}
inline BOOL CELISTestServerDlg::SetDataFilePath(ULONG i, CString dataFilePath, CMyListCtrl myListCtrl, UINT32 dataFileType)
{
	UINT32 dataFileHeader[3];
	CFile dataFile(dataFilePath, CFile::modeRead);
	BUF_TYPE dataFileHeaderBuf[sizeof(UINT32)*3];
	dataFile.Read(dataFileHeaderBuf, sizeof(UINT32)*3);
	dataFile.Close();
					
	memcpy(dataFileHeader, dataFileHeaderBuf, sizeof(UINT32)*3);
	UINT32 toolADDR=dataFileHeader[0];
	UINT32 subsetNo=dataFileHeader[1];
	UINT32 dataType=dataFileHeader[2];
	if (toolADDR == atoi(myListCtrl.GetItemText(i, 1))
		&& subsetNo == atoi(myListCtrl.GetItemText(i, 2))
		&& dataType == dataFileType)//�ļ���ʽƥ��
	{
		if (myListCtrl.GetItemText(i, 5) != dataFilePath)
		{
			myListCtrl.SetItemText(i, 5, dataFilePath);
			return TRUE;
		} 
		return FALSE;				
	} 
	else
	{
		char t[50];
		sprintf(t, "%s", "�ļ�ѡ�����,������ѡ��!");
		AfxMessageBox(_T(t));
		return FALSE;
	}
}
inline BOOL CELISTestServerDlg::SetAllDataFilePaths(CMyListCtrl myListCtrl, UINT32 dataFileType)
{
	BOOL findDataFile = FALSE;
	for (ULONG i = 0; i < myListCtrl.GetItemCount(); i++)
	{
		findDataFile|= SetDataFilePath(i, myListCtrl, dataFileType);
	}
	return findDataFile;
}

BEGIN_MESSAGE_MAP(CELISTestServerDlg, CDialog)
	//{{AFX_MSG_MAP(CELISTestServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Ok, OnButtonOk)
	ON_BN_CLICKED(IDC_Cancel, OnButtonCancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_ELISTESTSERVER_TAB, OnSelchangeElistestserverTab)
	ON_BN_CLICKED(IDC_BUTTON_ACT_FOLDER, OnButtonActFolder)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SERVER_PORT, OnButtonServerPort)
	ON_BN_CLICKED(IDC_BUTTON_CALVER_FOLDER, OnButtonCalverFolder)
	ON_BN_CLICKED(IDC_BUTTON_DATA_BUFFER_SIZE, OnButtonDataBufferSize)
	//ON_BN_CLICKED(IDC_BUTTON_SPEED, OnButtonSpeed)
	//ON_BN_CLICKED(IDC_BUTTON_START_LOG, OnButtonStartLog)
	//ON_BN_CLICKED(IDC_BUTTON_TRUE_DEPTH, OnButtonTrueDepth)
	ON_BN_CLICKED(IDC_RADIO_IMPERIAL, OnRadioImperial)
	ON_BN_CLICKED(IDC_RADIO_METRIC, OnRadioMetric)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_LOG, OnButtonCreateLog)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE_LOG, OnButtonPauseLog)
	//ON_MESSAGE
	ON_MESSAGE(WM_WORKMODE, OnWorkModeUpdated)
	ON_MESSAGE(WM_DIRECTION, OnDirectionUpdated)
	ON_MESSAGE(WM_DEPTH, OnDepthUpdated)
	ON_MESSAGE(WM_SPEED, OnSpeedUpdated)
	ON_MESSAGE(WM_TIME, OnTimeUpdated)
	ON_MESSAGE(WM_SERVER_IP_PORT, OnShowServerIPAndPort)
	//ON_MESSAGE(WM_SERVER_IP, OnShowServerIP)
	//ON_MESSAGE(WM_SERVER_PORT, OnShowServerPort)
	ON_MESSAGE(WM_CLIENT_IP_PORT, OnShowClientIPAndPort)
	//ON_MESSAGE(WM_CLIENT_IP, OnShowClientIP)
	//ON_MESSAGE(WM_CLIENT_PORT, OnShowClientPort)
	ON_MESSAGE(WM_ACT_LIST, OnACTListUpdated)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CELISTestServerDlg message handlers
VOID CELISTestServerDlg::OnWorkModeUpdated(WPARAM wParam, LPARAM lParam)
{
	m_workMode = (UINT32)lParam;
	//CString str;
	switch (m_workMode)
	{
	case NET_CMD_NA:
		m_workModeStr = "N\\A";
		break;
	case RtcSYS_DEACTIVE_CMD:
		m_workModeStr = "DEACTIVE";
		break;
	case RtcSYS_IDLE_CMD:
		m_workModeStr = "IDLE";
		break;
	case RtcSYS_STANDBY_CMD:
		m_workModeStr = "STANDBY";
		break;
	case RtcSYS_RECSTART_CMD:
		m_workModeStr = "RECSTART";
		break;
	case RtcSYS_RECSTOP_CMD:
		m_workModeStr = "RECSTOP";
		break;
	case RtcSYS_CALIBSTART_CMD:
		m_workModeStr = "CALIBSTART";
		break;
	default:
		m_workModeStr = "WRONG WORK MODE";
		break;
	}
	GetDlgItem(IDC_STATIC_WORKMODE_VALUE)->SetWindowText(m_workModeStr);
	
}
VOID CELISTestServerDlg::OnDirectionUpdated(WPARAM wParam, LPARAM lParam)
{
	UINT32 direction = (UINT32)lParam;

	CString str;
	str = (direction ? "DOWN" : "UP");
	GetDlgItem(IDC_STATIC_DIRECTION_VALUE)->SetWindowText(str);

}
VOID CELISTestServerDlg::OnDepthUpdated(WPARAM wParam, LPARAM lParam)
{
	m_depthDU = (long)lParam;
	
	//CString str;
	m_depthStr.Format(FLOAT_TO_STRING_FORMAT, (m_measure ?	\
	(m_depth = (float)m_depthDU/METRIC_DU)					\
	: (m_depth = (float)m_depthDU/IMPERIAL_DU) ) 
	);
	GetDlgItem(IDC_STATIC_DEPTH_VALUE)->SetWindowText(m_depthStr);
}
VOID CELISTestServerDlg::OnSpeedUpdated(WPARAM wParam, LPARAM lParam){
	m_speedDUPM = (long)lParam;
	/*
	float speedDUPMFloat;
	if (m_measure)
	{
		speedDUPMFloat = (float)speedDUPM/METRIC_DU;
	} 
	else
	{
		speedDUPMFloat = (float)speedDUPM/IMPERIAL_DU;
	}
	*/
	//CString str;
	m_speedPMStr.Format(FLOAT_TO_STRING_FORMAT, (m_measure ?	\
	(m_speedPM = (float)m_speedDUPM/METRIC_DU)					\
	: (m_speedPM = (float)m_speedDUPM/IMPERIAL_DU) )
	);
	GetDlgItem(IDC_STATIC_SPEED_VALUE)->SetWindowText(m_speedPMStr);
}
VOID CELISTestServerDlg::OnTimeUpdated(WPARAM wParam, LPARAM lParam){
	long timeMS = (long)lParam;
	
	//CString str;
	m_timeSStr.Format(FLOAT_TO_STRING_FORMAT, ( m_timeS = (float)timeMS/1000) );
	GetDlgItem(IDC_STATIC_TIME_VALUE)->SetWindowText(m_timeSStr);
}

VOID CELISTestServerDlg::OnShowServerIPAndPort(WPARAM wParam, LPARAM lParam)
{
	char serverName[256];
	HOSTENT* pServer;
	gethostname(serverName, 128);
	pServer = gethostbyname(serverName);
	m_serverIP = inet_ntoa( *( (in_addr *)pServer->h_addr ) );
	
	CString serverPort;
	serverPort.Format("%d", m_serverPort);
	GetDlgItem(IDC_STATIC_SERVER_IP_PORT_VALUE)->SetWindowText(_T(m_serverIP+":"+serverPort));
}
/*
VOID CELISTestServerDlg::OnShowServerIP(WPARAM wParam, LPARAM lParam)
{
	CString 
}
VOID CELISTestServerDlg::OnShowServerPort(WPARAM wParam, LPARAM lParam)
{

}
*/
VOID CELISTestServerDlg::OnShowClientIPAndPort(WPARAM wParam, LPARAM lParam)
{
	m_clientPort = (UINT)wParam;
	m_clientIP = *((CString *)lParam);
	CString clientPort;
	clientPort.Format("%d", m_clientPort);
	GetDlgItem(IDC_STATIC_CLIENT_IP_PORT_VALUE)->SetWindowText(_T(m_clientIP+":"+clientPort));
}
/*
VOID CELISTestServerDlg::OnShowClientIP(WPARAM wParam, LPARAM lParam)
{

}
VOID CELISTestServerDlg::OnShowClientPort(WPARAM wParam, LPARAM lParam)
{

}
*/
VOID CELISTestServerDlg::OnACTListUpdated(WPARAM wParam, LPARAM lParam)
{
	m_actList = (ACTList *)lParam;
	m_myTabCtrl.m_actDialog->m_actListCtrl.DeleteAllItems();
	m_actNum = m_actList->actNum;
	if (m_actDataFilePath)
	{
		delete [] m_actDataFilePath;
		m_actDataFilePath = NULL;
	}
	m_actDataFilePath = new CString[m_actNum];
	BOOL findDataFile = FALSE;
	for (ULONG i = 0; i <m_actNum; i++)
	{
		char str[256];

		itoa(m_actList->pSaList[i].actNo, str, 10);
		m_myTabCtrl.m_actDialog->m_actListCtrl.InsertItem(i, str);

		itoa(m_actList->pSaList[i].toolAddress, str, 10);
		m_myTabCtrl.m_actDialog->m_actListCtrl.SetItemText(i, 2, str);

		itoa(m_actList->pSaList[i].subsetNo, str, 10);
		m_myTabCtrl.m_actDialog->m_actListCtrl.SetItemText(i, 2, str);

		itoa(m_actList->pSaList[i].depthSampleRate, str, 10);
		CString value_unit = str;
		if (m_actList->nDepthInterruptMode)
		{
			value_unit+= " SPM";
			
		} 
		else
		{
			value_unit+= " SPF";
		}
		m_myTabCtrl.m_actDialog->m_actListCtrl.SetItemText(i, 3, value_unit);

		itoa(m_actList->pSaList[i].timeInterval, str, 10);
		value_unit = str;
		value_unit+= " ms";
		m_myTabCtrl.m_actDialog->m_actListCtrl.SetItemText(i, 4, value_unit);

		findDataFile|= SetDataFilePath(i, m_myTabCtrl.m_actDialog->m_actListCtrl, 0);

	}
	if (findDataFile)
	{
		::PostThreadMessage(m_cmdHandlerThread->m_nThreadID, WM_ALL_ACT_DATAFILE_PATHS, NULL, (LPARAM)m_actDataFilePath);
	} 
	
}

BOOL CELISTestServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	m_myTabCtrl.InsertItem(0, _T("ACT"));
	m_myTabCtrl.InsertItem(1, _T("Cal/Ver"));
	m_myTabCtrl.Init();
	m_myTabCtrl.SetElisTestServerDlg(this);

	//m_myTabCtrl.m_dlgAct->setCElisTestServerDlg(this);
	//m_myTabCtrl.m_dlgCalVer->setCElisTestServerDlg(this);

	CString dataConfigFileName = "dataconfig.ini";//D:\\vc6\\MyProjects\\elis\\ELISTestServer6.3.2
	char currentDirectoryChar[1024];
	GetCurrentDirectory(1024, currentDirectoryChar);
	CString dataConfigFilePath = currentDirectoryChar;
	dataConfigFilePath+= "\\";
	dataConfigFilePath+= dataConfigFileName;

	LPCTSTR lpDefault;
	INT nDefault;
	_TCHAR confBuf[1024];

	m_measure = GetPrivateProfileInt("Parameter Setting", "Measure", nDefault, dataConfigFilePath);

	m_serverPort = GetPrivateProfileInt("Net Connection", "Port", nDefault, dataConfigFilePath);

	GetPrivateProfileString("Data File", "ACTRoot", lpDefault, confBuf, 1024, dataConfigFilePath);
	m_actDataFileRootPath = confBuf;

	GetPrivateProfileString("Data File", "CALVERRoot", lpDefault, confBuf, 1024, dataConfigFilePath);
	m_calverListRootFolder = confBuf;

	m_dataFileBufSize = GetPrivateProfileInt("Data File", "BufSize", nDefault, dataConfigFilePath);

	
	if (m_measure)
	{
		((CButton*)GetDlgItem(IDC_RADIO_METRIC))->SetCheck(TRUE);
		GetDlgItem(IDC_STATIC_DEPTH)->SetWindowText("���(meter):");
		GetDlgItem(IDC_STATIC_SPEED)->SetWindowText("�ٶ�(m/min):");
	} 
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_IMPERIAL))->SetCheck(TRUE);
		GetDlgItem(IDC_STATIC_DEPTH)->SetWindowText("���(feet):");
		GetDlgItem(IDC_STATIC_SPEED)->SetWindowText("�ٶ�(ft/min):");
	}
	
	CString dataFileBufSizeStr;
	dataFileBufSizeStr.Format("%ld", m_dataFileBufSize);
	m_dataFileBufSize*= 1024*1024;
	GetDlgItem(IDC_EDIT_DATA_BUFFER_SIZE)->SetWindowText(dataFileBufSizeStr);

	m_cmdHandlerThread = AfxBeginThread((RUNTIME_CLASS)CCommandHandlerThread);
	m_socketThread = AfxBeginThread((RUNTIME_CLASS)CSocketThread);
	m_socketThread->SetCmdHandlerThreadID(m_cmdHandlerThread->m_nThreadID);
	m_cmdHandlerThread->SetSocketThreadID(m_socketThread->m_nThreadID);
	m_cmdHandlerThread->Init();

	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CELISTestServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CELISTestServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CELISTestServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CELISTestServerDlg::OnButtonOk() 
{
	// TODO: Add your control notification handler code here

	DestroyWindow();
}

void CELISTestServerDlg::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here

	DestroyWindow();
}

void CELISTestServerDlg::OnSelchangeElistestserverTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
	switch(m_myTabCtrl.GetCurSel()){

	case 0:
		m_myTabCtrl.m_actDialog->ShowWindow(SW_SHOW);
		m_myTabCtrl.m_calverDialog->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_myTabCtrl.m_actDialog->ShowWindow(SW_HIDE);
		m_myTabCtrl.m_calverDialog->ShowWindow(SW_SHOW);
		break;
	default:
		break;

	}
}


void CELISTestServerDlg::OnButtonActFolder() 
{
	// TODO: Add your control notification handler code here
	char szPath[MAX_PATH]; //���ѡ���Ŀ¼·��
    CString str;
    ZeroMemory(szPath, sizeof(szPath)); 
    BROWSEINFO bi;
    bi.hwndOwner = m_hWnd;
    bi.pidlRoot =  NULL;//m_actListRootFolder;
    bi.pszDisplayName = szPath;
    bi.lpszTitle = "��ѡ��ACT�����ļ���Ŀ¼:";
    bi.ulFlags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS;//0
    bi.lpfn = NULL;
    bi.lParam = 0;
    bi.iImage = 0;
	
    //����ѡ��Ŀ¼�Ի���
    LPITEMIDLIST lp = SHBrowseForFolder(&bi);
    if( lp && SHGetPathFromIDList(lp, szPath) ) {
        str.Format("%s",  szPath);
        //AfxMessageBox(_T(str));
		GetDlgItem(IDC_EDIT_ACT_FOLDER)->SetWindowText(str);
		m_actDataFileRootPath = str;
		if (SetAllDataFilePaths(m_myTabCtrl.m_actDialog->m_actListCtrl, 0))
		{
			::PostThreadMessage(m_cmdHandlerThread->m_nThreadID, WM_ALL_ACT_DATAFILE_PATHS, NULL, (LPARAM)m_actDataFilePath);
		}
		
		/*UpdateData(TRUE);
		TabAct* actListCtrlDlg = m_tabMyTabCtrl.m_dlgAct;
		int itemCount = actListCtrlDlg->m_listctrlAct.GetItemCount();
		if (itemCount > 0)
		{
			for (int i = 0; i < itemCount; i++)
			{
				actListCtrlDlg->setDataFilePath(str, i);
			}
		}*/
		//m_actListRootFolder=str;
    } else {
        //AfxMessageBox(_T("��Ч��Ŀ¼��������ѡ��!"));
    }

	
}

void CELISTestServerDlg::OnButtonServerPort() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	GetDlgItem(IDC_BUTTON_SERVER_PORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SERVER_PORT)->EnableWindow(FALSE);
	
	::SendMessage((HWND)m_socketThread->m_hThread, WM_PORT, NULL, 
	m_dataFileBufSize);
}

void CELISTestServerDlg::OnButtonCalverFolder() 
{
	// TODO: Add your control notification handler code here
	char szPath[MAX_PATH]; //���ѡ���Ŀ¼·��
    CString str;
    ZeroMemory(szPath, sizeof(szPath)); 
    BROWSEINFO bi;
    bi.hwndOwner = m_hWnd;
    bi.pidlRoot = NULL;//m_calverListRootFolder;
    bi.pszDisplayName = szPath;
    bi.lpszTitle = "��ѡ��CAL/VER�����ļ���Ŀ¼:";
    bi.ulFlags = BIF_USENEWUI;//0
    bi.lpfn = NULL;
    bi.lParam = 0;
    bi.iImage = 0;
	
    //����ѡ��Ŀ¼�Ի���
    LPITEMIDLIST lp = SHBrowseForFolder(&bi);
    if( lp && SHGetPathFromIDList(lp, szPath) ) {
        str.Format("%s",  szPath);
        //AfxMessageBox(_T(str));
		GetDlgItem(IDC_EDIT_CALVER_FOLDER)->SetWindowText(str);
		UpdateData(TRUE);
		//m_actListRootFolder=str;
    } else {
        //AfxMessageBox(_T("��Ч��Ŀ¼��������ѡ��!"));
    }
	
}

void CELISTestServerDlg::OnButtonDataBufferSize() 
{
	// TODO: Add your control notification handler code here
	CString dataFileBufSizeStr;
	GetDlgItem(IDC_EDIT_DATA_BUFFER_SIZE)->GetWindowText(dataFileBufSizeStr);
	ULONG dataFileBufSize = atol(dataFileBufSizeStr);
	m_dataFileBufSize = dataFileBufSize*1024*1024;//���ֽ�ת��Ϊ�ֽ�
	
}

void CELISTestServerDlg::OnRadioImperial() 
{
	// TODO: Add your control notification handler code here
	int old_measure = m_measure;
	m_measure = 0;
	GetDlgItem(IDC_STATIC_DEPTH)->SetWindowText("���(feet):");
	GetDlgItem(IDC_STATIC_SPEED)->SetWindowText("�ٶ�(ft/min):");
	if (old_measure)
	{
		if (m_speedPMStr != "")
		{
			m_speedPM*= (float)METRIC_DU/IMPERIAL_DU;
			m_speedPMStr.Format(FLOAT_TO_STRING_FORMAT, m_speedPM);
			GetDlgItem(IDC_STATIC_SPEED_VALUE)->SetWindowText(m_speedPMStr);
		}
		if(m_depthStr != ""){
			m_depth*= (float)METRIC_DU/IMPERIAL_DU;
			m_depthStr.Format(FLOAT_TO_STRING_FORMAT, m_depth);
			GetDlgItem(IDC_STATIC_DEPTH_VALUE)->SetWindowText(m_depthStr);
		}
		
	} 
	
	
}

void CELISTestServerDlg::OnRadioMetric() 
{
	// TODO: Add your control notification handler code here
	int old_measure = m_measure;
	m_measure=1;
	GetDlgItem(IDC_STATIC_DEPTH)->SetWindowText("���(meter):");
	GetDlgItem(IDC_STATIC_SPEED)->SetWindowText("�ٶ�(m/min):");
	if (!old_measure)
	{
		if (m_speedPMStr != "")
		{
			m_speedPM*= (float)IMPERIAL_DU/METRIC_DU;
			m_speedPMStr.Format(FLOAT_TO_STRING_FORMAT, m_speedPM);
			GetDlgItem(IDC_STATIC_SPEED_VALUE)->SetWindowText(m_speedPMStr);
		}
		if(m_depthStr != ""){
			m_depth*= (float)IMPERIAL_DU/METRIC_DU;
			m_depthStr.Format(FLOAT_TO_STRING_FORMAT, m_depth);
			GetDlgItem(IDC_STATIC_DEPTH_VALUE)->SetWindowText(m_depthStr);
		}
		
	} 
	
}

void CELISTestServerDlg::OnButtonCreateLog() 
{
	// TODO: Add your control notification handler code here
	
	if (m_speedPM != 0.0)
	{
		if(m_workMode == RtcSYS_STANDBY_CMD) {
			
			EnableStopLog(TRUE);
			EnableCreateLog(FALSE);
			EnableActRootFolderSelection(FALSE);
			
		} 
		else if (m_workMode == RtcSYS_RECSTART_CMD)
		{
			
			EnableStopLog(TRUE);
			EnableCreateLog(FALSE);
			EnableActRootFolderSelection(FALSE);
			
			
		} 
		else
		{
			AfxMessageBox(_T("��ǰ״̬ӦΪSTANDBY��RECSTART!"));
		}
	} 
	else
	{
		AfxMessageBox(_T("�ٶȻ�δ�趨!"));
	}
	
	
}

void CELISTestServerDlg::OnButtonPauseLog() 
{
	// TODO: Add your control notification handler code here
	EnableStopLog(FALSE);
	EnableCreateLog(TRUE);
	EnableActRootFolderSelection(TRUE);
	//wms->oldMode = wms->mode;
	//EnableStartLog(FALSE);
	//EnableUnitRadio(TRUE);
}


void CELISTestServerDlg::EnableActRootFolderSelection(BOOL enableButton)
{
	GetDlgItem(IDC_BUTTON_ACT_FOLDER)->EnableWindow(enableButton);
}
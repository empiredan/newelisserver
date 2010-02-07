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
	m_sPort=0;
	
	//m_pmasterDataQueue=new MasterDataQueue<CMasterData>;

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
	
	confStr.Format("%d", m_sPort);
	WritePrivateProfileString("Net Connection", "Port", confStr, dataConfigFilePath);
	
	WritePrivateProfileString("Data File", "ACTRoot", m_actListRootFolder, dataConfigFilePath);
	
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
	DDX_Control(pDX, IDC_MY_TAB, m_tabMyTabCtrl);

	DDX_Text(pDX, IDC_EDIT_SERVER_PORT, m_sPort);

	DDX_Text(pDX, IDC_EDIT_ACT_FOLDER, m_actListRootFolder);
	DDX_Text(pDX, IDC_EDIT_CALVER_FOLDER, m_calverListRootFolder);

	DDX_Text(pDX, IDC_STATIC_MODE_VALUE, m_currentWorkStateStr);
	DDX_Text(pDX, IDC_STATIC_DIRECTION_VALUE, m_directionStr);

	DDX_Text(pDX, IDC_STATIC_TRUE_DEPTH_SHOW_VALUE, m_trueDepthStr);
	DDX_Text(pDX, IDC_STATIC_CORRECTED_DEPTH_SHOW_VALUE, m_correctedDepthStr);
	DDX_Text(pDX, IDC_STATIC_SPEED_SHOW_VALUE, m_speedStr);
	DDX_Text(pDX, IDC_STATIC_CURRENT_TIME_VALUE, m_currentTimeStr);
	//DDX_Text(pDX, IDC_EDIT_SPEED, m_speedStr);
	//DDX_Text(pDX, IDC_EDIT_TRUE_DEPTH, m_trueDepthStr);
	//DDX_Text(pDX, IDC_STATIC_CURRENT_DEPTH_SHOW_VALUE, m_currentDepthStr);
	//}}AFX_DATA_MAP
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
	ON_BN_CLICKED(IDC_BUTTON_STOP_LOG, OnButtonStopLog)
	//ON_MESSAGE
	ON_MESSAGE(WM_WORKMODE, OnWorkModeUpdated)
	ON_MESSAGE(WM_DIRECTION, OnDirectionUpdated)
	ON_MESSAGE(WM_DEPTH, OnDepthUpdated)
	ON_MESSAGE(WM_SPEED, OnSpeedUpdated)
	ON_MESSAGE(WM_TIME, OnTimeUpdated)
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

	m_tabMyTabCtrl.InsertItem(0, _T("ACT"));
	m_tabMyTabCtrl.InsertItem(1, _T("Cal/Ver"));
	m_tabMyTabCtrl.Init();

	m_tabMyTabCtrl.m_dlgAct->setCElisTestServerDlg(this);
	m_tabMyTabCtrl.m_dlgCalVer->setCElisTestServerDlg(this);

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

	m_sPort = GetPrivateProfileInt("Net Connection", "Port", nDefault, dataConfigFilePath);

	GetPrivateProfileString("Data File", "ACTRoot", lpDefault, confBuf, 1024, dataConfigFilePath);
	m_actListRootFolder = confBuf;

	GetPrivateProfileString("Data File", "CALVERRoot", lpDefault, confBuf, 1024, dataConfigFilePath);
	m_calverListRootFolder = confBuf;

	m_dataFileBufSize = GetPrivateProfileInt("Data File", "BufSize", nDefault, dataConfigFilePath);

	
	if (m_measure)
	{
		((CButton*)GetDlgItem(IDC_RADIO_METRIC))->SetCheck(TRUE);
		GetDlgItem(IDC_STATIC_DEPTH)->SetWindowText("深度(meter):");
		GetDlgItem(IDC_STATIC_SPEED)->SetWindowText("速度(m/min):");
	} 
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_IMPERIAL))->SetCheck(TRUE);
		GetDlgItem(IDC_STATIC_DEPTH)->SetWindowText("深度(feet):");
		GetDlgItem(IDC_STATIC_SPEED)->SetWindowText("速度(ft/min):");
	}
	
	CString dataFileBufSizeStr;
	dataFileBufSizeStr.Format("%ld", m_dataFileBufSize);
	m_dataFileBufSize*= 1024*1024;
	GetDlgItem(IDC_EDIT_DATA_BUFFER_SIZE)->SetWindowText(dataFileBufSizeStr);

	m_cmdHandlerThread = AfxBeginThread((RUNTIME_CLASS)CCommandHandlerThread);
	m_socketThread = AfxBeginThread((RUNTIME_CLASS)CSocketThread);
	m_socketThread->SetCmdHandlerThreadID(m_cmdHandlerThread->m_nThreadID);
	m_cmdHandlerThread->SetSocketThreadID(m_socketThread->m_nThreadID);

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
	switch(m_tabMyTabCtrl.GetCurSel()){

	case 0:
		m_tabMyTabCtrl.m_dlgAct->ShowWindow(SW_SHOW);
		m_tabMyTabCtrl.m_dlgCalVer->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_tabMyTabCtrl.m_dlgAct->ShowWindow(SW_HIDE);
		m_tabMyTabCtrl.m_dlgCalVer->ShowWindow(SW_SHOW);
		break;
	default:
		break;

	}
}


void CELISTestServerDlg::OnButtonActFolder() 
{
	// TODO: Add your control notification handler code here
	char szPath[MAX_PATH]; //存放选择的目录路径
    CString str;
    ZeroMemory(szPath, sizeof(szPath)); 
    BROWSEINFO bi;
    bi.hwndOwner = m_hWnd;
    bi.pidlRoot =  NULL;//m_actListRootFolder;
    bi.pszDisplayName = szPath;
    bi.lpszTitle = "请选择ACT数据文件的目录:";
    bi.ulFlags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS;//0
    bi.lpfn = NULL;
    bi.lParam = 0;
    bi.iImage = 0;
	
    //弹出选择目录对话框
    LPITEMIDLIST lp = SHBrowseForFolder(&bi);
    if( lp && SHGetPathFromIDList(lp, szPath) ) {
        str.Format("%s",  szPath);
        //AfxMessageBox(_T(str));
		GetDlgItem(IDC_EDIT_ACT_FOLDER)->SetWindowText(str);
		UpdateData(TRUE);
		TabAct* actListCtrlDlg = m_tabMyTabCtrl.m_dlgAct;
		int itemCount = actListCtrlDlg->m_listctrlAct.GetItemCount();
		if (itemCount > 0)
		{
			for (int i = 0; i < itemCount; i++)
			{
				actListCtrlDlg->setDataFilePath(str, i);
			}
		}
		//m_actListRootFolder=str;
    } else {
        //AfxMessageBox(_T("无效的目录，请重新选择!"));
    }

	
}

void CELISTestServerDlg::OnButtonServerPort() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	GetDlgItem(IDC_BUTTON_SERVER_PORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SERVER_PORT)->EnableWindow(FALSE);
	
	::SendMessage((HWND)m_socketThread->m_hThread, WM_PORT, NULL, \
	m_dataFileBufSize);
}

void CELISTestServerDlg::OnButtonCalverFolder() 
{
	// TODO: Add your control notification handler code here
	char szPath[MAX_PATH]; //存放选择的目录路径
    CString str;
    ZeroMemory(szPath, sizeof(szPath)); 
    BROWSEINFO bi;
    bi.hwndOwner = m_hWnd;
    bi.pidlRoot = NULL;//m_calverListRootFolder;
    bi.pszDisplayName = szPath;
    bi.lpszTitle = "请选择CAL/VER数据文件的目录:";
    bi.ulFlags = BIF_USENEWUI;//0
    bi.lpfn = NULL;
    bi.lParam = 0;
    bi.iImage = 0;
	
    //弹出选择目录对话框
    LPITEMIDLIST lp = SHBrowseForFolder(&bi);
    if( lp && SHGetPathFromIDList(lp, szPath) ) {
        str.Format("%s",  szPath);
        //AfxMessageBox(_T(str));
		GetDlgItem(IDC_EDIT_CALVER_FOLDER)->SetWindowText(str);
		UpdateData(TRUE);
		//m_actListRootFolder=str;
    } else {
        //AfxMessageBox(_T("无效的目录，请重新选择!"));
    }
	
}

void CELISTestServerDlg::OnButtonDataBufferSize() 
{
	// TODO: Add your control notification handler code here
	CString dataFileBufSizeStr;
	GetDlgItem(IDC_EDIT_DATA_BUFFER_SIZE)->GetWindowText(dataFileBufSizeStr);
	ULONG dataFileBufSize = atol(dataFileBufSizeStr);
	m_dataFileBufSize = dataFileBufSize*1024*1024;//兆字节转变为字节
	
}

void CELISTestServerDlg::OnRadioImperial() 
{
	// TODO: Add your control notification handler code here
	int old_measure = m_measure;
	m_measure = 0;
	GetDlgItem(IDC_STATIC_DEPTH)->SetWindowText("深度(feet):");
	GetDlgItem(IDC_STATIC_SPEED)->SetWindowText("速度(ft/min):");
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
	GetDlgItem(IDC_STATIC_DEPTH)->SetWindowText("深度(meter):");
	GetDlgItem(IDC_STATIC_SPEED)->SetWindowText("速度(m/min):");
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
			AfxMessageBox(_T("当前状态应为STANDBY或RECSTART!"));
		}
	} 
	else
	{
		AfxMessageBox(_T("速度还未设定!"));
	}
	
	
}

void CELISTestServerDlg::OnButtonStopLog() 
{
	// TODO: Add your control notification handler code here
	EnableStopLog(FALSE);
	EnableCreateLog(TRUE);
	EnableActRootFolderSelection(TRUE);
	wms->oldMode = wms->mode;
	//EnableStartLog(FALSE);
	//EnableUnitRadio(TRUE);
}


void CELISTestServerDlg::EnableActRootFolderSelection(BOOL enableButton)
{
	GetDlgItem(IDC_BUTTON_ACT_FOLDER)->EnableWindow(enableButton);
}

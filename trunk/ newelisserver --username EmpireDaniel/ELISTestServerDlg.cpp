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
	this->m_sPort=0;
	this->m_psConnectSocket=NULL;
	m_rStatus=SOCK_RECEIVE_HEADER;
	m_pmasterDataQueue=new MasterDataQueue<CMasterData>;
	m_rbuf = new BUF_TYPE[DEFAULT_BUF_LEN];
	pBuf = m_rbuf;
	m_rbuflen = DEFAULT_BUF_LEN;
	rremain = SOCK_RECEIVE_HEADER_LEN;
	received = 0;

	acttab = NULL;
	calibpara = NULL;

	wms = new CWorkModeSetter();
	wms->changeDepth = FALSE;
	wms->changeTime = FALSE;
	wms->returnSubsetData = FALSE;
	wms->depthSign = 0;
	
	calibsubset=NULL;

	m_dataFileBufSize=5*1024*1024;

	m_subsetAssister=new CSubsetDataAssister;
	m_dataFileBuf=new CDataFileBuf(this);
	m_subsetAssister->setDataFileBuf(m_dataFileBuf);

	m_measure = 1;
	//m_speed=0.5f;
	//m_trueDepth=500;

	//m_isSpeedReceived = FALSE;
	//m_isTrueDepthReceived = FALSE;
	//m_isCorrectedDepthReceived = FALSE;
	m_speed = 0.0;
	m_speedStr.Format(FLOAT_TO_STRING_FORMAT, m_speed);
	m_speedDU = 0;

	m_trueDepth = 0.0;
	m_trueDepthStr.Format(FLOAT_TO_STRING_FORMAT, m_trueDepth);
	m_trueDepthDU = 0;

	m_correctedDepth = 0.0;
	m_correctedDepthStr.Format(FLOAT_TO_STRING_FORMAT, m_correctedDepth);
	m_correctedDepthDU = 0;

	m_currentTime = 0.0;
	m_currentTimeStr.Format(FLOAT_TO_STRING_FORMAT, m_currentTime);
	
	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	cmdh.setParentWindow(this);
	msgs.setParentWindow(this);
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


	m_sListenSocket.Close();
	
	if(m_psConnectSocket){
		m_psConnectSocket->Close();
		//m_psConnectSocket->Close();
		delete m_psConnectSocket;
		m_psConnectSocket=NULL;
	}
	if(m_pmasterDataQueue){
		delete m_pmasterDataQueue;
		m_pmasterDataQueue=NULL;
		//char* t="m_pmasterDataQueue deconstructed!";
		//AfxMessageBox(_T(t), MB_YESNO, 0);
	}
	if (m_subsetAssister)
	{
		delete m_subsetAssister;
	}
	
	if (m_dataFileBuf)
	{
		delete m_dataFileBuf;
	} 
	if(m_rbuf != NULL) {
		delete [] m_rbuf;
	}
	log.Close();
}

void CELISTestServerDlg::SetCurrentWorkState()
{
	
	switch (wms->mode)
	{
	case RtcSYS_DEACTIVE_CMD:
		m_currentWorkStateStr="RtcSYS_DEACTIVE_CMD";
		break;
	case RtcSYS_IDLE_CMD:
		m_currentWorkStateStr="RtcSYS_IDLE_CMD";
		break;
	case RtcSYS_STANDBY_CMD:
		m_currentWorkStateStr="RtcSYS_STANDBY_CMD";
		break;
	case RtcSYS_RECSTART_CMD:
		m_currentWorkStateStr="RtcSYS_RECSTART_CMD";
		break;
	case RtcSYS_RECSTOP_CMD:
		m_currentWorkStateStr="RtcSYS_RECSTOP_CMD";
		break;
	case RtcSYS_CALIBSTART_CMD:
		m_currentWorkStateStr="RtcSYS_CALIBSTART_CMD";
		break;
	default:
		CString ex; 
		ex.Format("Current Work State %lx not identified!\n",wms->mode);

		log.Write(ex.GetBuffer(ex.GetLength()),ex.GetLength());
		break;
	}
	
	GetDlgItem(IDC_STATIC_MODE_VALUE)->SetWindowText(m_currentWorkStateStr);
	//UpdateData(FALSE);
	
}
void CELISTestServerDlg::SetDirection()
{
	switch(wms->direction) {
	case 0:
		m_directionStr="UP";
		break;
	case 1:
		m_directionStr="DOWN";
		break;
	default:
		/*
		AfxMessageBox("方向数据错误!");
		*/
		break;
	}
	GetDlgItem(IDC_STATIC_DIRECTION_VALUE)->SetWindowText(m_directionStr);
	//UpdateData(FALSE);
}

/*
void CELISTestServerDlg::EnableStartLog(BOOL enableButton)
{
	GetDlgItem(IDC_BUTTON_START_LOG)->EnableWindow(enableButton);

}
*/
void CELISTestServerDlg::EnableCreateLog(BOOL enableButton)
{
	GetDlgItem(IDC_BUTTON_CREATE_LOG)->EnableWindow(enableButton);
	
}

void CELISTestServerDlg::EnableStopLog(BOOL enableButton)
{
	GetDlgItem(IDC_BUTTON_STOP_LOG)->EnableWindow(enableButton);
	
}
/*
void CELISTestServerDlg::EnableUnitRadio(BOOL enableButton)
{
	GetDlgItem(IDC_RADIO_IMPERIAL)->EnableWindow(enableButton);
	GetDlgItem(IDC_RADIO_METRIC)->EnableWindow(enableButton);
}
*/
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CELISTestServerDlg message handlers

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
	
	m_sListenSocket.SetParent(this);

	m_tabMyTabCtrl.InsertItem(0, _T("ACT"));
	m_tabMyTabCtrl.InsertItem(1, _T("Cal/Ver"));
	m_tabMyTabCtrl.Init();
	
	//m_tabMyTabCtrl.m_dlgAct->SetParent(this);
	//m_tabMyTabCtrl.m_dlgCalVer->SetParent(this);

	m_tabMyTabCtrl.m_dlgAct->setCElisTestServerDlg(this);
	m_tabMyTabCtrl.m_dlgCalVer->setCElisTestServerDlg(this);

	//OnButtonDataBufferSize();

	//GetDlgItem(IDC_EDIT_TRUE_DEPTH)->SetWindowText("50000");
	//GetDlgItem(IDC_EDIT_SPEED)->SetWindowText("0.05");
	//GetDlgItem(IDC_EDIT_SERVER_PORT)->SetWindowText("6050");

	CString dataConfigFileName = "dataconfig.ini";//D:\\vc6\\MyProjects\\elis\\ELISTestServer6.3.2
	char currentDirectoryChar[1024];
	GetCurrentDirectory(1024, currentDirectoryChar);
	CString dataConfigFilePath = currentDirectoryChar;
	dataConfigFilePath+= "\\";
	dataConfigFilePath+= dataConfigFileName;

	LPCTSTR lpDefault;
	INT nDefault;
	_TCHAR confBuf[1024];

	//GetPrivateProfileString("Parameter Setting", "TrueDepth", lpDefault, confBuf, 1024, dataConfigFilePath);
	//m_trueDepthStr = confBuf;

	//GetPrivateProfileString("Parameter Setting", "Speed", lpDefault, confBuf, 1024, dataConfigFilePath);
	//m_speedStr = confBuf;

	m_measure = GetPrivateProfileInt("Parameter Setting", "Measure", nDefault, dataConfigFilePath);

	m_sPort = GetPrivateProfileInt("Net Connection", "Port", nDefault, dataConfigFilePath);

	GetPrivateProfileString("Data File", "ACTRoot", lpDefault, confBuf, 1024, dataConfigFilePath);
	m_actListRootFolder = confBuf;

	GetPrivateProfileString("Data File", "CALVERRoot", lpDefault, confBuf, 1024, dataConfigFilePath);
	m_calverListRootFolder = confBuf;

	m_dataFileBufSize = GetPrivateProfileInt("Data File", "BufSize", nDefault, dataConfigFilePath);

	
	if (m_measure)
	{
		//m_currentDepthDU = atof(m_trueDepthStr)*METRIC_DU;
		//m_speed = atof(m_speedStr);
		//m_speedDU = m_speed*METRIC_DU;
		((CButton*)GetDlgItem(IDC_RADIO_METRIC))->SetCheck(TRUE);
		GetDlgItem(IDC_STATIC_TRUE_DEPTH_SHOW)->SetWindowText("真实深度(meter):");
		GetDlgItem(IDC_STATIC_CORRECTED_DEPTH_SHOW)->SetWindowText("校正深度(meter):");
		GetDlgItem(IDC_STATIC_SPEED_SHOW)->SetWindowText("速度(m/min):");
	} 
	else
	{
		//m_currentDepthDU = atof(m_trueDepthStr)*IMPERIAL_DU;
		//m_speed = atof(m_speedStr);
		//m_speedDU = m_speed*IMPERIAL_DU;
		((CButton*)GetDlgItem(IDC_RADIO_IMPERIAL))->SetCheck(TRUE);
		GetDlgItem(IDC_STATIC_TRUE_DEPTH_SHOW)->SetWindowText("真实深度(feet):");
		GetDlgItem(IDC_STATIC_CORRECTED_DEPTH_SHOW)->SetWindowText("校正深度(feet):");
		GetDlgItem(IDC_STATIC_SPEED_SHOW)->SetWindowText("速度(ft/min):");
	}
	
	CString dataFileBufSizeStr;
	dataFileBufSizeStr.Format("%ld", m_dataFileBufSize);
	m_dataFileBufSize*= 1024*1024;
	GetDlgItem(IDC_EDIT_DATA_BUFFER_SIZE)->SetWindowText(dataFileBufSizeStr);
	//GetDlgItem(IDC_EDIT_DATA_BUFFER_SIZE)->SetWindowText("5");
	//((CButton*)GetDlgItem(IDC_RADIO_IMPERIAL))->SetCheck(TRUE);
	
	//m_actListRootFolder="D:\\LogData\\E130~MSF71_file1";
	//m_calverListRootFolder="D:\\LogData\\刻度数据";

	cmdh.start();
	msgs.start();
	ta = 5;

	//acttab->setParentDlg(this);

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

void CELISTestServerDlg::OnClose()
{
	//m_psConnectSocket->Close();		
}

void CELISTestServerDlg::OnAccept()
{
	
	//this->m_sConnectSocket;
	//AfxMessageBox(_T(this->m_sConnectSocket));
	//AfxMessageBox(_T("Hello!"));
	m_psConnectSocket =new MySocket;
	m_psConnectSocket->SetParent(this);
	if(!m_sListenSocket.Accept(*m_psConnectSocket)){
		char t[50];
		int e = GetLastError();
		sprintf(t, "ServerSocket Accepted Error Code:%d", e);
		AfxMessageBox(_T(t), MB_YESNO, 0);
		m_sListenSocket.Close();
	} else {
		CString clientIP;
		UINT clientPort;

		m_psConnectSocket->GetPeerName(clientIP,clientPort);
		
		CString strClientPort;
		strClientPort.Format("%d",clientPort);

		GetDlgItem(IDC_STATIC_CLIENT_IP_PORT_VALUE)->SetWindowText(_T(clientIP+":"+strClientPort));

		//Accept成功，启动DepthTimer进行定期传输
		//深度数据
		StopDepthTimer();
		CreateDepthTimer(DEPTH_TIMER_INTERVAL);
	}
}
void CELISTestServerDlg::OnReceive()
{
	//char t[200];
	//AfxMessageBox(_T("CAcceptedSocket OnReceive"), MB_YESNO, 0);
	//sprintf(t, "%ld", sizeof(this->m_msDataHeader));
	ULONG *t;
	ULONG len;
	ULONG cmdType;
	//int *t;
	//int len;
	

	if (m_rStatus == SOCK_RECEIVE_HEADER) {
		len = m_psConnectSocket->Receive(pBuf, rremain, 0);
		//解析header，确定body长度
		if(len != SOCKET_ERROR && len > 0) {
			received += len;
			if(received == SOCK_RECEIVE_HEADER_LEN) {
				t = (ULONG*)m_rbuf;
				cmdType = ntohl(t[0]);
				//t = (int*)m_rbuf;	
				m_msDataLen = ntohl(t[1]);
				m_bodyLen = m_msDataLen - SOCK_RECEIVE_HEADER_LEN;
			
				if(m_bodyLen <= 0) {
					AfxMessageBox(_T("OnReceive Strange Err: message body have 0 length"));
				} else {
					if((m_bodyLen + SOCK_RECEIVE_HEADER_LEN) > m_rbuflen) {
						BUF_TYPE *bft;
						bft = new BUF_TYPE[m_bodyLen + SOCK_RECEIVE_HEADER_LEN];

						ASSERT(bft != NULL);
						
						memcpy(bft, m_rbuf, SOCK_RECEIVE_HEADER_LEN);
						delete []m_rbuf;
						m_rbuf = bft;
						m_rbuflen = m_bodyLen + SOCK_RECEIVE_HEADER_LEN;
						received = 0;
						pBuf = m_rbuf;
						rremain = m_bodyLen;
					}
					m_rStatus = SOCK_RECEIVE_BODY;
					rremain = m_bodyLen;
					received = 0;
					pBuf = m_rbuf + SOCK_RECEIVE_HEADER_LEN;
				}
			} else {
				rremain = SOCK_RECEIVE_HEADER_LEN - len;
				pBuf = pBuf + len;
			}
		} 
	} else if(m_rStatus == SOCK_RECEIVE_BODY) {
		len = m_psConnectSocket->Receive(pBuf, rremain, 0);
		
		if(len != SOCKET_ERROR && len > 0) {
			received += len;
			if(received == m_bodyLen) {
				//把接收到的rbuf填入ReceiverQueue中
				CMasterData* p_msData=new CMasterData();
				p_msData->setData(m_rbuf, m_bodyLen + SOCK_RECEIVE_HEADER_LEN);
				m_pmasterDataQueue->enQueue(p_msData);
				
				m_rStatus = SOCK_RECEIVE_HEADER;
				pBuf = m_rbuf;
				rremain = SOCK_RECEIVE_HEADER_LEN;
				received = 0;
			} else if(received < m_bodyLen) {
				rremain = m_bodyLen - received;
				pBuf += len;
			} else {
				AfxMessageBox(_T("Recceive Body should no occur"));
			}
		} else {
			if(len <= 0) {
				AfxMessageBox(_T("OnReceive body length received <=0"));
			} else {
				AfxMessageBox(_T("OnReceive Socket error"));
			}
		}
	} else {
		AfxMessageBox(_T("OnReceive wrong status"));
		//m_rStatus = SOCK_RECEIVE_HEADER;
	}
	
	//m_psConnectSocket->OnReceive(nErrorCode);
}

void CELISTestServerDlg::OnButtonOk() 
{
	// TODO: Add your control notification handler code here
	/*m_sListenSocket.Close();
	if(m_psConnectSocket){
		m_psConnectSocket->Close();
		delete m_psConnectSocket;
	}
	this->DestroyWindow();
	*/
	msgs.finish = TRUE;
	cmdh.finish = TRUE;

	DestroyWindow();
}

void CELISTestServerDlg::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	/*m_sListenSocket.Close();
	if(m_psConnectSocket){
		m_psConnectSocket->Close();
		delete m_psConnectSocket;
	}
	this->DestroyWindow();
	*/
	msgs.finish = TRUE;
	cmdh.finish = TRUE;

	DestroyWindow();
}


void CELISTestServerDlg::HandleWorkStateChange() {
	//
	char loginfo[1024];
	switch(wms->mode) {
	case RtcSYS_IDLE_CMD:
		wms->changeDepth = FALSE;
		wms->changeTime = FALSE;
		wms->returnSubsetData = FALSE;
		//EnableStartLog(FALSE);
		//EnableCreateLog(FALSE);
		//updateCurrentTime(0);
		break;
	case RtcSYS_STANDBY_CMD:
		ASSERT(acttab != NULL);

		if(wms->direction == 0) {
			wms->depthSign = -1;
		} else if(wms->direction = 1) {
			wms->depthSign = 1;
		} else {//-1, 
			//wms->depthSign = 0;
		}
		wms->changeDepth = FALSE;
		wms->changeTime = TRUE;
		wms->returnSubsetData = TRUE;
		updateCurrentTime(0);
		break;
	case RtcSYS_RECSTART_CMD:
		ASSERT(acttab != NULL);
		
		if(wms->direction == 0) {
			wms->depthSign = -1;
		} else if(wms->direction = 1) {
			wms->depthSign = 1;
		} else {//-1, 
			//wms->depthSign = 0;
			//should not here
		}
		wms->changeDepth = TRUE;
		wms->changeTime = TRUE;
		wms->returnSubsetData = TRUE;
		if (wms->oldMode != RtcSYS_RECSTART_CMD)
		{
			updateCurrentTime(0);
		}
		
		break;
	case RtcSYS_CALIBSTART_CMD:
		//重新设置DataFileBuf的缓冲区，将对应的文件
		//读入到相应的位置上，后来的刻度命令会从这个
		//位置读取刻度的SubsetData
		//离开这个状态进入
		StopLogTimer();
		break;
	case RtcSYS_TRAINSTART_CMD:
		StopLogTimer();
		break;
	default:
		StopLogTimer();//????
		sprintf(loginfo, "handleWorkStateChange, Un handled Workstate, StopLogTimer???\n");
		log.Write(loginfo, strlen(loginfo));
		break;
	}

/**
 * CreateLogTimer的条件：
 * （1）设置服务表后
 * （2）从CalVer状态结束后返回Standby time状态
 * （3）点击CreateLog（开始log）按钮
 * （4）
 * （5）
 */
/**
 * StopLogTimer的条件：
 * （1）系统Deactivate时，即处于NA状态
 * （2）系统从Idle，Standby time进入CalVer状态
 * （3）系统进入除time，depth，idle之外的其他状态
 * （4）
 * （5）
 */
	/*
	if ((wms->mode != RtcSYS_IDLE_CMD) && (wms->oldMode != wms->mode))
	{
		updateCurrentTime(0);
	}
*/
	if(wms->oldMode == NET_CMD_NA && wms->mode == RtcSYS_STANDBY_CMD) {
		//刚刚激活服务表。
		//重新生成缓冲区后重新构造DataFileBuf缓冲区并加载文件
		acttab->buildSubsetDataAssister(m_subsetAssister, m_speed, wms->mode, m_measure);
		
		m_dataFileBuf->create(m_dataFileBufSize, acttab->actNum);
		m_dataFileBuf->layout();
		m_dataFileBuf->fillWithDataFile();
		//if (m_speedStr != NULL 
		//	&& m_trueDepthStr != NULL 
		//	&& m_correctedDepthStr != NULL)
		//{
		OnButtonCreateLog();
		//}
		
		
		//
	} else if((wms->oldMode == RtcSYS_RECSTART_CMD && wms->mode == RtcSYS_STANDBY_CMD) ||
		(wms->oldMode == RtcSYS_STANDBY_CMD && wms->mode == RtcSYS_RECSTART_CMD)) {
		//经过了深度到时间或时间到深度模式的切换
		//重新构造DataFileBuf缓冲区并重新加载文件
		acttab->reBuildSubsetDataAssister(m_subsetAssister, m_speed, wms->mode, m_measure);
		m_subsetAssister->Save(log);

		//(1)stop log timer
		StopLogTimer();
		//(2)layout, fill
		m_subsetAssister->dataFileBuf->layout();
		m_subsetAssister->dataFileBuf->fillWithDataFile();
		//(3)CreateLogTimer
		CreateLogTimer(m_subsetAssister->assist.logTimerElapse);
		//
	} /*else if(wms->mode == RtcSYS_CALIBSTART_CMD) {
		//从Idle或Standby time模式进入CalVer模式
		//要在DataFileBuf中构造CalVer文件的缓冲区

		//Stop log timer
		//StopLogTimer();
		//fill(i)===>用calver的数据文件
		
		//
	} */else if(wms->oldMode == RtcSYS_CALIBSTART_CMD && wms->mode == RtcSYS_STANDBY_CMD) {
		//从CalVer状态退出时，自动转换到Idle再到Standby time
		//重新构造DataFileBuf缓冲区并重新加载文件
		//
		//Stop log timer原则上不需要再stop，因为在进入Calver时已经stop过了，见上
		//fill(i)==>用ACT的数据文件
		UINT i = m_subsetAssister->dataFileBuf->getCalVerBufBlockIndex();
		m_subsetAssister->dataFileBuf->resetCurrentPointer(i);
		fillDataFileBufWithAct();
		//
		acttab->getLogTimerElapse(m_subsetAssister, m_speed, wms->mode, m_measure);
		CreateLogTimer(m_subsetAssister->assist.logTimerElapse);
		
	}

	if (wms->changeTime)
	{

		m_timeDelta = (float)m_subsetAssister->assist.logTimerElapse/1000;
	}

	if (wms->changeDepth)
	{
		m_depthDelta_Consistency = (float)wms->depthSign/acttab->gcd;
		m_depthDelta_toImp = (float)wms->depthSign*METRIC_DU/(acttab->gcd*IMPERIAL_DU);
		m_depthDelta_toM = (float)wms->depthSign*IMPERIAL_DU/(acttab->gcd*METRIC_DU);
		
		if (acttab->nDepthInterruptMode)
		{
			m_depthDUDelta = wms->depthSign*METRIC_DU/acttab->gcd;
			
		} 
		else
		{
			m_depthDUDelta = wms->depthSign*IMPERIAL_DU/acttab->gcd;
			
		}
		
		
	}
	sprintf(loginfo, "OldMode:0x%lx,Mode:0x%lx;", wms->oldMode, wms->mode);
	sprintf(loginfo, "%sOld3Dir:%d,Old2Dir:%d,OldDir:%d,Dir:%d;", loginfo, wms->old3Direction, wms->old2Direction, wms->oldDirection, wms->direction);
	sprintf(loginfo, "%schangeDepth:%d,changeTime:%d, ReturnSubsetData:%d,depthSign:%d\n", loginfo, wms->changeDepth, wms->changeTime, wms->returnSubsetData, wms->depthSign);
	log.Write(loginfo, strlen(loginfo));
	log.Flush();
}

void CELISTestServerDlg::fillDataFileBufWithCalVer()
{
	

	MyListCtrl& calverListCtrl=m_tabMyTabCtrl.m_dlgCalVer->m_listctrlCalVer;
	CString filePath = calverListCtrl.GetItemText(0, 3);
	UINT i = m_subsetAssister->dataFileBuf->getCalVerBufBlockIndex();
	m_subsetAssister->dataFileBuf->fillWithDataFile(i, filePath);
	
	
}

void CELISTestServerDlg::fillDataFileBufWithAct()
{
	
	UINT i = m_subsetAssister->dataFileBuf->getCalVerBufBlockIndex();
	m_subsetAssister->dataFileBuf->fillWithDataFile(i);
	
}

void CELISTestServerDlg::getCalibSubset()//CCalibSubset* 
{
	CCalibSubset *rtn = NULL;
	rtn = new CCalibSubset();
	//
	rtn->setCommandHeader(calibpara, m_subsetAssister);
	rtn->setSubsetData(calibpara, m_subsetAssister);
	getFrontDataQueue()->enQueue(rtn);
	//return calibsubset;
}

void CELISTestServerDlg::CreateTimer(UINT_PTR nIDEvent, UINT uElapse) {
	SetTimer(nIDEvent, uElapse, NULL);
}
/**
 * CreateLogTimer的条件：
 * （1）设置服务表后
 * （2）从CalVer状态结束后返回Standby time状态
 * （3）点击CreateLog（开始log）按钮
 * （4）
 * （5）
 */
void CELISTestServerDlg::CreateLogTimer(UINT uElapse) {
	CreateTimer(LOGDATA_TIMER, uElapse);
	//updateCurrentTime(0);
}
void CELISTestServerDlg::CreateDepthTimer(UINT uElapse) {
	CreateTimer(DEPTH_TIMER, uElapse);
}
void CELISTestServerDlg::StopTimer(UINT_PTR nIDEvent) {
	KillTimer(nIDEvent);
}
/**
 * StopLogTimer的条件：
 * （1）系统Deactivate时，即处于NA状态
 * （2）系统从Idle，Standby time进入CalVer状态
 * （3）系统进入除time，depth，idle之外的其他状态
 * （4）
 * （5）
 */
void CELISTestServerDlg::StopLogTimer() {
	StopTimer(LOGDATA_TIMER);
}
void CELISTestServerDlg::StopDepthTimer() {
	StopTimer(DEPTH_TIMER);
}



void CELISTestServerDlg::OnTimer(UINT nIDEvent) {
	switch(nIDEvent) {
	case LOGDATA_TIMER:
		//从文件中读取相应的log数据，构造ACT表
		//生成FrontData类型对象，将其入SendQueue队列
		//构造ACT表的依据是CommandHandler线程中执行NetCmd_InitServiceTable
		//时解析的参数和相关数据结构，这个数据结构应该专门定义一个类
		//并在本Dialog里保持这个类的一个对象。
		LogDataTimerHandler();
		break;
	case DEPTH_TIMER:
		DepthTimerHandler();
	break;
	default:
		AfxMessageBox(_T("OnTimer can not find appropriate timer type"));
		break;
	}
}
void CELISTestServerDlg::LogDataTimerHandler() {
	//AfxMessageBox(_T("LogDataTimer triggered, implement me!!!"));
	
	//UpdateData(TRUE);
	int direction=wms->direction;
	if (wms->changeTime)
	{
		updateCurrentTime(m_timeDelta);
	}
	if (wms->changeDepth)
	{
		m_trueDepthDU+= m_depthDUDelta;
		m_correctedDepthDU+= m_depthDUDelta;
		
		if (acttab->nDepthInterruptMode)
		{
			if (m_measure)
			{
				updateTrueDepth(m_depthDelta_Consistency);
				updateCorrectedDepth(m_depthDelta_Consistency);
			} 
			else
			{
				updateTrueDepth(m_depthDelta_toImp);
				updateCorrectedDepth(m_depthDelta_toImp);
			}
			
		} 
		else
		{
			if (m_measure)
			{
				updateTrueDepth(m_depthDelta_toM);
				updateCorrectedDepth(m_depthDelta_toM);
			} 
			else
			{
				updateTrueDepth(m_depthDelta_Consistency);
				updateCorrectedDepth(m_depthDelta_Consistency);
			}

		}
		
		
	}

	if (wms->returnSubsetData)
	{
		CSubsetData* subsetData=new CSubsetData;
		subsetData->setCommandHeader(m_subsetAssister);
		subsetData->setSubsetData(m_subsetAssister, acttab);
		//subsetData->Save(m_subsetAssister, log);
		getFrontDataQueue()->enQueue(subsetData);

	}
	
}
void CELISTestServerDlg::DepthTimerHandler() {
	//AfxMessageBox(_T("DepthTimer triggered, implement me!!!"));
	CDPMDisplayParameter *dpmp = new CDPMDisplayParameter();
	//CFrontData *fd = new CFrontData();

	//下面这些参数应该从根据实际模拟的进程计算出来
	//填写
	dpmp->ddp.corr_Depth = m_correctedDepthDU;
	dpmp->ddp.true_Depth = m_trueDepthDU;//this->getCurrentDepthDU();
	dpmp->ddp.speed = m_speedDU;// DU per second
	dpmp->ddp.totalTension = 5;
	dpmp->ddp.mmd = 0;
	dpmp->ddp.differTension = 2;
	dpmp->ddp.time = m_currentTime*1000;
	dpmp->ddp.nreserved2 = 0;
	

	//构造这个数据后将其放入SendQueue即可
	//MessageSender线程会自动从队列中取出
	//FrontData数据并发送之
	dpmp->setData(dpmp->ddp);
	fq.enQueue(dpmp);
}

//<<<<<<< .mine
void CELISTestServerDlg::SetCalibParameter(CCalibParameter *clibpara) {
	if(calibpara != NULL) {
		delete calibpara;
		calibpara = NULL;
	}
	calibpara = clibpara;
	//在这理添加代码更新Cal/Ver的ClistTable。
	this->m_tabMyTabCtrl.m_dlgCalVer->SetCalibParameter(clibpara,acttab);
	fillDataFileBufWithCalVer();

	
}

void CELISTestServerDlg::SetACTTable(CActTable *tb) {//091206

	if(acttab != NULL) {
		delete acttab;
		acttab = NULL;
	}
	acttab = tb;
	this->m_tabMyTabCtrl.m_dlgAct->setACTTable(acttab);

/*
	char bff[8192];

	sprintf(bff, "actNum:%ld,nDepthInterruptMode:%ld,",acttab->actNum, acttab->nDepthInterruptMode);
	sprintf(bff, "%spSaList[0].actNo:%d,", bff, acttab->pSaList[0].actNo);

	sprintf(bff, "%spSaList[0].switchOn:%d,", bff, acttab->pSaList[0].switchOn);
	sprintf(bff, "%spSaList[0].actDone:%d,", bff, acttab->pSaList[0].actDone);
	sprintf(bff, "%spSaList[0].depthSampleRate:%d,", bff, acttab->pSaList[0].depthSampleRate);
	sprintf(bff, "%spSaList[0].timeInterval:%d,", bff, acttab->pSaList[0].timeInterval);

	sprintf(bff, "%spSaList[0].toolAddress:%d,", bff, acttab->pSaList[0].toolAddress);
	sprintf(bff, "%spSaList[0].subsetNo:%d,", bff, acttab->pSaList[0].subsetNo);
	sprintf(bff, "%spSaList[0].delay:%d,", bff, acttab->pSaList[0].delay);
	sprintf(bff, "%spSaList[0].m2Length:%d,", bff, acttab->pSaList[0].m2Length);
	sprintf(bff, "%spSaList[0].m5Length:%d,", bff, acttab->pSaList[0].m5Length);
	sprintf(bff, "%spSaList[0].m7Length:%d", bff, acttab->pSaList[0].m7Length);

	AfxMessageBox(_T(bff));
	*/
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


	AfxSocketInit(NULL);
	
	
	if(!m_sListenSocket.Create(this->m_sPort)) {
		AfxMessageBox(_T("Socket Error!"));
		m_sListenSocket.Close();
	}else{

		CString serverIP;
		CString serverPort;
		serverPort.Format("%d",m_sPort);
		
		
		//CString serverADDR=serverIP+":"+CString(serverPort);
		char   name[256];  
		hostent*   pHost;  
		gethostname(name,   128);//获得主机名  
		pHost   =   gethostbyname(name);//获得主机结构  
		serverIP   =   inet_ntoa(*((in_addr   *)pHost->h_addr));
		
		GetDlgItem(IDC_STATIC_SERVER_IP_PORT_VALUE)->SetWindowText(_T(serverIP+":"+serverPort));
	}
	
	if(!m_sListenSocket.Listen()){
		int nErrorCode = m_sListenSocket.GetLastError();
		if (nErrorCode != WSAEWOULDBLOCK) {
			m_sListenSocket.Close(); 
		}
	}

	
	

	
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
	CString strBufSize;
	GetDlgItem(IDC_EDIT_DATA_BUFFER_SIZE)->GetWindowText(strBufSize);
	ULONG ul=atol(strBufSize);
	if (errno==ERANGE||errno==EINVAL)
	{
		ul=5;
		
	} 
	
	m_dataFileBufSize=ul*1024*1024;//兆字节转变为字节
	
	
	
	
}
/*
void CELISTestServerDlg::OnButtonSpeed() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_STATIC_SPEED_SHOW_VALUE)->SetWindowText(m_speedStr);
	m_speed = atof(m_speedStr);
	m_speed/= 60;
	if (errno==ERANGE||errno==EINVAL){
		m_speed=0.5f;
	}
	if (m_measure)
	{
		//GetDlgItem(IDC_STATIC_SPEED_SHOW)->SetWindowText("速度(m/min):");
		m_speedDU = m_speed*METRIC_DU;
	} 
	else
	{
		//GetDlgItem(IDC_STATIC_SPEED_SHOW)->SetWindowText("速度(ft/min):");
		m_speedDU = m_speed*IMPERIAL_DU;
	}
	
	
}
*/
/*
这个按钮的响应改成：
修改速度后，启用新的速度进行log数据的传输。
首先在速度框里输入新速度值，点击其后面的Set
按钮后，新速度值显示在当前速度指示的label上。
此时再点击“以新速度进行log”按钮，则logtimer
会在新修改的速度上执行数据返回。
用户点击速度文本框后的Set按钮时，应保证把新的速度
存到成员变量m_speed中。
这个button只有当工作状态处在log数据返回状态：
RtcSYS_STANDBY_CMD、RtcSYS_RECSTART_CMD，且
已经初始化了服务表后才起作用
*/
/*
void CELISTestServerDlg::OnButtonStartLog() 
{
	// TODO: Add your control notification handler code here
	CString currentDepthStr,speedStr;
	GetDlgItem(IDC_STATIC_CURRENT_DEPTH_SHOW_VALUE)->GetWindowText(currentDepthStr);
	GetDlgItem(IDC_STATIC_SPEED_SHOW_VALUE)->GetWindowText(speedStr);
	if (currentDepthStr!="" && speedStr!="")
	{
		if((wms->mode == RtcSYS_STANDBY_CMD || wms->mode == RtcSYS_RECSTART_CMD) && acttab != NULL) {
			acttab->getLogTimerElapse(m_subsetAssister, m_speed, wms->mode, m_measure);
			StopLogTimer();
			CreateLogTimer(m_subsetAssister->assist.logTimerElapse);
			//SetCurrentTestTime(0);
		} else {
			if(acttab != NULL) {
				acttab->getLogTimerElapse(m_subsetAssister, m_speed, wms->mode, m_measure);
			}
			AfxMessageBox(_T("新速度已生效，系统当前状态不是StandBy Time或Record Time!"));
		}
	}
	else
	{
		if (currentDepthStr=="")
		{
			AfxMessageBox(_T("请设定深度后再开始log!"));
		}
		if (speedStr=="")
		{
			AfxMessageBox(_T("请设定速度度后再开始log!"));
		}
	}
	
}
*/
/*
void CELISTestServerDlg::OnButtonTrueDepth() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_STATIC_CURRENT_DEPTH_SHOW_VALUE)->SetWindowText(m_trueDepthStr);
	
	if (m_measure)
	{
		//GetDlgItem(IDC_STATIC_CURRENT_DEPTH_SHOW)->SetWindowText("当前深度(meter):");
		m_currentDepthDU = atof(m_trueDepthStr)*METRIC_DU;
	} 
	else
	{
		//GetDlgItem(IDC_STATIC_CURRENT_DEPTH_SHOW)->SetWindowText("当前深度(feet):");
		m_currentDepthDU = atof(m_trueDepthStr)*IMPERIAL_DU;
	}
	
	//m_trueDepth=atof(m_trueDepthStr);
	//if (errno==ERANGE||errno==EINVAL){
	//	m_trueDepth=500f;
	//}
	
	

}
*/
void CELISTestServerDlg::OnRadioImperial() 
{
	// TODO: Add your control notification handler code here
	int old_measure = m_measure;
	m_measure = 0;
	GetDlgItem(IDC_STATIC_TRUE_DEPTH_SHOW)->SetWindowText("真实深度(feet):");
	GetDlgItem(IDC_STATIC_CORRECTED_DEPTH_SHOW)->SetWindowText("校正深度(feet):");
	GetDlgItem(IDC_STATIC_SPEED_SHOW)->SetWindowText("速度(ft/min):");
	if (old_measure)
	{
		if (m_speedStr != "")
		{
			m_speed*= (float)METRIC_DU/IMPERIAL_DU;
			m_speedStr.Format(FLOAT_TO_STRING_FORMAT, m_speed);
			GetDlgItem(IDC_STATIC_SPEED_SHOW_VALUE)->SetWindowText(m_speedStr);
		}
		if(m_trueDepthStr != ""){
			m_trueDepth*= (float)METRIC_DU/IMPERIAL_DU;
			m_trueDepthStr.Format(FLOAT_TO_STRING_FORMAT, m_trueDepth);
			GetDlgItem(IDC_STATIC_TRUE_DEPTH_SHOW_VALUE)->SetWindowText(m_trueDepthStr);
		}
		if(m_correctedDepthStr != ""){
			m_correctedDepth*= (float)METRIC_DU/IMPERIAL_DU;
			m_correctedDepthStr.Format(FLOAT_TO_STRING_FORMAT, m_correctedDepth);
			GetDlgItem(IDC_STATIC_CORRECTED_DEPTH_SHOW_VALUE)->SetWindowText(m_correctedDepthStr);
		}
		
	} 
	
	
}

void CELISTestServerDlg::OnRadioMetric() 
{
	// TODO: Add your control notification handler code here
	int old_measure = m_measure;
	m_measure=1;
	GetDlgItem(IDC_STATIC_CURRENT_DEPTH_SHOW)->SetWindowText("真实深度(meter):");
	GetDlgItem(IDC_STATIC_CORRECTED_DEPTH_SHOW)->SetWindowText("校正深度(meter):");
	GetDlgItem(IDC_STATIC_SPEED_SHOW)->SetWindowText("速度(m/min):");
	if (!old_measure)
	{
		if (m_speedStr != "")
		{
			m_speed*= (float)IMPERIAL_DU/METRIC_DU;
			m_speedStr.Format(FLOAT_TO_STRING_FORMAT, m_speed);
			GetDlgItem(IDC_STATIC_SPEED_SHOW_VALUE)->SetWindowText(m_speedStr);
		}
		if(m_trueDepthStr != ""){
			m_trueDepth*= (float)IMPERIAL_DU/METRIC_DU;
			m_trueDepthStr.Format(FLOAT_TO_STRING_FORMAT, m_trueDepth);
			GetDlgItem(IDC_STATIC_TRUE_DEPTH_SHOW_VALUE)->SetWindowText(m_trueDepthStr);
		}
		if(m_correctedDepthStr != ""){
			m_correctedDepth*= (float)IMPERIAL_DU/METRIC_DU;
			m_correctedDepthStr.Format(FLOAT_TO_STRING_FORMAT, m_correctedDepth);
			GetDlgItem(IDC_STATIC_CORRECTED_DEPTH_SHOW_VALUE)->SetWindowText(m_correctedDepthStr);
		}
		
	} 
	
}
/*
int CELISTestServerDlg::getMeasure()
{
	return m_measure;
}

UINT CELISTestServerDlg::getCurrentDepthDU()
{
	float currentDepth;
	if (m_measure)
	{
		currentDepth=GetCurrentDepth()*METRIC_DU;
	} 
	else
	{
		currentDepth=GetCurrentDepth()*IMPERIAL_DU;
	}
	return (UINT)currentDepth;
}
*/
void CELISTestServerDlg::OnButtonCreateLog() 
{
	// TODO: Add your control notification handler code here
	//CString currentDepthStr,speedStr;
	//GetDlgItem(IDC_STATIC_CURRENT_DEPTH_SHOW_VALUE)->GetWindowText(currentDepthStr);
	//GetDlgItem(IDC_STATIC_SPEED_SHOW_VALUE)->GetWindowText(speedStr);
	/*if (m_speedStr != NULL 
		&& m_trueDepthStr != NULL 
		&& m_correctedDepthStr != NULL)
	{
		OnButtonCreateLog();
	}*/
	if (m_speedStr != "" 
		&& m_trueDepthStr != "")
	{
		if (acttab != NULL)
		{
			if(wms->mode == RtcSYS_STANDBY_CMD) {//|| wms->mode == RtcSYS_CALIBSTART_CMD
				StopLogTimer();
				//这个调用要在handleWorkstatechange中执行。
				//acttab->buildSubsetDataAssister(m_subsetAssister, m_speed, wms->mode);
				m_subsetAssister->Save(log);
				updateCurrentTime(0);
				CreateLogTimer(m_subsetAssister->assist.logTimerElapse);
				EnableStopLog(TRUE);
				EnableCreateLog(FALSE);
				EnableActRootFolderSelection(FALSE);
				//EnableStartLog(TRUE);
				//EnableUnitRadio(FALSE);
				//SetCurrentTestTime(0);
			} else if (wms->mode == RtcSYS_RECSTART_CMD)
			{
				StopLogTimer();
				//这个调用要在handleWorkstatechange中执行。
				//acttab->buildSubsetDataAssister(m_subsetAssister, m_speed, wms->mode);
				m_subsetAssister->Save(log);
				if (wms->oldMode != RtcSYS_RECSTART_CMD)
					updateCurrentTime(0);
				CreateLogTimer(m_subsetAssister->assist.logTimerElapse);
				EnableStopLog(TRUE);
				EnableCreateLog(FALSE);
				EnableActRootFolderSelection(FALSE);
				//EnableStartLog(TRUE);
				//EnableUnitRadio(FALSE);
				//SetCurrentTestTime(0);
			}else {
				AfxMessageBox(_T("系统当前状态不是StandBy Time或Record Time!"));
			}
		}
		
	} 
	else
	{
		if (m_speedStr=="")
		{
			AfxMessageBox(_T("还没有设定速度!"));
		}
		if (m_trueDepthStr=="")
		{
			AfxMessageBox(_T("还没有设定真实深度!"));
		}
		
		
	}

	
	
}

void CELISTestServerDlg::OnButtonStopLog() 
{
	// TODO: Add your control notification handler code here
	StopLogTimer();
	EnableStopLog(FALSE);
	EnableCreateLog(TRUE);
	EnableActRootFolderSelection(TRUE);
	wms->oldMode = wms->mode;
	//EnableStartLog(FALSE);
	//EnableUnitRadio(TRUE);
}

void CELISTestServerDlg::setSpeed(BUF_TYPE *buf, ULONG len)
{
	long * speed = (long *)buf;
	m_speedDU = ntohl(speed[0]);
	//m_isSpeedReceived = TRUE;
	if (m_measure)
	{
		m_speed = (float)m_speedDU/METRIC_DU;
		m_speedStr.Format(FLOAT_TO_STRING_FORMAT, m_speed);
	} 
	else
	{
		m_speed = (float)m_speedDU/IMPERIAL_DU;
		m_speedStr.Format(FLOAT_TO_STRING_FORMAT, m_speed); 
	}

	GetDlgItem(IDC_STATIC_SPEED_SHOW_VALUE)->SetWindowText(m_speedStr);
	//m_speed/= 60;

	if((wms->mode == RtcSYS_STANDBY_CMD || wms->mode == RtcSYS_RECSTART_CMD) && acttab != NULL) {
		acttab->getLogTimerElapse(m_subsetAssister, m_speed, wms->mode, m_measure);
		m_timeDelta = (float)m_subsetAssister->assist.logTimerElapse/1000;
		StopLogTimer();
		CreateLogTimer(m_subsetAssister->assist.logTimerElapse);
		//SetCurrentTestTime(0);
	} else {
		if(acttab != NULL) {
			acttab->getLogTimerElapse(m_subsetAssister, m_speed, wms->mode, m_measure);
		}
		
	}

	
	
	
	
	
}

void CELISTestServerDlg::setTrueDepth(BUF_TYPE *buf, ULONG len)
{
	long * trueDepth = (long *)buf;
	m_trueDepthDU = ntohl(trueDepth[0]);
	//m_isTrueDepthReceived = TRUE;
	if (m_measure)
	{
		m_trueDepth = (float)m_trueDepthDU/METRIC_DU;
		
	} 
	else
	{
		m_trueDepth = (float)m_trueDepthDU/IMPERIAL_DU;
	}
	m_trueDepthStr.Format(FLOAT_TO_STRING_FORMAT, m_trueDepth);
	GetDlgItem(IDC_STATIC_TRUE_DEPTH_SHOW_VALUE)->SetWindowText(m_trueDepthStr);
}

void CELISTestServerDlg::setCorrectedDepth(BUF_TYPE *buf, ULONG len)
{
	long * correctedDepth = (long *)buf;
	m_correctedDepthDU = ntohl(correctedDepth[0]);
	//m_isCorrectedDepthReceived = TRUE;
	if (m_measure)
	{
		m_correctedDepth = (float)m_correctedDepthDU/METRIC_DU;
	} 
	else
	{
		m_correctedDepth = (float)m_correctedDepthDU/IMPERIAL_DU;
	}
	m_correctedDepthStr.Format(FLOAT_TO_STRING_FORMAT, m_correctedDepth);
	GetDlgItem(IDC_STATIC_CORRECTED_DEPTH_SHOW_VALUE)->SetWindowText(m_correctedDepthStr);
}

void CELISTestServerDlg::updateCurrentTime(float delta)
{
	if (delta == 0.0)
	{
		m_currentTime = 0.0;

	}
	else{
		m_currentTime+= delta;
	}
	m_currentTimeStr.Format(FLOAT_TO_STRING_FORMAT, m_currentTime);
	GetDlgItem(IDC_STATIC_CURRENT_TIME_VALUE)->SetWindowText(m_currentTimeStr);
}

void CELISTestServerDlg::updateTrueDepth(float delta)
{
	m_trueDepth+= delta;
	m_trueDepthStr.Format(FLOAT_TO_STRING_FORMAT, m_trueDepth);
	GetDlgItem(IDC_STATIC_TRUE_DEPTH_SHOW_VALUE)->SetWindowText(m_trueDepthStr);
}

void CELISTestServerDlg::updateCorrectedDepth(float delta)
{
	m_correctedDepth+= delta;
	m_correctedDepthStr.Format(FLOAT_TO_STRING_FORMAT, m_correctedDepth);
	GetDlgItem(IDC_STATIC_CORRECTED_DEPTH_SHOW_VALUE)->SetWindowText(m_correctedDepthStr);
}
/*
UINT CELISTestServerDlg::GetCurrentTestTime()
{
	//UpdateData(TRUE);
	GetDlgItem(IDC_STATIC_CURRENT_TIME_VALUE)->GetWindowText(m_currentTimeStr);
	float currentTime;
	currentTime=atof(m_currentTimeStr);
	if (errno==ERANGE||errno==EINVAL)
	{
		currentTime=5.0;
		
	} 
	
	return currentTime*1000;
}
void CELISTestServerDlg::SetCurrentTestTime(float ct)
{
	m_currentTimeStr.Format("%10.2f",ct/1000.0);
	GetDlgItem(IDC_STATIC_CURRENT_TIME_VALUE)->SetWindowText(m_currentTimeStr);
	//UpdateData(FALSE);
	
}
float CELISTestServerDlg::GetCurrentDepth()
{
	//UpdateData(TRUE);
	GetDlgItem(IDC_STATIC_CURRENT_DEPTH_SHOW_VALUE)->GetWindowText(m_currentDepthStr);
	float currentDepth;
	currentDepth=atof(m_currentDepthStr);
	if (errno==ERANGE||errno==EINVAL)
	{
		currentDepth=5000;
		
	} 
	
	return currentDepth;
}
void CELISTestServerDlg::SetCurrentDepth(float cp)
{
	m_currentDepthStr.Format("%f",cp);
	//UpdateData(FALSE);
	GetDlgItem(IDC_STATIC_CURRENT_DEPTH_SHOW_VALUE)->SetWindowText(m_currentDepthStr);
	if (m_measure)
	{
		GetDlgItem(IDC_STATIC_CURRENT_DEPTH_SHOW)->SetWindowText("当前深度(meter):");
	} 
	else
	{
		GetDlgItem(IDC_STATIC_CURRENT_DEPTH_SHOW)->SetWindowText("当前深度(feet):");
	}
	
}
*/

void CELISTestServerDlg::EnableActRootFolderSelection(BOOL enableButton)
{
	GetDlgItem(IDC_BUTTON_ACT_FOLDER)->EnableWindow(enableButton);
}

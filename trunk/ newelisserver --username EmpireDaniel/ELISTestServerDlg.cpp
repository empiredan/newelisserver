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

CMutex CELISTestServerDlg::m_accessDataFileMutex;

CELISTestServerDlg::CELISTestServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CELISTestServerDlg::IDD, pParent)//,
	//log(".\\commandlist.txt", CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone)
{
	//{{AFX_DATA_INIT(CELISTestServerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	CString m_serverIP = "0.0.0.0";
	m_serverPort = 0;
	m_clientIP = "0.0.0.0";
	m_clientPort = 0;
	
	m_actDataFileRootPath = "";
	m_actDataFilePath = NULL;
	m_calverDataFileRootPath = "";
	m_calverDataFilePath = "";
	m_dataFileBufSize = 10*1024*1024;
	m_actNum = 0;
	
	m_speedDUPM = 0;
	m_speedPM = 0.0;
	m_speedPMStr.Format(FLOAT_TO_STRING_FORMAT, m_speedPM);
	m_depthDU = 0;
	m_depth = 0.0;
	m_depthStr.Format(FLOAT_TO_STRING_FORMAT, m_depth);
	m_timeS = 0.0;
	m_timeSStr.Format(FLOAT_TO_STRING_FORMAT, m_timeS);
	m_measure = 1;
	m_isStartLogEnabled = FALSE;
	m_isPauseLogEnabled = FALSE;
	m_isClosingWindowEnabled = TRUE;
	
	m_socketThread = NULL;
	m_cmdHandlerThread = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CELISTestServerDlg::WriteConfigFile()
{
	CString dataConfigFileName = "dataconfig.ini";//D:\\vc6\\MyProjects\\elis\\ELISTestServer6.3.2
	char currentDirectoryChar[1024];
	GetCurrentDirectory(1024, currentDirectoryChar);
	CString dataConfigFilePath = currentDirectoryChar;
	dataConfigFilePath+= "\\";
	dataConfigFilePath+= dataConfigFileName;
	
	//_TCHAR confBuf[1024];
	CString confStr;
	
	confStr.Format("%d", m_measure);
	WritePrivateProfileString("Parameter Setting", "Measure", confStr, dataConfigFilePath);
	
	confStr.Format("%d", m_serverPort);
	WritePrivateProfileString("Net Connection", "Port", confStr, dataConfigFilePath);
	
	WritePrivateProfileString("Data File", "ACTRoot", m_actDataFileRootPath, dataConfigFilePath);
	
	WritePrivateProfileString("Data File", "CALVERRoot", m_calverDataFileRootPath, dataConfigFilePath);
	
	confStr.Format("%ld", m_dataFileBufSize/(1024*1024));
	WritePrivateProfileString("Data File", "BufSize", confStr, dataConfigFilePath);
}
CELISTestServerDlg::~CELISTestServerDlg()
{

	m_cmdHandlerThread->PostThreadMessage(WM_QUIT, 0, 0);
	::WaitForSingleObject(m_cmdHandlerThread->m_hThread, INFINITE);

	m_socketThread->PostThreadMessage(WM_QUIT, 0, 0);
	::WaitForSingleObject(m_socketThread->m_hThread, INFINITE);
	
	//m_cmdHandlerThread->Delete();
	//m_socketThread->Delete();
	
	if (m_actDataFilePath)
	{
		delete [] m_actDataFilePath;
		m_actDataFilePath = NULL;
	}

	WriteConfigFile();

	//log.Close();
}

void CELISTestServerDlg::EnableStartLog(BOOL enableButton)
{
	GetDlgItem(IDC_BUTTON_START_LOG)->EnableWindow(enableButton);
}

void CELISTestServerDlg::EnableOkAndCancelButton(BOOL enableButton)
{
	GetDlgItem(IDC_Ok)->EnableWindow(enableButton);
	GetDlgItem(IDC_Cancel)->EnableWindow(enableButton);
}

void CELISTestServerDlg::EnablePauseLog(BOOL enableButton)
{
	GetDlgItem(IDC_BUTTON_PAUSE_LOG)->EnableWindow(enableButton);	
}

void CELISTestServerDlg::EnableACTRootFolderSelection(BOOL enableButton)
{
	GetDlgItem(IDC_BUTTON_ACT_FOLDER)->EnableWindow(enableButton);
}

void CELISTestServerDlg::EnableCALVERRootFolderSelection(BOOL enableButton)
{
	GetDlgItem(IDC_BUTTON_CALVER_FOLDER)->EnableWindow(enableButton);
}
void CELISTestServerDlg::EnableDataBufferSizeSet(BOOL enableButton)
{
	GetDlgItem(IDC_BUTTON_DATA_BUFFER_SIZE)->EnableWindow(enableButton);
	GetDlgItem(IDC_EDIT_DATA_BUFFER_SIZE)->EnableWindow(enableButton);
}
void CELISTestServerDlg::SetDataFilePath(ULONG i, CMyListCtrl& myListCtrl, UINT32 dataFileType)
{
	CFileFind dataFileFind;
	CString dataFileRootPath;
	(void)( dataFileType ? (dataFileRootPath = m_calverDataFileRootPath)
		: (dataFileRootPath = m_actDataFileRootPath) );
	BOOL isRootPathFinded = dataFileFind.FindFile(dataFileRootPath+"\\*.dat");
	if (isRootPathFinded)
	{
		BOOL isDataFilePathFinded = isRootPathFinded;
		UINT32 toolADDR = atoi(myListCtrl.GetItemText(i, 1));
		UINT32 subsetNo = atoi(myListCtrl.GetItemText(i, 2));
		while(isDataFilePathFinded)
		{
			isDataFilePathFinded = dataFileFind.FindNextFile();
			CString dataFilePath = dataFileFind.GetFilePath();
			
			UINT32 dataFileHeader[3];
			BUF_TYPE dataFileHeaderBuf[sizeof(UINT32)*3];
			CFile dataFile(dataFilePath, CFile::modeRead);
			dataFile.Read(dataFileHeaderBuf, sizeof(UINT32)*3);
			dataFile.Close();
			
			memcpy(dataFileHeader, dataFileHeaderBuf, sizeof(UINT32)*3);
			UINT32 realToolADDR = dataFileHeader[0];
			UINT32 realSubsetNo = dataFileHeader[1];
			UINT32 realDataFileType = dataFileHeader[2];
			if (realToolADDR == toolADDR
				&& realSubsetNo == subsetNo
				&& realDataFileType == dataFileType)
			{
				dataFileType ? (m_calverDataFilePath = dataFilePath) 
					: (m_actDataFilePath[i] = dataFilePath);
				myListCtrl.SetItemText(i, dataFileType?3:5, dataFilePath);
				dataFileFind.Close();
				return ;//TRUE;
			}
		}
	}
	(void)( dataFileType ? (m_calverDataFilePath = "") : (m_actDataFilePath[i] = "") );
	dataFileFind.Close();
	//return FALSE;
	
}

void CELISTestServerDlg::SetDataFilePath(ULONG i, CString dataFilePath, CMyListCtrl& myListCtrl, UINT32 dataFileType)
{
	UINT32 dataFileHeader[3];
	BUF_TYPE dataFileHeaderBuf[sizeof(UINT32)*3];
	CFile dataFile(dataFilePath, CFile::modeRead);
	dataFile.Read(dataFileHeaderBuf, sizeof(UINT32)*3);
	dataFile.Close();
	
	memcpy(dataFileHeader, dataFileHeaderBuf, sizeof(UINT32)*3);
	UINT32 toolADDR=dataFileHeader[0];
	UINT32 subsetNo=dataFileHeader[1];
	UINT32 dataType=dataFileHeader[2];
	if (toolADDR == (UINT32)atoi(myListCtrl.GetItemText(i, 1))
		&& subsetNo == (UINT32)atoi(myListCtrl.GetItemText(i, 2))
		&& dataType == dataFileType)//文件格式匹配
	{
		if (myListCtrl.GetItemText(i, dataFileType?3:5) != dataFilePath)
		{
			myListCtrl.SetItemText(i, dataFileType?3:5, dataFilePath);
			m_cmdHandlerThread->PostThreadMessage(
			dataFileType?WM_CALVER_DATAFILE_PATH:WM_ACT_DATAFILE_PATH, 
			dataFileType?NULL:(WPARAM)i, 
			(LPARAM)(&dataFilePath));
			//return TRUE;
		} 
		//return FALSE;				
	} 
	else
	{
		char t[50];
		sprintf(t, "%s", "文件选择错误,请重新选择!");
		AfxMessageBox(_T(t));
		//return FALSE;
	}
}

void CELISTestServerDlg::SetAllDataFilePaths(CMyListCtrl& myListCtrl, UINT32 dataFileType)
{
	//BOOL findDataFile = FALSE;
	CFileFind dataFileFind;
	BOOL isRootPathFinded = dataFileFind.FindFile(m_actDataFileRootPath+"\\*.dat");
	if (isRootPathFinded)
	{
		m_cmdHandlerThread->PostThreadMessage(WM_ACT_DATAFILE_ROOT_PATH, NULL, (LPARAM)(&m_actDataFileRootPath));
		
		for (ULONG i = 0; i < (ULONG)myListCtrl.GetItemCount(); i++)
		{
			//findDataFile|= SetDataFilePath(i, myListCtrl, dataFileType);
			CELISTestServerDlg::m_accessDataFileMutex.Lock();
			SetDataFilePath(i, myListCtrl, dataFileType);
			CELISTestServerDlg::m_accessDataFileMutex.Unlock();
		}
	}
	dataFileFind.Close();
	//return findDataFile;
}

void CELISTestServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CELISTestServerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_MY_TAB, m_myTabCtrl);

	DDX_Text(pDX, IDC_EDIT_SERVER_PORT, m_serverPort);
	DDX_Text(pDX, IDC_EDIT_ACT_FOLDER, m_actDataFileRootPath);
	DDX_Text(pDX, IDC_EDIT_CALVER_FOLDER, m_calverDataFileRootPath);
	//DDX_Text(pDX, IDC_EDIT_DATA_BUFFER_SIZE, m_dataFileBufSize);

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
	ON_BN_CLICKED(IDC_BUTTON_SERVER_PORT_CONNECTION, OnButtonServerPortConnection)
	ON_BN_CLICKED(IDC_BUTTON_SERVER_PORT_DISCONNECTION, OnButtonServerPortDisconnection)
	ON_BN_CLICKED(IDC_BUTTON_CALVER_FOLDER, OnButtonCalverFolder)
	ON_BN_CLICKED(IDC_BUTTON_DATA_BUFFER_SIZE, OnButtonDataBufferSize)
	ON_BN_CLICKED(IDC_RADIO_IMPERIAL, OnRadioImperial)
	ON_BN_CLICKED(IDC_RADIO_METRIC, OnRadioMetric)
	ON_BN_CLICKED(IDC_BUTTON_START_LOG, OnButtonStartLog)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE_LOG, OnButtonPauseLog)
	ON_MESSAGE(WM_WORKMODE, OnWorkModeUpdated)
	ON_MESSAGE(WM_DIRECTION, OnDirectionUpdated)
	ON_MESSAGE(WM_DEPTH, OnDepthUpdated)
	ON_MESSAGE(WM_SPEED, OnSpeedUpdated)
	ON_MESSAGE(WM_TIME, OnTimeUpdated)
	ON_MESSAGE(WM_ENABLE_START_LOG, OnStartLogEnabled)
	ON_MESSAGE(WM_ENABLE_PAUSE_LOG, OnPauseLogEnabled)
	ON_MESSAGE(WM_SERVER_IP_PORT, OnShowServerIPAndPort)
	ON_MESSAGE(WM_CLIENT_IP_PORT, OnShowClientIPAndPort)
	ON_MESSAGE(WM_ENABLE_SERVER_PORT_CONNECTION, OnButtonServerPortConnectionEnabled)
	ON_MESSAGE(WM_ENABLE_SERVER_PORT_DISCONNECTION, OnButtonServerPortDisconnectionEnabled)
	ON_MESSAGE(WM_ACT_LIST, OnACTListUpdated)
	ON_MESSAGE(WM_CALVER_LIST, OnCALVERListUpdated)
	ON_MESSAGE(WM_ENABLE_ACT_ROOT_FOLDER_BUTTON, OnButtonACTRootFolderEnabled)
	ON_MESSAGE(WM_ENABLE_CALVER_ROOT_FOLDER_BUTTON, OnButtonCALVERRootFolderEnabled)
	ON_MESSAGE(WM_ENABLE_DATA_BUFFER_SIZE_BUTTON, OnButtonDataBufferSizeEnabled)
	ON_WM_TIMER()
	ON_WM_CLOSE()
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
	case RtcSYS_IDLE_CMD:
		EnableStartLog(FALSE);
		EnablePauseLog(FALSE);
		m_workModeStr = "IDLE";
		break;
	case RtcSYS_STANDBY_CMD:
		m_workModeStr = "STANDBY";
		break;
	case RtcSYS_RECSTART_CMD:
		m_workModeStr = "RECSTART";
		break;
	case RtcSYS_TRAINSTART_CMD:
		EnableStartLog(FALSE);
		EnablePauseLog(FALSE);
		m_workModeStr = "TRAINSTART";
		break;
	case RtcSYS_CALIBSTART_CMD:
		EnableStartLog(FALSE);
		EnablePauseLog(FALSE);
		m_workModeStr = "CALIBSTART";
		break;
	default:
		m_workModeStr = "UNDEFINED WORK MODE";
		break;
	}
	GetDlgItem(IDC_STATIC_WORKMODE_VALUE)->SetWindowText(m_workModeStr);
	
}
VOID CELISTestServerDlg::OnDirectionUpdated(WPARAM wParam, LPARAM lParam)
{
	m_direction = (int)lParam;

	switch(m_direction)
	{
	case 0:
		m_directionStr = "UP";
		break;
	case 1:
		m_directionStr = "DOWN";
		break;
	default:
		m_directionStr = "";
		break;
	}
	GetDlgItem(IDC_STATIC_DIRECTION_VALUE)->SetWindowText(m_directionStr);

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
VOID CELISTestServerDlg::OnButtonServerPortConnectionEnabled(WPARAM wParam, LPARAM lParam)
{
	BOOL isServerPortConnectionEnabled = (BOOL)lParam;
}
VOID CELISTestServerDlg::OnButtonServerPortDisconnectionEnabled(WPARAM wParam, LPARAM lParam)
{
	BOOL isServerPortDisconnectionEnabled = (BOOL)lParam;
	if (!isServerPortDisconnectionEnabled)
	{//"MyConnectSocket" has been disconnected
		GetDlgItem(IDC_STATIC_CLIENT_IP_PORT_VALUE)->SetWindowText(_T(""));
	}
	
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
	//BOOL findDataFile = FALSE;
	CFileFind dataFileFind;
	BOOL isRootPathFinded = dataFileFind.FindFile(m_actDataFileRootPath+"\\*.dat");
	for (ULONG i = 0; i <m_actNum; i++)
	{
		char str[256];

		itoa(m_actList->pSaList[i].actNo, str, 10);
		m_myTabCtrl.m_actDialog->m_actListCtrl.InsertItem(i, str);

		itoa(m_actList->pSaList[i].toolAddress, str, 10);
		m_myTabCtrl.m_actDialog->m_actListCtrl.SetItemText(i, 1, str);

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
		
		if (isRootPathFinded)
		{
			//findDataFile|= SetDataFilePath(i, m_myTabCtrl.m_actDialog->m_actListCtrl, 0);
			CELISTestServerDlg::m_accessDataFileMutex.Lock();
			SetDataFilePath(i, m_myTabCtrl.m_actDialog->m_actListCtrl, 0);
			CELISTestServerDlg::m_accessDataFileMutex.Unlock();
		}	

	}
	dataFileFind.Close();
	/*
	if (findDataFile)
	{
		m_cmdHandlerThread->PostThreadMessage(WM_ALL_ACT_DATAFILE_PATHS, NULL, (LPARAM)m_actDataFilePath);
	} */
	
}
VOID CELISTestServerDlg::OnCALVERListUpdated(WPARAM wParam, LPARAM lParam)
{
	m_calibData = (CalibData *)lParam;
	m_myTabCtrl.m_calverDialog->m_calverListCtrl.DeleteAllItems();

	char str[256];

	itoa(m_calibData->rtcSubset.actNo, str, 10);
	m_myTabCtrl.m_calverDialog->m_calverListCtrl.InsertItem(0, str);

	itoa(m_calibData->rtcSubset.toolAddress, str, 10);
	m_myTabCtrl.m_calverDialog->m_calverListCtrl.SetItemText(0, 1, str);

	itoa(m_calibData->rtcSubset.subsetNo, str, 10);
	m_myTabCtrl.m_calverDialog->m_calverListCtrl.SetItemText(0, 2, str);

	CELISTestServerDlg::m_accessDataFileMutex.Lock();
	SetDataFilePath(0, m_myTabCtrl.m_calverDialog->m_calverListCtrl, 1);
	CELISTestServerDlg::m_accessDataFileMutex.Unlock();

}
VOID CELISTestServerDlg::OnStartLogEnabled(WPARAM wParam, LPARAM lParam)
{
	m_isStartLogEnabled = (BOOL)lParam;
	EnableStartLog(m_isStartLogEnabled);
}
VOID CELISTestServerDlg::OnPauseLogEnabled(WPARAM wParam, LPARAM lParam)
{
	m_isPauseLogEnabled = (BOOL)lParam;
	EnablePauseLog(m_isPauseLogEnabled);
}
VOID CELISTestServerDlg::OnButtonACTRootFolderEnabled(WPARAM wParam, LPARAM lParam)
{
	m_isACTRootFolderButtonEnabled = (BOOL)lParam;
	EnableACTRootFolderSelection(m_isACTRootFolderButtonEnabled);
}
VOID CELISTestServerDlg::OnButtonCALVERRootFolderEnabled(WPARAM wParam, LPARAM lParam)
{
	m_isCALVERRootFolderButtonEnabled = (BOOL)lParam;
	EnableCALVERRootFolderSelection(m_isCALVERRootFolderButtonEnabled);
}
VOID CELISTestServerDlg::OnButtonDataBufferSizeEnabled(WPARAM wParam, LPARAM lParam)
{
	BOOL isDataBufferSizeButtonEnabled = (BOOL)lParam;
	EnableDataBufferSizeSet(isDataBufferSizeButtonEnabled);
	EnableOkAndCancelButton(isDataBufferSizeButtonEnabled);
	m_isClosingWindowEnabled = isDataBufferSizeButtonEnabled;
}
void CELISTestServerDlg::ReadConfigFile()
{
	CString dataConfigFileName = "dataconfig.ini";//D:\\vc6\\MyProjects\\elis\\ELISTestServer6.3.2
	char currentDirectoryChar[1024];
	GetCurrentDirectory(1024, currentDirectoryChar);
	CString dataConfigFilePath = currentDirectoryChar;
	dataConfigFilePath+= "\\";
	dataConfigFilePath+= dataConfigFileName;
	
	LPCTSTR lpDefault;
	INT nDefault;
	_TCHAR confBuf[1024];
	
	nDefault = 1;
	m_measure = GetPrivateProfileInt("Parameter Setting", "Measure", nDefault, dataConfigFilePath);
	
	nDefault = 6050;
	m_serverPort = GetPrivateProfileInt("Net Connection", "Port", nDefault, dataConfigFilePath);
	
	lpDefault = "D:\\Log\\LogData\\刻度数据";
	GetPrivateProfileString("Data File", "ACTRoot", lpDefault, confBuf, 1024, dataConfigFilePath);
	m_actDataFileRootPath = confBuf;
	
	lpDefault = "D:\\Log\\LogData\\E115";
	GetPrivateProfileString("Data File", "CALVERRoot", lpDefault, confBuf, 1024, dataConfigFilePath);
	m_calverDataFileRootPath = confBuf;
	
	m_dataFileBufSize = GetPrivateProfileInt("Data File", "BufSize", nDefault, dataConfigFilePath);
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

	ReadConfigFile();
	
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
	m_dataFileBufSize*= 1024*1024;//WM_QUIT
	GetDlgItem(IDC_EDIT_DATA_BUFFER_SIZE)->SetWindowText(dataFileBufSizeStr);

	m_cmdHandlerThread = AfxBeginThread(RUNTIME_CLASS(CCommandHandlerThread));
	m_socketThread = AfxBeginThread(RUNTIME_CLASS(CSocketThread));
	m_socketThread->PostThreadMessage(WM_CMD_HANDLER_THREAD_ID, NULL, m_cmdHandlerThread->m_nThreadID);
	m_cmdHandlerThread->PostThreadMessage(WM_SOCKET_THREAD_ID, NULL, m_socketThread->m_nThreadID);
	//((CSocketThread)m_socketThread)->SetCmdHandlerThreadID(m_cmdHandlerThread->m_nThreadID);
	//((CCommandHandlerThread)m_cmdHandlerThread)->SetSocketThreadID(m_socketThread->m_nThreadID);
	//((CCommandHandlerThread)m_cmdHandlerThread)->Init();
	m_cmdHandlerThread->PostThreadMessage(WM_ACT_DATAFILE_ROOT_PATH, NULL, (LPARAM)(&m_actDataFileRootPath));
	m_cmdHandlerThread->PostThreadMessage(WM_CALVER_DATAFILE_ROOT_PATH, NULL, (LPARAM)(&m_calverDataFileRootPath));
	m_cmdHandlerThread->PostThreadMessage(WM_DATABUF_LEN, NULL, m_dataFileBufSize);
	
	UpdateData(FALSE);

	OnButtonServerPortConnection();

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

void CELISTestServerDlg::OnButtonServerPortConnection() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	GetDlgItem(IDC_BUTTON_SERVER_PORT_CONNECTION)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SERVER_PORT)->EnableWindow(FALSE);
	::PostThreadMessage(m_socketThread->m_nThreadID, WM_PORT, NULL, 
	(LPARAM)m_serverPort);
	/*::SendMessage((HWND)m_socketThread->m_hThread, WM_PORT, NULL, 
	m_dataFileBufSize);*/
}
void CELISTestServerDlg::OnButtonServerPortDisconnection() 
{
	// TODO: Add your control notification handler code here
	
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
		m_calverDataFileRootPath = str;
		m_cmdHandlerThread->PostThreadMessage(WM_CALVER_DATAFILE_ROOT_PATH, NULL, (LPARAM)(&m_calverDataFileRootPath));
		/*
		if (SetAllDataFilePaths(m_myTabCtrl.m_calverDialog->m_calverListCtrl, 1))
		{
			::PostThreadMessage(m_cmdHandlerThread->m_nThreadID, WM_CALVER_DATAFILE_PATH, NULL, (LPARAM)&m_calverDataFilePath);
		}*/
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
    if( lp && SHGetPathFromIDList(lp, szPath) ) 
	{
        str.Format("%s",  szPath);
		GetDlgItem(IDC_EDIT_ACT_FOLDER)->SetWindowText(str);
		m_actDataFileRootPath = str;
		SetAllDataFilePaths(m_myTabCtrl.m_actDialog->m_actListCtrl, 0);
		m_cmdHandlerThread->PostThreadMessage(WM_ACT_DATAFILE_ROOT_PATH, NULL, (LPARAM)(&m_actDataFileRootPath));
		/*
		if (SetAllDataFilePaths(m_myTabCtrl.m_actDialog->m_actListCtrl, 0))
		{
			::PostThreadMessage(m_cmdHandlerThread->m_nThreadID, WM_ALL_ACT_DATAFILE_PATHS, NULL, (LPARAM)m_actDataFilePath);
		}*/
		
    }

	
}



void CELISTestServerDlg::OnButtonDataBufferSize() 
{
	// TODO: Add your control notification handler code here
	CString dataFileBufSizeStr;
	GetDlgItem(IDC_EDIT_DATA_BUFFER_SIZE)->GetWindowText(dataFileBufSizeStr);
	ULONG dataFileBufSize = atol(dataFileBufSizeStr);
	m_dataFileBufSize = dataFileBufSize*1024*1024;//兆字节转变为字节
	m_cmdHandlerThread->PostThreadMessage(WM_DATABUF_LEN, NULL, m_dataFileBufSize);
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

void CELISTestServerDlg::OnButtonStartLog() 
{
	// TODO: Add your control notification handler code here
	if (m_speedPM != 0.0)
	{
		switch (m_workMode)
		{
		case RtcSYS_RECSTART_CMD:
			
		case RtcSYS_STANDBY_CMD:
			EnableStartLog(FALSE);
			EnablePauseLog(TRUE);
			//EnableACTRootFolderSelection(FALSE);
			m_cmdHandlerThread->PostThreadMessage(WM_ENABLE_RETURN_SUBSET_DATA, NULL, (LPARAM)TRUE);
			break;
		default:
			AfxMessageBox(_T("当前状态应为STANDBY或RECSTART!"));
		}
	} 
	else
	{
		AfxMessageBox(_T("速度还未设定!"));
	}
	
}

void CELISTestServerDlg::OnButtonPauseLog() 
{
	// TODO: Add your control notification handler code here
	EnableStartLog(TRUE);
	EnablePauseLog(FALSE);
	//EnableACTRootFolderSelection(TRUE);
	m_cmdHandlerThread->PostThreadMessage(WM_ENABLE_RETURN_SUBSET_DATA, NULL, (LPARAM)FALSE);
}






void CELISTestServerDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//if (m_isClosingWindowEnabled)
	//{
	CDialog::OnClose();
	//}
	
}

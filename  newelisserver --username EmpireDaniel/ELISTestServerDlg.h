// ELISTestServerDlg.h : header file
//

#if !defined(AFX_ELISTESTSERVERDLG_H__6AFE4040_494B_4411_87B4_38BD588A2CAA__INCLUDED_)
#define AFX_ELISTESTSERVERDLG_H__6AFE4040_494B_4411_87B4_38BD588A2CAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "MyTabCtrl.h"
//#include "Queue.h"
#include "commands.h"
#include "ACTList.h"
#include "Calib.h"
#include "CommandHandlerThread.h"
#include "SocketThread.h"
#include "MyTabCtrl.h"
#include "MyListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CELISTestServerDlg dialog
#define FLOAT_TO_STRING_FORMAT "%10.2f"

#define WM_WORKMODE WM_USER+21
#define WM_DIRECTION WM_USER+22
#define WM_DEPTH WM_USER+23
#define WM_SPEED WM_USER+24
#define WM_TIME WM_USER+25
#define WM_SERVER_IP_PORT WM_USER+26
//#define WM_SERVER_IP WM_USER+25
//#define WM_SERVER_PORT WM_USER+26
#define WM_CLIENT_IP_PORT WM_USER+27
//#define WM_CLIENT_IP WM_USER+27
//#define WM_CLIENT_PORT WM_USER+28
#define WM_ACT_LIST WM_USER+28
#define WM_CALVER_LIST WM_USER+29
#define WM_ENABLE_START_LOG WM_USER+30
#define WM_ENABLE_PAUSE_LOG WM_USER+31
#define WM_ENABLE_ACT_ROOT_FOLDER_BUTTON WM_USER+32
#define WM_ENABLE_CALVER_ROOT_FOLDER_BUTTON WM_USER+33
#define WM_ENABLE_DATA_BUFFER_SIZE_BUTTON WM_USER+34
#define WM_ENABLE_SERVER_PORT_CONNECTION WM_USER+35
#define WM_ENABLE_SERVER_PORT_DISCONNECTION WM_USER+36

class CELISTestServerDlg : public CDialog
{
// Construction
public:
	CELISTestServerDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CELISTestServerDlg();

// Attributes
public:
	static CMutex m_accessDataFileMutex;
public:
	/**
	* Net connection
	*/
	CString m_serverIP;
	UINT m_serverPort;
	CString m_clientIP;
	UINT m_clientPort;

	/**
	* Data file
	*/
	CString m_actDataFileRootPath;
	CString m_calverDataFileRootPath;
	ULONG m_dataFileBufSize;

	CString * m_actDataFilePath;
	ULONG m_actNum;
	CString m_calverDataFilePath;

	BOOL m_isACTRootFolderButtonEnabled;
	BOOL m_isCALVERRootFolderButtonEnabled;

	/**
	* Parameters showing
	*/
	CString m_workModeStr;
	UINT32 m_workMode;
	CString m_directionStr;
	int m_direction;
	
	long m_speedDUPM;// DU per minute
	float m_speedPM;// per minute
	CString m_speedPMStr;//per minute
	
	long m_depthDU; //DU
	float m_depth; //according to unit system
	CString m_depthStr; //according to unit system
	
	CString m_timeSStr;
	float m_timeS; // second
	
	int m_measure;

	BOOL m_isStartLogEnabled;
	BOOL m_isPauseLogEnabled;
	BOOL m_isClosingWindowEnabled;
	/**
	* ACT list
	*/
	ACTList * m_actList;

	/**
	* CAL/VER list
	*/
	CalibData * m_calibData;

	/**
	* Thread objects inheriting from CWinThread  
	*/
	CWinThread * m_socketThread;
	CWinThread * m_cmdHandlerThread;
	
	//MasterDataQueue<CMasterData>* m_pmasterDataQueue;
	//FrontDataQueue<CFrontData> fq;

    /**
	* Log file
	*/
	CFile log;
/*
	MasterDataQueue<CMasterData>* getMasterDataQueue() {
		return m_pmasterDataQueue;
	}

	void CreateTimer(UINT_PTR nIDEvent, UINT uElapse);
	void CreateLogTimer(UINT uElapse);
	void CreateDepthTimer(UINT uElapse);
	void StopTimer(UINT_PTR nIDEvent);
	void StopLogTimer();
	void StopDepthTimer();
	FrontDataQueue<CFrontData>* getFrontDataQueue() {
		return &fq;
	}
	
	void DepthTimerHandler();
	void LogDataTimerHandler();

	void SetACTTable(CActTable *tb);
	void SetCalibParameter(CCalibParameter *clibpara);

	void SetCurrentWorkState();
	void SetDirection();
		*/
	//void SetCurrentTestTime(float ct);
	//void SetCurrentDepth(float cp);
	//float GetCurrentDepth();
	//UINT GetCurrentTestTime();

	//void EnableStartLog(BOOL enableButton);
	
	//void EnableUnitRadio(BOOL enableButton);
	//void EnableStartRelog(BOOL enableButton);
/*
	void getCalibSubset();//CCalibSubset* 

	void HandleWorkStateChange();
	*/
	//DWORD WINAPI WriteCmdRecvQueFunc(LPVOID lpParameter);

// Dialog Data
	//{{AFX_DATA(CELISTestServerDlg)
	enum { IDD = IDD_ELISTESTSERVER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	CMyTabCtrl m_myTabCtrl;
	CButton* m_fileButton;
	
	
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CELISTestServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Operations
public:
	void EnableOkAndCancelButton(BOOL enableButton);
	void EnableACTRootFolderSelection(BOOL enableButton);
	void EnableCALVERRootFolderSelection(BOOL enableButton);
	void EnableStartLog(BOOL enableButton);
	void EnablePauseLog(BOOL enableButton);
	void EnableDataBufferSizeSet(BOOL enableButton);
	void SetDataFilePath(ULONG i, CMyListCtrl& myListCtrl, UINT32 dataFileType);
	void SetDataFilePath(ULONG i, CString dataFilePath, CMyListCtrl& myListCtrl, UINT32 dataFileType);
	void SetAllDataFilePaths(CMyListCtrl& myListCtrl, UINT32 dataFileType);
	void ReadConfigFile();
	void WriteConfigFile();

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CELISTestServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonOk();
	afx_msg void OnButtonCancel();
	afx_msg void OnSelchangeElistestserverTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonActFolder();
	afx_msg void OnButtonServerPortConnection();
	afx_msg void OnButtonServerPortDisconnection();
	afx_msg void OnButtonCalverFolder();
	afx_msg void OnButtonDataBufferSize();
	afx_msg void OnRadioImperial();
	afx_msg void OnRadioMetric();
	afx_msg void OnButtonStartLog();
	afx_msg void OnButtonPauseLog();
	afx_msg VOID OnWorkModeUpdated(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnDirectionUpdated(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnDepthUpdated(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnSpeedUpdated(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnTimeUpdated(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnStartLogEnabled(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnPauseLogEnabled(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnShowServerIPAndPort(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnShowClientIPAndPort(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnButtonServerPortConnectionEnabled(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnButtonServerPortDisconnectionEnabled(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnACTListUpdated(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnCALVERListUpdated(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnButtonACTRootFolderEnabled(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnButtonCALVERRootFolderEnabled(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnButtonDataBufferSizeEnabled(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELISTESTSERVERDLG_H__6AFE4040_494B_4411_87B4_38BD588A2CAA__INCLUDED_)

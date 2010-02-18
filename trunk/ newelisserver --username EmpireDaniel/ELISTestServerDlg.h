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
//#include "CalibParameter.h"
//#include "CalibSubset.h"
#include "ACTList.h"
#include "CommandHandlerThread.h"
#include "SocketThread.h"
#include "MyTabCtrl.h"
#include "MyListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CELISTestServerDlg dialog
#define FLOAT_TO_STRING_FORMAT "%10.2f"

#define WM_WORKMODE WM_USER+20
#define WM_DIRECTION WM_USER+21
#define WM_DEPTH WM_USER+22
#define WM_SPEED WM_USER+23
#define WM_TIME WM_USER+24
#define WM_SERVER_IP_PORT WM_USER+25
//#define WM_SERVER_IP WM_USER+25
//#define WM_SERVER_PORT WM_USER+26
#define WM_CLIENT_IP_PORT WM_USER+26
//#define WM_CLIENT_IP WM_USER+27
//#define WM_CLIENT_PORT WM_USER+28
#define WM_ACT_LIST WM_USER+27


class CELISTestServerDlg : public CDialog
{
// Construction
public:
	CELISTestServerDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CELISTestServerDlg();

// Attributes
private:
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
	CString m_calverListRootFolder;
	ULONG m_dataFileBufSize;

	CString * m_actDataFilePath;
	ULONG m_actNum;
	CString m_calverDataFilePath;

	/**
	* Parameters showing
	*/
	CString m_workModeStr;
	UINT32 m_workMode;
	CString m_directionStr;
	UINT32 m_direction;
	
	long m_speedDUPM;// DU per minute
	float m_speedPM;// per minute
	CString m_speedPMStr;//per minute
	
	long m_depthDU; //DU
	float m_depth; //according to unit system
	CString m_depthStr; //according to unit system
	
	CString m_timeSStr;
	float m_timeS; // second
	
	int m_measure;

	/**
	* ACT list
	*/
	ACTList * m_actList;



	/**
	* Thread objects inheriting from CWinThread  
	*/
	CSocketThread * m_socketThread;
	CCommandHandlerThread * m_cmdHandlerThread;
	
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
	void EnableActRootFolderSelection(BOOL enableButton);
	void EnableCreateLog(BOOL enableButton);
	void EnableStopLog(BOOL enableButton);
	inline BOOL SetDataFilePath(ULONG i, CMyListCtrl myListCtrl, UINT32 dataFileType);
	inline BOOL SetAllDataFilePaths(CMyListCtrl myListCtrl, UINT32 dataFileType);

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
	//afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonServerPort();
	afx_msg void OnButtonCalverFolder();
	afx_msg void OnButtonDataBufferSize();
	afx_msg void OnRadioImperial();
	afx_msg void OnRadioMetric();
	afx_msg void OnButtonCreateLog();
	afx_msg void OnButtonPauseLog();

	afx_msg VOID OnWorkModeUpdated(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnDirectionUpdated(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnDepthUpdated(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnSpeedUpdated(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnTimeUpdated(WPARAM wParam, LPARAM lParam);

	afx_msg VOID OnShowServerIPAndPort(WPARAM wParam, LPARAM lParam);
	//afx_msg VOID OnShowServerPort(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnShowClientIPAndPort(WPARAM wParam, LPARAM lParam);
	//afx_msg VOID OnShowClientPort(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnACTListUpdated(WPARAM wParam, LPARAM lParam);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELISTESTSERVERDLG_H__6AFE4040_494B_4411_87B4_38BD588A2CAA__INCLUDED_)
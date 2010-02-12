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
#include "CommandHandlerThread.h"
#include "SocketThread.h"

/////////////////////////////////////////////////////////////////////////////
// CELISTestServerDlg dialog
#define FLOAT_TO_STRING_FORMAT "%10.2f"

#define WM_WORKMODE WM_USER+20
#define WM_DIRECTION WM_USER+21
#define WM_DEPTH WM_USER+22
#define WM_SPEED WM_USER+23
#define WM_TIME WM_USER+24


class CELISTestServerDlg : public CDialog
{
// Construction
public:
	void EnableActRootFolderSelection(BOOL enableButton);
	void EnableCreateLog(BOOL enableButton);
	void EnableStopLog(BOOL enableButton);
	CELISTestServerDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CELISTestServerDlg();

	UINT m_sPort;

	CString m_actListRootFolder;
	CString m_calverListRootFolder;

	CSocketThread * m_socketThread;
	CCommandHandlerThread * m_cmdHandlerThread;
/*
	MasterDataQueue<CMasterData>* m_pmasterDataQueue;
	FrontDataQueue<CFrontData> fq;
*/
	ULONG m_dataFileBufSize;

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
	MyTabCtrl m_tabMyTabCtrl;
	CButton* m_fileButton;
	
	
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CELISTestServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

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
	afx_msg void OnButtonStopLog();

	afx_msg VOID OnWorkModeUpdated(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnDirectionUpdated(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnDepthUpdated(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnSpeedUpdated(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnTimeUpdated(WPARAM wParam, LPARAM lParam);

	afx_msg VOID OnServerIP(WPARAM wParam, LPARAM lParam);
	

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELISTESTSERVERDLG_H__6AFE4040_494B_4411_87B4_38BD588A2CAA__INCLUDED_)

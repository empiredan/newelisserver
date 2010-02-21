#if !defined(AFX_COMMANDHANDLERTHREAD_H__DDE42665_4F89_4125_A8A5_8C47CEC34BDC__INCLUDED_)
#define AFX_COMMANDHANDLERTHREAD_H__DDE42665_4F89_4125_A8A5_8C47CEC34BDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommandHandlerThread.h : header file
//

#include "commands.h"
#include "Data.h"
#include "ACTList.h"
#include "DataFileBuffer.h"
//#include "MessageReceiverThread.h"
//#include "MessageSenderThread.h"

#define WM_SOCKET_THREAD_ID WM_USER+1
#define WM_COMMAND_DATA WM_USER+2
#define WM_DATABUF_LEN WM_USER+3
#define WM_ALL_ACT_DATAFILE_PATHS WM_USER+4
#define WM_ACT_DATAFILE_PATH WM_USER+5
#define WM_CALVER_DATAFILE_PATH WM_USER+6
#define WM_DEPTH_DATA_TIMER WM_USER+7
/////////////////////////////////////////////////////////////////////////////
// CCommandHandlerThread thread


typedef struct {
	int corr_Depth;
	int true_Depth;
	int speed;
	int totalTension;		//总张力
	int mmd;
	int differTension;		//差分张力
	UINT  time;					//时间
	int   nreserved2;			//保留
} DPM_DISPLAY_PARA;

class CCommandHandlerThread : public CWinThread
{
	DECLARE_DYNCREATE(CCommandHandlerThread)
protected:
	CCommandHandlerThread();           // protected constructor used by dynamic creation

// Attributes
public:

private:
	//Head of received data
	ULONG m_cmdType;
	ULONG m_totalLen;
	//Size of head
	ULONG m_headLen;
	//Body of received data	
	BUF_TYPE * m_bodyBuf;
	//Head of body
	ULONG m_bodyLen;

	ULONG m_bufferLen;
	//CString m_actRootPath;

	CACTList m_cACTList;
	DPM_DISPLAY_PARA m_dpmDisplayPara;
	CWorkMode m_cWorkMode;
	CDataFileBuffer m_cDataFileBuffer;

	long m_timeMS;
	long m_speedDUPM;
	long m_speedDUPS;
	long m_depthDU;
//	long m_trueDepthDU = 0;
//	long m_correctedDepthDU = 0;

	//Timer
	UINT m_subsetDataTimerIdentifier;
	UINT m_depthDataTimerIdentifier;

	//Related to thread
	DWORD m_socketThreadID;

	static void * m_pObject;
	
// Operations
public:
	void Init();
	static VOID CALLBACK OnTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	/*
	inline void SetSocketThreadID(DWORD tid){
		m_socketThreadID = tid;
	}*/
	inline void PreProcessMasterData(CMasterData *md);
	VOID CALLBACK SubsetDataTimerProc();
	VOID CALLBACK DepthDataTimerProc();
	void WorkModeProc();
	void NetCmd_InitServiceTable();
	void NetCmd_CalibPara();
	void NetCmd_CalibStart();
	//void NetCmd_CalibStop();
	void NetCmd_CtrlWorkState();
	//void NetCmd_SetStandbyTimeInterval();
	//void NetCmd_CtrlRecStop();
	//void NetCmd_CtrlActSwitch();
	void NetCmd_CtrlActDeactivate();
	
	//void NetCmd_InitConfigParameter();
	//void NetCmd_CtrlStandbytimeInterval();
	//void NetCmd_CtrlRecstop();
	//void NetCmd_CtrlACTSwitch();
	//void NetCmd_SnglCtl();
	//void NetCmd_DepthInternal();
	//void NetCmd_DepthDirection();
	void NetCmd_DepthSpeed();
	void NetCmd_TrueDepth();
	void NetCmd_CorrectedDepth();
	//void NetCmd_ManualDepthCorrection();
	//void NetCmd_AutoDepthCorrection();
	//void NetCmd_DepthLock();
	//void NetCmd_DepthUnlock();
	//void NetCmd_DepthTensionFactor();
	//void NetCmd_DepthTensionAngle();
	//void NetCmd_DepthCHT();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommandHandlerThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
public:
	

protected:
	virtual ~CCommandHandlerThread();

	// Generated message map functions
	//{{AFX_MSG(CCommandHandlerThread)
	afx_msg VOID OnSocketThreadID(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnDataBufLen(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnAllACTDataFilePaths(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnACTDataFilePath(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnCALVERDataFilePath(WPARAM wParam, LPARAM lParam);
	afx_msg VOID OnDepthDataTimer(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	

	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMANDHANDLERTHREAD_H__DDE42665_4F89_4125_A8A5_8C47CEC34BDC__INCLUDED_)

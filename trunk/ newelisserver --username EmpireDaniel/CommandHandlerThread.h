#if !defined(AFX_COMMANDHANDLERTHREAD_H__DDE42665_4F89_4125_A8A5_8C47CEC34BDC__INCLUDED_)
#define AFX_COMMANDHANDLERTHREAD_H__DDE42665_4F89_4125_A8A5_8C47CEC34BDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommandHandlerThread.h : header file
//

#define WM_COMMAND WM_USER+1
#define WM_DATABUF WM_USER+2
#define WM_ACTROOT WM_USER+3
#define WM_CALVERROOT WM_USER+4

/////////////////////////////////////////////////////////////////////////////
// CCommandHandlerThread thread

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

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommandHandlerThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
public:
	void PreProcessMasterData(CMasterData *md);
	void NetCmd_InitServiceTable();
	void NetCmd_CalibPara();
	void NetCmd_CalibStart();
	void NetCmd_CalibStop();
	void NetCmd_CtrlWorkState();
	void NetCmd_SetStandbyTimeInterval();
	void NetCmd_CtrlRecStop();
	void NetCmd_CtrlActSwitch();
	void NetCmd_CtrlActDeactivate();
	
	void NetCmd_InitConfigParameter();
	void NetCmd_CtrlStandbytimeInterval();
	void NetCmd_CtrlRecstop();
	void NetCmd_CtrlACTSwitch();
	void NetCmd_SnglCtl();
	void NetCmd_DepthInternal();
	void NetCmd_DepthDirection();
	void NetCmd_DepthSpeed();
	void NetCmd_TrueDepth();
	void NetCmd_CorrectedDepth();
	void NetCmd_ManualDepthCorrection();
	void NetCmd_AutoDepthCorrection();
	void NetCmd_DepthLock();
	void NetCmd_DepthUnlock();
	void NetCmd_DepthTensionFactor();
	void NetCmd_DepthTensionAngle();
	void NetCmd_DepthCHT();

protected:
	virtual ~CCommandHandlerThread();

	// Generated message map functions
	//{{AFX_MSG(CCommandHandlerThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg VOID OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMANDHANDLERTHREAD_H__DDE42665_4F89_4125_A8A5_8C47CEC34BDC__INCLUDED_)

// ELISTestServerDlg.h : header file
//

#if !defined(AFX_ELISTESTSERVERDLG_H__6AFE4040_494B_4411_87B4_38BD588A2CAA__INCLUDED_)
#define AFX_ELISTESTSERVERDLG_H__6AFE4040_494B_4411_87B4_38BD588A2CAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <windows.h>
//#include "MasterHeader.h"
#include "MyTabCtrl.h"
#include "MySocket.h"
#include "Data.h"
#include "Queue.h"
#include "MessageSender.h"
#include "commands.h"
#include "CommandHandler.h"

#include "Utils.h"
#include "ActTable.h"
#include "CalibParameter.h"
#include "WorkModeSetter.h"
#include "CalibSubset.h"
#include "DPMDisplayParameter.h"
#include "TimerDef.h"

#include "SubsetDataAssister.h"
#include "DataFileBuf.h"
//#include "DealThread.h"
//#include <deque>
//using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CELISTestServerDlg dialog
#define FLOAT_TO_STRING_FORMAT "%10.2f"

class CELISTestServerDlg : public CDialog
{
// Construction
public:
	void EnableActRootFolderSelection(BOOL enableButton);
	float m_depthDelta_toM;
	float m_depthDelta_toImp;
	float m_depthDelta_Consistency;
	long m_depthDUDelta;
	float m_timeDelta; //second
	void updateCorrectedDepth(float delta);
	void updateTrueDepth(float delta);
	void updateCurrentTime(float delta);
	//BOOL m_isCorrectedDepthReceived;
	//BOOL m_isTrueDepthReceived;
	//BOOL m_isSpeedReceived;
	void setCorrectedDepth(BUF_TYPE *buf, ULONG len);
	void setSpeed(BUF_TYPE *buf, ULONG len);
	void setTrueDepth(BUF_TYPE* buf, ULONG len);
	void fillDataFileBufWithAct();
	void fillDataFileBufWithCalVer();
	//UINT getCurrentDepthDU();
	//int getMeasure();
	CELISTestServerDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CELISTestServerDlg();

	UINT m_sPort;
	MySocket m_sListenSocket;
	MySocket* m_psConnectSocket;
	UINT m_rStatus;

	CString m_actListRootFolder;
	CString m_calverListRootFolder;

	ULONG m_bodyLen;
	ULONG m_msDataLen;
	ULONG m_rbuflen;
	BUF_TYPE *m_rbuf, *pBuf;
	ULONG rremain, received;


	MasterDataQueue<CMasterData>* m_pmasterDataQueue;
	//MasterDataQueue<CMasterData> mq;
	FrontDataQueue<CFrontData> fq;
	CCommandHandler cmdh;
	CMessageSender msgs;
	CActTable *acttab;
	CCalibParameter *calibpara;
	CWorkModeSetter *wms;
	unsigned char ta;

	CCalibSubset* calibsubset;

	ULONG m_dataFileBufSize;

    CString m_currentWorkStateStr;
	CString m_directionStr;

	long m_speedDU;// DU per minute
	float m_speed;// per minute
	CString m_speedStr;//per minute

	long m_trueDepthDU; //DU
	float m_trueDepth; //according to unit system
	CString m_trueDepthStr; //according to unit system

	long m_correctedDepthDU; //DU
	float m_correctedDepth; //according to unit system
	CString m_correctedDepthStr; //according to unit system

	//CString m_currentDepthStr;
	//long m_currentDepthDU;
	
	CString m_currentTimeStr;
	float m_currentTime; // second
	
	int m_measure;

	CSubsetDataAssister* m_subsetAssister;
	CDataFileBuf* m_dataFileBuf;

	CFile log;

	void OnReceive();
	void OnAccept();
	void OnClose();

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
	//void SetCurrentTestTime(float ct);
	//void SetCurrentDepth(float cp);
	//float GetCurrentDepth();
	//UINT GetCurrentTestTime();

	//void EnableStartLog(BOOL enableButton);
	void EnableCreateLog(BOOL enableButton);
	void EnableStopLog(BOOL enableButton);
	//void EnableUnitRadio(BOOL enableButton);
	//void EnableStartRelog(BOOL enableButton);

	void getCalibSubset();//CCalibSubset* 

	void HandleWorkStateChange();
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
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonServerPort();
	afx_msg void OnButtonCalverFolder();
	afx_msg void OnButtonDataBufferSize();
	//afx_msg void OnButtonSpeed();
	//afx_msg void OnButtonStartLog();
	//afx_msg void OnButtonTrueDepth();
	afx_msg void OnRadioImperial();
	afx_msg void OnRadioMetric();
	afx_msg void OnButtonCreateLog();
	afx_msg void OnButtonStopLog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELISTESTSERVERDLG_H__6AFE4040_494B_4411_87B4_38BD588A2CAA__INCLUDED_)

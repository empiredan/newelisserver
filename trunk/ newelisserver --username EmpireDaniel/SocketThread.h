#if !defined(AFX_SOCKETTHREAD_H__1B8E61AB_423C_4F34_8EAF_E2671A3221AB__INCLUDED_)
#define AFX_SOCKETTHREAD_H__1B8E61AB_423C_4F34_8EAF_E2671A3221AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SocketThread.h : header file
//

#include "MyListenSocket.h"

#define WM_PORT WM_USER+10
#define WM_SEND WM_USER+11

/////////////////////////////////////////////////////////////////////////////
// CSocketThread thread

class CSocketThread : public CWinThread
{
	DECLARE_DYNCREATE(CSocketThread)
protected:
	CSocketThread();           // protected constructor used by dynamic creation

// Attributes
public:

private:
	
	DWORD m_cmdThreadID;

	UINT m_socketPort;
	CMyListenSocket m_listenSocket;
	
// Operations
public:
	inline void SetCmdHandlerThreadID(DWORD tid){
		m_cmdThreadID = tid;
		m_listenSocket.SetCmdHandlerThreadID(tid);
	}
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSocketThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSocketThread();

	// Generated message map functions
	//{{AFX_MSG(CSocketThread)
		// NOTE - the ClassWizard will add and remove member functions here.
		afx_msg VOID OnPort(WPARAM wParam, LPARAM lParam);
		afx_msg VOID OnSend(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKETTHREAD_H__1B8E61AB_423C_4F34_8EAF_E2671A3221AB__INCLUDED_)

#if !defined(AFX_MESSAGERECEIVERTHREAD_H__0B70B739_FA3B_42A1_A5E1_59C53B68C678__INCLUDED_)
#define AFX_MESSAGERECEIVERTHREAD_H__0B70B739_FA3B_42A1_A5E1_59C53B68C678__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessageReceiverThread.h : header file
//

#include "MySocket.h"
#include "CommandHandlerThread.h"

#define WM_PORT WM_USER
#define WM_ACCEPT WM_USER
/////////////////////////////////////////////////////////////////////////////
// CMessageReceiverThread thread

class CMessageReceiverThread : public CWinThread
{
	DECLARE_DYNCREATE(CMessageReceiverThread)
protected:
	CMessageReceiverThread();           // protected constructor used by dynamic creation

// Attributes
public:
	CCommandHandlerThread * m_pCmdHandlerThread;

private:
	UINT m_socketPort;
	CMySocket m_listenSocket;
	//CMySocket * m_pConnectSocket;
	//UINT m_rcvStatus;

// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessageReceiverThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMessageReceiverThread();

	// Generated message map functions
	//{{AFX_MSG(CMessageReceiverThread)
		// NOTE - the ClassWizard will add and remove member functions here.
		afx_msg VOID OnPort(WPARAM wParam, LPARAM lParam);
		afx_msg VOID OnSocketAccept(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGERECEIVERTHREAD_H__0B70B739_FA3B_42A1_A5E1_59C53B68C678__INCLUDED_)

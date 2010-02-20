#if !defined(AFX_MYLISTENSOCKET_H__55E71FAE_51DF_438B_9BD0_DC395CC339AE__INCLUDED_)
#define AFX_MYLISTENSOCKET_H__55E71FAE_51DF_438B_9BD0_DC395CC339AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyListenSocket.h : header file
//

#include "MyConnectSocket.h"
//#include "CommandHandlerThread.h"

/////////////////////////////////////////////////////////////////////////////
// CMyListenSocket command target
class CSocketThread;
class CMyListenSocket : public CAsyncSocket
{
// Attributes
public:

private:
	//CCommandHandlerThread * m_cmdHandlerThread;
	CSocketThread * m_socketThread;

	DWORD m_cmdThreadID;

//public:
	CMyConnectSocket m_connectSocket;

// Operations
public:
	CMyListenSocket();
	virtual ~CMyListenSocket();
	inline void SetSocketThread(CSocketThread* st){
		m_socketThread = st;
	}
	inline void SetCmdHandlerThreadID(DWORD tid){
		//m_connectSocket.SetCmdHandlerThread(cmdHThread);
		m_cmdThreadID = tid;
		m_connectSocket.SetCmdHandlerThreadID(tid);
	}
	
	inline CMyConnectSocket * GetConnectSocket(){
		return &m_connectSocket;
	}

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyListenSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CMyListenSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLISTENSOCKET_H__55E71FAE_51DF_438B_9BD0_DC395CC339AE__INCLUDED_)

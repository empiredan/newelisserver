#if !defined(AFX_MYCONNECTSOCKET_H__484118C7_E6F8_4611_8C9E_97E9D229603B__INCLUDED_)
#define AFX_MYCONNECTSOCKET_H__484118C7_E6F8_4611_8C9E_97E9D229603B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyConnectSocket.h : header file
//
//#include <windows.h>
//#include "CommandHandlerThread.h"
#include "commands.h"

//using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CMyConnectSocket command target

class CMyConnectSocket : public CAsyncSocket
{
// Attributes
public:
private:

	//CCommandHandlerThread * m_cmdHandlerThread;
	DWORD m_cmdThreadID;

	//Receiving status
	UINT m_rcvStatus;
	
	ULONG m_receivedLen;
	
	BUF_TYPE m_headBuf[SOCK_RECEIVE_HEADER_LEN+10];
	
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
	CMyConnectSocket();
	CMyConnectSocket(const CMyConnectSocket& connectSocket){

		m_cmdThreadID = connectSocket.m_cmdThreadID;
		m_rcvStatus = connectSocket.m_rcvStatus;
		m_receivedLen = connectSocket.m_receivedLen;
		memcpy(m_headBuf, connectSocket.m_headBuf, SOCK_RECEIVE_HEADER_LEN+10);
		m_cmdType = connectSocket.m_cmdType;
		m_totalLen = connectSocket.m_totalLen;
		m_headLen = connectSocket.m_headLen;
		m_bodyBuf = connectSocket.m_bodyBuf;
		m_bodyLen = connectSocket.m_bodyLen;

	}
	virtual ~CMyConnectSocket();
	inline void SetCmdHandlerThreadID(DWORD tid){
		//m_cmdHandlerThread = cmdHThread;
		m_cmdThreadID = tid;
	}

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyConnectSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CMyConnectSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCONNECTSOCKET_H__484118C7_E6F8_4611_8C9E_97E9D229603B__INCLUDED_)

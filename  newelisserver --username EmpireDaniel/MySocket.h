#if !defined(AFX_MYSOCKET_H__B2852606_6492_4CA2_BB0B_48F6A5B7E1A5__INCLUDED_)
#define AFX_MYSOCKET_H__B2852606_6492_4CA2_BB0B_48F6A5B7E1A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySocket.h : header file
//

#include "Data.h"
#include "CommandHandlerThread.h"

#define SOCK_RECEIVE_HEADER 0x0001
#define SOCK_RECEIVE_BODY 0x0002

/////////////////////////////////////////////////////////////////////////////
// MySocket command target
class CELISTestServerDlg;
class CMySocket : public CAsyncSocket
{
// Attributes
public:
	CCommandHandlerThread * m_pCmdHandlerThread;
private:
	//The pointer of connection socket
	CAsyncSocket * m_pConnectSocket;

	//Receiving status
	UINT m_rcvStatus = SOCK_RECEIVE_HEADER;

	ULONG m_receivedLen = 0;

	BUF_TYPE m_headBuf[SOCK_RECEIVE_HEADER_LEN+10];

	//Head of received data
	ULONG m_cmdType;
	ULONG m_totalLen;
	//Size of head
	ULONG m_headLen = SOCK_RECEIVE_HEADER_LEN;
	//Body of received data	
	BUF_TYPE * m_bodyBuf;
	//Head of body
	ULONG m_bodyLen;

// Operations
public:
	CMySocket();
	virtual ~CMySocket();
	
// Overrides
public:
	//void SetParent(CELISTestServerDlg* pDlg);
	//CELISTestServerDlg* m_pELISTestServerDlg;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MySocket)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(MySocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:

	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSOCKET_H__B2852606_6492_4CA2_BB0B_48F6A5B7E1A5__INCLUDED_)

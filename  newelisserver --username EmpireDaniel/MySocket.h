#if !defined(AFX_MYSOCKET_H__B2852606_6492_4CA2_BB0B_48F6A5B7E1A5__INCLUDED_)
#define AFX_MYSOCKET_H__B2852606_6492_4CA2_BB0B_48F6A5B7E1A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySocket.h : header file
//

#include "Data.h"

#define SOCK_RECEIVE_HEADER 0x0001
#define SOCK_RECEIVE_BODY 0x0002

/////////////////////////////////////////////////////////////////////////////
// MySocket command target
class CELISTestServerDlg;
class MySocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	MySocket();
	virtual ~MySocket();

// Overrides
public:
	void SetParent(CELISTestServerDlg* pDlg);
	CELISTestServerDlg* m_pELISTestServerDlg;
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
private:
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSOCKET_H__B2852606_6492_4CA2_BB0B_48F6A5B7E1A5__INCLUDED_)

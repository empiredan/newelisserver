#if !defined(AFX_MESSAGESENDERTHREAD_H__0A733DBB_BA9C_46AF_9138_4525B07A7D4B__INCLUDED_)
#define AFX_MESSAGESENDERTHREAD_H__0A733DBB_BA9C_46AF_9138_4525B07A7D4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessageSenderThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CMessageSenderThread thread

class CMessageSenderThread : public CWinThread
{
	DECLARE_DYNCREATE(CMessageSenderThread)
protected:
	CMessageSenderThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessageSenderThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMessageSenderThread();

	// Generated message map functions
	//{{AFX_MSG(CMessageSenderThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGESENDERTHREAD_H__0A733DBB_BA9C_46AF_9138_4525B07A7D4B__INCLUDED_)

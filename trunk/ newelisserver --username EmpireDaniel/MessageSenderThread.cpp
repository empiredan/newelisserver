// MessageSenderThread.cpp : implementation file
//

#include "stdafx.h"
#include "elistestserver.h"
#include "MessageSenderThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessageSenderThread

IMPLEMENT_DYNCREATE(CMessageSenderThread, CWinThread)

CMessageSenderThread::CMessageSenderThread()
{
}

CMessageSenderThread::~CMessageSenderThread()
{
}

BOOL CMessageSenderThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CMessageSenderThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMessageSenderThread, CWinThread)
	//{{AFX_MSG_MAP(CMessageSenderThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageSenderThread message handlers

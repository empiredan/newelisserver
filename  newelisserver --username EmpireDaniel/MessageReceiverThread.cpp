// MessageReceiverThread.cpp : implementation file
//

#include "stdafx.h"
#include "elistestserver.h"
#include "MessageReceiverThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessageReceiverThread

IMPLEMENT_DYNCREATE(CMessageReceiverThread, CWinThread)

CMessageReceiverThread::CMessageReceiverThread()
{
}

CMessageReceiverThread::~CMessageReceiverThread()
{
}

BOOL CMessageReceiverThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CMessageReceiverThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMessageReceiverThread, CWinThread)
	//{{AFX_MSG_MAP(CMessageReceiverThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageReceiverThread message handlers

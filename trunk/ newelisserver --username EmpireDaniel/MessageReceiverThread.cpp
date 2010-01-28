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
		ON_THREAD_MESSAGE(WM_PORT, OnPort)
		ON_THREAD_MESSAGE(WM_ACCEPT, OnSocketAccept)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageReceiverThread message handlers
VOID CMessageReceiverThread::OnPort(WPARAM wParam, LPARAM lParam)
{
	m_socketPort = (UINT)lParam;
	
	if(!m_listenSocket.Create(m_socketPort)) {
		AfxMessageBox(_T("Socket Error!"));
		m_listenSocket.Close();
	}else{
		
		m_listenSocket.m_pCmdHandlerThread = m_pCmdHandlerThread;

		CString serverIP;
		CString serverPort;
		serverPort.Format("%d",m_socketPort);
		
		char   name[256];  
		hostent*   pHost;  
		gethostname(name,   128);//获得主机名  
		pHost   =   gethostbyname(name);//获得主机结构  
		serverIP   =   inet_ntoa(*((in_addr   *)pHost->h_addr));
		
		
	}
	
	if(!m_listenSocket.Listen()){
		int nErrorCode = m_listenSocket.GetLastError();
		if (nErrorCode != WSAEWOULDBLOCK) {
			m_listenSocket.Close(); 
		}
	}
}

VOID CMessageReceiverThread::OnSocketAccept(WPARAM wParam, LPARAM lParam)
{

}
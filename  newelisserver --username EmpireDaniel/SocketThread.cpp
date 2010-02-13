// SocketThread.cpp : implementation file
//

#include "stdafx.h"
#include "elistestserver.h"
#include "SocketThread.h"

#include "Data.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSocketThread

IMPLEMENT_DYNCREATE(CSocketThread, CWinThread)

CSocketThread::CSocketThread()
{
}

CSocketThread::~CSocketThread()
{
}

BOOL CSocketThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CSocketThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSocketThread, CWinThread)
	//{{AFX_MSG_MAP(CSocketThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		ON_THREAD_MESSAGE(WM_PORT, OnPort)
		ON_THREAD_MESSAGE(WM_SEND, OnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSocketThread message handlers
VOID CSocketThread::OnPort(WPARAM wParam, LPARAM lParam)
{
	m_socketPort = (UINT)lParam;
	
	if(!m_listenSocket.Create(m_socketPort)) {
		AfxMessageBox(_T("Socket Error!"));
		m_listenSocket.Close();
	}else{
		
		//m_listenSocket.m_pCmdHandlerThread = m_pCmdHandlerThread;
		
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

VOID CSocketThread::OnSend(WPARAM wParam, LPARAM lParam)
{
	CFrontData * frontData = (CFrontData *)lParam;
	m_listenSocket.GetConnectSocket().Send(frontData->GetTotalBuf(), frontData->GetTotalLen(), 0);
	delete frontData;
}

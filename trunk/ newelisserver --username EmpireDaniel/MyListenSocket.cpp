// MyListenSocket.cpp : implementation file
//

#include "stdafx.h"
#include "elistestserver.h"
#include "MyListenSocket.h"

#include "SocketThread.h"
#include "ELISTestServerDlg.h"
#include "CommandHandlerThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyListenSocket

CMyListenSocket::CMyListenSocket()
{
}

CMyListenSocket::~CMyListenSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CMyListenSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CMyListenSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CMyListenSocket member functions

void CMyListenSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(nErrorCode == 0){
		
		if(!this->Accept(m_connectSocket)){
			char t[50];
			int e = GetLastError();
			sprintf(t, "ServerSocket Accepted Error Code:%d", e);
			AfxMessageBox(_T(t), MB_YESNO, 0);
			m_connectSocket.Close();
		} else {
			CString clientIP;
			UINT clientPort;
			
			m_connectSocket.GetPeerName(clientIP,clientPort);
			
			//CString strClientPort;
			//strClientPort.Format("%d",clientPort);

			::SendMessage(m_socketThread->GetMainWnd()->GetSafeHwnd(), WM_CLIENT_IP_PORT, (WPARAM)clientPort, (LPARAM)&clientIP);
			::PostThreadMessage(m_cmdThreadID, WM_SET_DEPTH_DATA_TIMER, NULL, NULL);
			
		}
	}
	CAsyncSocket::OnAccept(nErrorCode);
}

void CMyListenSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CAsyncSocket::OnClose(nErrorCode);
}

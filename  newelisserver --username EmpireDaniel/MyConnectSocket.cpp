// MyConnectSocket.cpp : implementation file
//

#include "stdafx.h"
#include "elistestserver.h"
#include "MyConnectSocket.h"

#include "CommandHandlerThread.h"
#include "SocketThread.h"
#include "ELISTestServerDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyConnectSocket

CMyConnectSocket::CMyConnectSocket()
{
	m_rcvStatus = SOCK_RECEIVE_HEADER;
	
	m_receivedLen = 0;

	m_cmdType = 0;
	m_totalLen = SOCK_RECEIVE_HEADER_LEN;

	//m_headBuf = NULL;
	m_headLen = SOCK_RECEIVE_HEADER_LEN;

	m_bodyBuf = NULL;
	m_bodyLen = 0;
}

CMyConnectSocket::~CMyConnectSocket()
{
	if (m_bodyBuf)
	{
		delete [] m_bodyBuf;
		m_bodyBuf = NULL;
	}
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CMyConnectSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CMyConnectSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CMyConnectSocket member functions

void CMyConnectSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(nErrorCode == 0){
		
		if (m_rcvStatus == SOCK_RECEIVE_HEADER) {
			
			ULONG rcvLen = Receive(m_headBuf+m_receivedLen, m_headLen-m_receivedLen, 0);
			//解析header，确定body长度
			if(rcvLen != SOCKET_ERROR && rcvLen > 0) {
				
				m_receivedLen += rcvLen;
				
				if(m_receivedLen == m_headLen) {
					
					ULONG *head;
					head= (ULONG*)m_headBuf;
					m_cmdType = ntohl(head[0]);
					m_totalLen = ntohl(head[1]);
					m_bodyLen = m_totalLen - m_headLen;
					if (m_bodyBuf)
					{
						delete [] m_bodyBuf;
						m_bodyBuf = NULL;
					}
					m_bodyBuf = new BUF_TYPE[m_bodyLen+10];
					m_rcvStatus = SOCK_RECEIVE_BODY;
					m_receivedLen = 0;
					
				} 
				
			} 
			
		} else if(m_rcvStatus == SOCK_RECEIVE_BODY) {
			
			ULONG rcvLen = Receive(m_bodyBuf+m_receivedLen, m_bodyLen-m_receivedLen, 0);
			
			if(rcvLen != SOCKET_ERROR && rcvLen > 0) {
				
				m_receivedLen += rcvLen;
				
				if(m_receivedLen == m_bodyLen) {
					
					CMasterData * mData=new CMasterData(m_cmdType, m_totalLen, m_headLen, m_bodyBuf, m_bodyLen);
					::PostThreadMessage(m_cmdThreadID, WM_COMMAND_DATA, NULL, (LPARAM)mData);
					m_rcvStatus = SOCK_RECEIVE_HEADER;
					m_receivedLen = 0;
				} 
				
			} 
			
		} else {
			AfxMessageBox(_T("OnReceive wrong status"));
			//m_rStatus = SOCK_RECEIVE_HEADER;
		}
	}

	CAsyncSocket::OnReceive(nErrorCode);
}

void CMyConnectSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	//AfxMessageBox(_T("Hello!"));
	::SendMessage((HWND)(m_socketThread->GetMainWnd()->GetSafeHwnd()), WM_ENABLE_SERVER_PORT_DISCONNECTION, NULL, (LPARAM)FALSE);
	this->Close();
	CAsyncSocket::OnClose(nErrorCode);
}

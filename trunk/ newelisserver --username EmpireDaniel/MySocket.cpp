// MySocket.cpp : implementation file
//

#include "stdafx.h"
#include "ELISTestServer.h"
#include "ELISTestServerDlg.h"
#include "MySocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MySocket

CMySocket::CMySocket()
{
	//this->m_pELISTestServerDlg=NULL;
	//m_pCmdHandlerThread = cmdHandler;
}

CMySocket::~CMySocket()
{
	//this->m_pELISTestServerDlg=NULL;
	if (m_pConnectSocket){
		delete m_pConnectSocket;
		m_pConnectSocket = NULL;
	}
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CMySocket, CAsyncSocket)
	//{{AFX_MSG_MAP(MySocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// MySocket member functions
/*
void CMySocket::SetParent(CELISTestServerDlg* pDlg)
{
	m_pELISTestServerDlg=pDlg;
}
*/
void CMySocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(nErrorCode == 0){
		m_pConnectSocket = new CMySocket;
		
		if(!this->Accept(m_pConnectSocket)){
			char t[50];
			int e = GetLastError();
			sprintf(t, "ServerSocket Accepted Error Code:%d", e);
			AfxMessageBox(_T(t), MB_YESNO, 0);
			m_pConnectSocket.Close();
		} else {
			CString clientIP;
			UINT clientPort;
			
			m_pConnectSocket->GetPeerName(clientIP,clientPort);
			
			CString strClientPort;
			strClientPort.Format("%d",clientPort);
		
		}
	}
	//	this->m_pELISTestServerDlg->OnAccept();
	CAsyncSocket::OnAccept(nErrorCode);
}

void CMySocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(nErrorCode == 0){
		
		if (m_rcvStatus == SOCK_RECEIVE_HEADER) {

			ULONG rcvLen = m_pConnectSocket->Receive(m_headBuf+m_receivedLen, m_headLen-m_receivedLen, 0);
			//解析header，确定body长度
			if(rcvLen != SOCKET_ERROR && rcvLen > 0) {

				m_receivedLen += rcvLen;

				if(m_receivedLen == m_headLen) {

					ULONG *head;
					head= (ULONG*)m_headBuf;
					m_cmdType = ntohl(head[0]);
					m_totalLen = ntohl(head[1]);
					m_bodyLen = m_totalLen - m_headLen;
					
					m_rcvStatus = SOCK_RECEIVE_BODY;
					m_receivedLen = 0;
				
				} 

			} 

		} else if(m_rcvStatus == SOCK_RECEIVE_BODY) {

			ULONG rcvLen = m_pConnectSocket->Receive(m_bodyBuf+m_receivedLen, m_bodyLen-m_receivedLen, 0);
			
			if(rcvLen != SOCKET_ERROR && rcvLen > 0) {

				m_receivedLen += rcvLen;

				if(m_receivedLen == m_bodyLen) {
					
					CMasterData * mData=new CMasterData(m_cmdType, m_totalLen, m_headLen, m_bodyBuf, m_bodyLen);
					::PostThreadMessage(m_pCmdHandlerThread->m_nThreadID, WM_COMMAND_DATA, NULL, mData);
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

void CMySocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(nErrorCode==0)
	{
		if (m_pConnectSocket){
			delete m_pConnectSocket;
			m_pConnectSocket = NULL;
		}
	}
		//this->Close();
		//this->m_pELISTestServerDlg->OnClose();
	CAsyncSocket::OnClose(nErrorCode);
}

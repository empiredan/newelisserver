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
	if(nErrorCode==0){
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
	if(nErrorCode==0){
		ULONG *t;
		ULONG len;
		ULONG cmdType;
		
		if (m_rcvStatus == SOCK_RECEIVE_HEADER) {
			len = m_pConnectSocket->Receive(pBuf, rremain, 0);
			//解析header，确定body长度
			if(len != SOCKET_ERROR && len > 0) {
				received += len;
				if(received == SOCK_RECEIVE_HEADER_LEN) {
					t = (ULONG*)m_rbuf;
					cmdType = ntohl(t[0]);
					//t = (int*)m_rbuf;	
					m_msDataLen = ntohl(t[1]);
					m_bodyLen = m_msDataLen - SOCK_RECEIVE_HEADER_LEN;
					
					if(m_bodyLen <= 0) {
						AfxMessageBox(_T("OnReceive Strange Err: message body have 0 length"));
					} else {
						if((m_bodyLen + SOCK_RECEIVE_HEADER_LEN) > m_rbuflen) {
							BUF_TYPE *bft;
							bft = new BUF_TYPE[m_bodyLen + SOCK_RECEIVE_HEADER_LEN];
							
							ASSERT(bft != NULL);
							
							memcpy(bft, m_rbuf, SOCK_RECEIVE_HEADER_LEN);
							delete []m_rbuf;
							m_rbuf = bft;
							m_rbuflen = m_bodyLen + SOCK_RECEIVE_HEADER_LEN;
							received = 0;
							pBuf = m_rbuf;
							rremain = m_bodyLen;
						}
						m_rStatus = SOCK_RECEIVE_BODY;
						rremain = m_bodyLen;
						received = 0;
						pBuf = m_rbuf + SOCK_RECEIVE_HEADER_LEN;
					}
				} else {
					rremain = SOCK_RECEIVE_HEADER_LEN - len;
					pBuf = pBuf + len;
				}
			} 
		} else if(m_rStatus == SOCK_RECEIVE_BODY) {
			len = m_psConnectSocket->Receive(pBuf, rremain, 0);
			
			if(len != SOCKET_ERROR && len > 0) {
				received += len;
				if(received == m_bodyLen) {
					//把接收到的rbuf填入ReceiverQueue中
					CMasterData* p_msData=new CMasterData();
					p_msData->setData(m_rbuf, m_bodyLen + SOCK_RECEIVE_HEADER_LEN);
					m_pmasterDataQueue->enQueue(p_msData);
					
					m_rStatus = SOCK_RECEIVE_HEADER;
					pBuf = m_rbuf;
					rremain = SOCK_RECEIVE_HEADER_LEN;
					received = 0;
				} else if(received < m_bodyLen) {
					rremain = m_bodyLen - received;
					pBuf += len;
				} else {
					AfxMessageBox(_T("Recceive Body should no occur"));
				}
			} else {
				if(len <= 0) {
					AfxMessageBox(_T("OnReceive body length received <=0"));
				} else {
					AfxMessageBox(_T("OnReceive Socket error"));
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

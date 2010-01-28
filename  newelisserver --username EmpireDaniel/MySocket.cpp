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

MySocket::MySocket()
{
	this->m_pELISTestServerDlg=NULL;
}

MySocket::~MySocket()
{
	this->m_pELISTestServerDlg=NULL;
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(MySocket, CAsyncSocket)
	//{{AFX_MSG_MAP(MySocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// MySocket member functions
void MySocket::SetParent(CELISTestServerDlg* pDlg)
{
	m_pELISTestServerDlg=pDlg;
}

void MySocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(nErrorCode==0)
		this->m_pELISTestServerDlg->OnAccept();
	CAsyncSocket::OnAccept(nErrorCode);
}

void MySocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(nErrorCode==0)
		this->m_pELISTestServerDlg->OnReceive();
	CAsyncSocket::OnReceive(nErrorCode);
}

void MySocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	//if(nErrorCode==0)
		//this->Close();
		//this->m_pELISTestServerDlg->OnClose();
	CAsyncSocket::OnClose(nErrorCode);
}

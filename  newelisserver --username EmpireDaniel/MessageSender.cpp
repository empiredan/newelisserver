#include "stdafx.h"
#include "Data.h"
#include "Queue.h"
#include "MessageSender.h"
#include "ELISTestServer.h"
#include "ELISTestServerDlg.h"

//class CELISTestServerDlg;
CMessageSender::CMessageSender()
{
	this->dlg = dlg;
	finish = FALSE;
}
void CMessageSender::start() {
	worker = CreateThread(NULL, 
											0,
											&CMessageSender::handle,
											this,
											0,			//start immediately after create
											&wid);
}

CMessageSender::~CMessageSender(void)
{
}

DWORD CMessageSender::handle(LPVOID param) {
	CMessageSender *handler = (CMessageSender *)param;
	CFrontData *d;
	FrontDataQueue<CFrontData> *q;
	int err;
	//
	//
	
	q = handler->dlg->getFrontDataQueue();
	//AfxMessageBox(_T("MessageSenderStarted:::"));
	while(!handler->finish) {
		d = q->deQueue();
		if( d == NULL || d->buflen <= 0) {
			continue;
		}
		//
		//t = (int*)(d->buf+SOCK_RECEIVE_HEADER_LEN);
		//sprintf(oo, "corr_Depth:%d,true_Depth:%d", t[0], t[1]);
		//AfxMessageBox(_T(oo));
		
		err = handler->dlg->m_psConnectSocket->Send(d->buf, d->contentlen);
		//ULONG *t;
		//char oo[1024];
		if(err == SOCKET_ERROR) {
			//AfxMessageBox(_T("Sending Messages Socket error"));
			int err1 = WSAGetLastError();
			if(err1 == WSAESHUTDOWN || err1 == WSAECONNRESET) {
				;
			}
			delete d;
			//t = (ULONG*)d->buf;
			//sprintf(oo, "Send discard cmdType:%lx\n",t[0]);
			//handler->dlg->log.Write(oo, strlen(oo));
			//handler->dlg->log.Flush();
			continue;
		} else {
			//t = (ULONG*)d->buf;
			//sprintf(oo, "Really Send cmdType:%lx\n",t[0]);
			//handler->dlg->log.Write(oo, strlen(oo));
			//handler->dlg->log.Flush();
		}
		//����Ҳ��������ɾ��CFrontData���͵Ķ���
		//���CFrontData���Ͷ���������ɶ�ʱ��������
		//CommandHendler���߳��ӵ������еģ�����Щ
		//��ʱ�������߳������ͨ��new�İ취����
		//��Щ����ģ���
		delete d;
	}

	return 0;
}


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
		//这里也不能忘记删除CFrontData类型的对象
		//这个CFrontData类型对象可能是由定时器，或者
		//CommandHendler等线程扔到队列中的，在那些
		//定时器或者线程里，都是通过new的办法创建
		//这些对象的！！
		delete d;
	}

	return 0;
}


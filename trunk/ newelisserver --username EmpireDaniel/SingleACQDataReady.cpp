#include "stdafx.h"
#include "SingleACQDataReady.h"

CSingleACQDataReady::CSingleACQDataReady(void)
{
	//�˴��������single act data ready body���ȵĴ���Ҫ����
	//�յ��������������ĽṹCActTable������
	//bodyLen = sizeof();
	AfxMessageBox(_T("�˴�������� single act data ready body���ȵĴ���Ҫ�����յ��������������ĽṹCActTable������"));
	cmdLen = bodyLen + SOCK_RECEIVE_HEADER_LEN;
	cmdType = NET_RETURN_SNGLACQ_DATAREADY;
}
CSingleACQDataReady::CSingleACQDataReady(BUF_TYPE* bf, ULONG len):CFrontData(bf, len) {
	//�˴��������single act data ready body���ȵĴ���Ҫ����
	//�յ��������������ĽṹCActTable������
	//bodyLen = sizeof();
	AfxMessageBox(_T("�˴�������� single act data ready body���ȵĴ���Ҫ�����յ��������������ĽṹCActTable������"));
	cmdLen = bodyLen + SOCK_RECEIVE_HEADER_LEN;
	cmdType = NET_RETURN_SNGLACQ_DATAREADY;
}

CSingleACQDataReady::~CSingleACQDataReady(void)
{
}

#include "stdafx.h"
#include "UpholeSendDataReady.h"

CUpholeSendDataReady::CUpholeSendDataReady(void)
{
	//�˴��������upholesend data ready body���ȵĴ���Ҫ����
	//�յ��������������ĽṹCActTable������
	bodyLen = sizeof(ULONG);
	//AfxMessageBox(_T("�˴��������upholesend data ready body���ȵĴ���Ҫ�����յ��������������ĽṹCActTable������"));
	cmdLen = bodyLen + SOCK_RECEIVE_HEADER_LEN;
	cmdType = NET_RETURN_UPHOLESEND_DATAREADY;
}
CUpholeSendDataReady::CUpholeSendDataReady(BUF_TYPE* bf, ULONG len):CFrontData(bf, len) {
	//�˴��������upholesend data ready body���ȵĴ���Ҫ����
	//�յ��������������ĽṹCActTable������
	bodyLen = sizeof(ULONG);
	//AfxMessageBox(_T("�˴��������upholesend data ready body���ȵĴ���Ҫ�����յ��������������ĽṹCActTable������"));
	cmdLen = bodyLen + SOCK_RECEIVE_HEADER_LEN;
	cmdType = NET_RETURN_UPHOLESEND_DATAREADY;
}

CUpholeSendDataReady::~CUpholeSendDataReady(void)
{
}

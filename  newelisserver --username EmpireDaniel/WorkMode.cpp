#include "stdafx.h"
#include "WorkMode.h"

CWorkMode::CWorkMode(void)
{
	//�˴��������work mode body���ȵĴ���Ҫ����
	//�յ��������������ĽṹCActTable������
	bodyLen = sizeof(UINT32);
	//AfxMessageBox(_T("�˴��������workmode body���ȵĴ���Ҫ�����յ��������������ĽṹCActTable������"));
	cmdLen = bodyLen + SOCK_RECEIVE_HEADER_LEN;
	cmdType = NET_RETURN_WORKMODE;
}
CWorkMode::CWorkMode(BUF_TYPE* bf, ULONG len):CFrontData(bf, len)
{
	//�˴��������work mode body���ȵĴ���Ҫ����
	//�յ��������������ĽṹCActTable������
	//AfxMessageBox(_T("�˴��������workmode body���ȵĴ���Ҫ�����յ��������������ĽṹCActTable������"));
	bodyLen = sizeof(UINT32);
	cmdLen = bodyLen + SOCK_RECEIVE_HEADER_LEN;
	cmdType = NET_RETURN_WORKMODE;
}

void CWorkMode::setData(BUF_TYPE *bf, ULONG len) {
	setHeader(cmdType, cmdLen);
	
	CData::setData(bf, len);
}

CWorkMode::~CWorkMode(void)
{
}

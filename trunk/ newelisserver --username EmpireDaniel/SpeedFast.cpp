#include "stdafx.h"
#include "SpeedFast.h"

CSpeedFast::CSpeedFast(void)
{
	//�˴��������speed fast body���ȵĴ���Ҫ����
	//�յ��������������ĽṹCActTable������
	//bodyLen = sizeof();
	AfxMessageBox(_T("�˴��������speed fast body���ȵĴ���Ҫ�����յ��������������ĽṹCActTable������"));
	cmdLen = bodyLen + SOCK_RECEIVE_HEADER_LEN;
	cmdType = NET_RETURN_SPEEDFAST;
}
CSpeedFast::CSpeedFast(BUF_TYPE* bf, ULONG len):CFrontData(bf, len) {
	//�˴��������speed fast body���ȵĴ���Ҫ����
	//�յ��������������ĽṹCActTable������
	//bodyLen = sizeof();
	AfxMessageBox(_T("�˴��������speed fast body���ȵĴ���Ҫ�����յ��������������ĽṹCActTable������"));
	cmdLen = bodyLen + SOCK_RECEIVE_HEADER_LEN;
	cmdType = NET_RETURN_SPEEDFAST;
}

CSpeedFast::~CSpeedFast(void)
{
}

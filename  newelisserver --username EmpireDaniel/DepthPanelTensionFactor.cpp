#include "stdafx.h"
#include "DepthPanelTensionFactor.h"

CDepthPanelTensionFactor::CDepthPanelTensionFactor(void)
{
	//�˴��������tension factor body���ȵĴ���Ҫ����
	//�յ��������������ĽṹCActTable������
	//bodyLen = sizeof();
	AfxMessageBox(_T("�˴��������tension factor body���ȵĴ���Ҫ�����յ��������������ĽṹCActTable������"));
	cmdLen = bodyLen + SOCK_RECEIVE_HEADER_LEN;
	cmdType = NET_RETURN_DEPTHPANEL_READTENSIONFACTOR;
}

CDepthPanelTensionFactor::CDepthPanelTensionFactor(BUF_TYPE* bf, ULONG len):CFrontData(bf, len) 
{
	//�˴��������tension factor body���ȵĴ���Ҫ����
	//�յ��������������ĽṹCActTable������
	//bodyLen = sizeof();
	AfxMessageBox(_T("�˴��������tension factor body���ȵĴ���Ҫ�����յ��������������ĽṹCActTable������"));
	cmdLen = bodyLen + SOCK_RECEIVE_HEADER_LEN;
	cmdType = NET_RETURN_DEPTHPANEL_READTENSIONFACTOR;
}


CDepthPanelTensionFactor::~CDepthPanelTensionFactor(void)
{
}

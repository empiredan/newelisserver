#include "stdafx.h"
#include "DepthPanelReadTensionAngle.h"

CDepthPanelReadTensionAngle::CDepthPanelReadTensionAngle(void)
{
	//�˴��������tension angle body���ȵĴ���Ҫ����
	//�յ��������������ĽṹCActTable������
	//bodyLen = sizeof();
	AfxMessageBox(_T("�˴��������tension angle body���ȵĴ���Ҫ�����յ��������������ĽṹCActTable������"));
	cmdLen = bodyLen + SOCK_RECEIVE_HEADER_LEN;
	cmdType = NET_RETURN_DEPTHPANEL_READTENSIONANGLE;
}
CDepthPanelReadTensionAngle::CDepthPanelReadTensionAngle(BUF_TYPE* bf, ULONG len):CFrontData(bf, len) {
	//�˴��������tension angle body���ȵĴ���Ҫ����
	//�յ��������������ĽṹCActTable������
	//bodyLen = sizeof();
	AfxMessageBox(_T("�˴��������tension angle body���ȵĴ���Ҫ�����յ��������������ĽṹCActTable������"));
	cmdLen = bodyLen + SOCK_RECEIVE_HEADER_LEN;
	cmdType = NET_RETURN_DEPTHPANEL_READTENSIONANGLE;
}

CDepthPanelReadTensionAngle::~CDepthPanelReadTensionAngle(void)
{
}

#include "stdafx.h"
#include "DepthPanelReadTensionAngle.h"

CDepthPanelReadTensionAngle::CDepthPanelReadTensionAngle(void)
{
	//此处加入计算tension angle body长度的代码要根据
	//收到的命令所解析的结构CActTable来计算
	//bodyLen = sizeof();
	AfxMessageBox(_T("此处加入计算tension angle body长度的代码要根据收到的命令所解析的结构CActTable来计算"));
	cmdLen = bodyLen + SOCK_RECEIVE_HEADER_LEN;
	cmdType = NET_RETURN_DEPTHPANEL_READTENSIONANGLE;
}
CDepthPanelReadTensionAngle::CDepthPanelReadTensionAngle(BUF_TYPE* bf, ULONG len):CFrontData(bf, len) {
	//此处加入计算tension angle body长度的代码要根据
	//收到的命令所解析的结构CActTable来计算
	//bodyLen = sizeof();
	AfxMessageBox(_T("此处加入计算tension angle body长度的代码要根据收到的命令所解析的结构CActTable来计算"));
	cmdLen = bodyLen + SOCK_RECEIVE_HEADER_LEN;
	cmdType = NET_RETURN_DEPTHPANEL_READTENSIONANGLE;
}

CDepthPanelReadTensionAngle::~CDepthPanelReadTensionAngle(void)
{
}

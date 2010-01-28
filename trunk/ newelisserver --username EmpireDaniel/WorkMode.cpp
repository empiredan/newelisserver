#include "stdafx.h"
#include "WorkMode.h"

CWorkMode::CWorkMode(void)
{
	//此处加入计算work mode body长度的代码要根据
	//收到的命令所解析的结构CActTable来计算
	bodyLen = sizeof(UINT32);
	//AfxMessageBox(_T("此处加入计算workmode body长度的代码要根据收到的命令所解析的结构CActTable来计算"));
	cmdLen = bodyLen + SOCK_RECEIVE_HEADER_LEN;
	cmdType = NET_RETURN_WORKMODE;
}
CWorkMode::CWorkMode(BUF_TYPE* bf, ULONG len):CFrontData(bf, len)
{
	//此处加入计算work mode body长度的代码要根据
	//收到的命令所解析的结构CActTable来计算
	//AfxMessageBox(_T("此处加入计算workmode body长度的代码要根据收到的命令所解析的结构CActTable来计算"));
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

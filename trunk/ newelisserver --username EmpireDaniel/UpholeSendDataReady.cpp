#include "stdafx.h"
#include "UpholeSendDataReady.h"

CUpholeSendDataReady::CUpholeSendDataReady(void)
{
	//此处加入计算upholesend data ready body长度的代码要根据
	//收到的命令所解析的结构CActTable来计算
	bodyLen = sizeof(ULONG);
	//AfxMessageBox(_T("此处加入计算upholesend data ready body长度的代码要根据收到的命令所解析的结构CActTable来计算"));
	cmdLen = bodyLen + SOCK_RECEIVE_HEADER_LEN;
	cmdType = NET_RETURN_UPHOLESEND_DATAREADY;
}
CUpholeSendDataReady::CUpholeSendDataReady(BUF_TYPE* bf, ULONG len):CFrontData(bf, len) {
	//此处加入计算upholesend data ready body长度的代码要根据
	//收到的命令所解析的结构CActTable来计算
	bodyLen = sizeof(ULONG);
	//AfxMessageBox(_T("此处加入计算upholesend data ready body长度的代码要根据收到的命令所解析的结构CActTable来计算"));
	cmdLen = bodyLen + SOCK_RECEIVE_HEADER_LEN;
	cmdType = NET_RETURN_UPHOLESEND_DATAREADY;
}

CUpholeSendDataReady::~CUpholeSendDataReady(void)
{
}

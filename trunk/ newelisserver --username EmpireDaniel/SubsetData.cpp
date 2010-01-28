#include "stdafx.h"
#include "SubsetData.h"

#include "ELISTestServerDlg.h"
CSubsetData::CSubsetData(void)
{
	//此处加入计算subsetdata body长度的代码要根据
	//收到的命令所解析的结构CActTable来计算
	bodyLen = 0;
	//AfxMessageBox(_T("此处加入计算subsetdata body长度的代码要根据收到的命令所解析的结构CActTable来计算"));
	//cmdLen = bodyLen + SOCK_RECEIVE_HEADER_LEN;
	cmdLen = 0;
	cmdType = NET_RETURN_SUBSETDATA;
}
CSubsetData::CSubsetData(BUF_TYPE* bf, ULONG len):CFrontData(bf, len)
{
	//此处加入计算subsetdata body长度的代码要根据
	//收到的命令所解析的结构CActTable来计算
	bodyLen = 0;
	//AfxMessageBox(_T("此处加入计算subsetdata body长度的代码要根据收到的命令所解析的结构CActTable来计算"));
	//cmdLen = bodyLen + SOCK_RECEIVE_HEADER_LEN;
	cmdLen = 0;
	cmdType = NET_RETURN_SUBSETDATA;
}

void CSubsetData::setCommandHeader(CSubsetDataAssister *assist) {
	//
	setBodyLength(assist);
	//must called after setBodyLength
	setHeader(cmdType, cmdLen);

	ULONG generalState = 0;
	setData((BUF_TYPE*)&generalState, sizeof(ULONG));
}
void CSubsetData::setSubsetData(CSubsetDataAssister *assist, CActTable *acttab) {
	//
	UINT i;
	//for debug begin
	//char bt[4096];
	BUF_TYPE *b;
	UINT sz;
	//for debug end
	for(i = 0; i < assist->actNum; i++) {
		assist->heads[i].currentDepth = assist->dataFileBuf->m_pdlg->m_trueDepthDU;//调试注释掉
		assist->heads[i].currentTime = assist->dataFileBuf->m_pdlg->m_currentTime*1000;//调试注释掉
		
		sz = assist->getRTCBlockDataHeaderSize();
		b = (BUF_TYPE*)&assist->heads[i];
		//sprintf(bt, "Fill SubsetData<%d> header to 0x%lx=%ld,", i, (ULONG)pBuf, (ULONG)pBuf);
		setData(b, sz);

		sz = assist->assist.totalSizeOfSubsetsPerReturn[i];
		b = assist->dataFileBuf->getNextDataPointer(i);
		//sprintf(bt, "%s body of 0x%lx=%ld(sz=%d) to 0x%lx=%ld\n", bt, (ULONG)b, (ULONG)b, sz, (ULONG)pBuf, (ULONG)pBuf);
		setData(b, sz);
		
		//for debug begin
		//assist->dataFileBuf->m_pdlg->log.Write(bt, strlen(bt));
		//assist->dataFileBuf->m_pdlg->log.Flush();
		//for debug end
	}
}

void CSubsetData::setData(BUF_TYPE *bf, ULONG len) {
	CData::setData(bf, len);
}
CSubsetData::~CSubsetData(void)
{
}


//must called before setHeader
void CSubsetData::setBodyLength(CSubsetDataAssister *assist) {
	bodyLen = assist->getTotalActDataSize();
	cmdLen = bodyLen + SOCK_RECEIVE_HEADER_LEN;
}

void CSubsetData::Save(CSubsetDataAssister*assist, CFile &log) {
	char b[4096];
	ULONG *tul;
	UINT32 *tui;
	UINT i,headsize, totalDataSize;
	
	tul = (ULONG*)buf;
	sprintf(b, "===cmd start==\nCMDTYPE:%lx,CMDLEN:%d, GENStaus:%lx\n", tul[0], tul[1], tul[2]);
	log.Write(b, strlen(b));
	log.Flush();
	tul = (ULONG*)(buf + 3*sizeof(ULONG));
	headsize = 3*sizeof(ULONG)+4*sizeof(UINT32);
	totalDataSize = 0;
	for(i = 0; i < assist->actNum; i++) {
		tul = (ULONG*)(buf + 3*sizeof(ULONG) +  i*headsize+totalDataSize);
		sprintf(b, "ToolAddr:%ld,SubsetNo:%ld,SubsetCnt:%ld,",tul[0],tul[1],tul[2]);
		tui = (UINT32*)(tul+3);
		sprintf(b, "%sCurDepth:%d,DataSize:%d,ActSwitch:%d,CurTime:%d,", b, 
			tui[0],tui[1],tui[2],tui[3]);
		sprintf(b, "%sbufAddr:0x%lx=%ld\n", b, (ULONG)tul, (ULONG)tul);
		log.Write(b, strlen(b));
		totalDataSize += assist->assist.totalSizeOfSubsetsPerReturn[i];
	}
	sprintf(b, "===cmd end==\n");
	log.Write(b, strlen(b));
	log.Flush();
}

// CalibSubset.cpp: implementation of the CCalibSubset class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ELISTestServer.h"
#include "CalibSubset.h"

#include "ELISTestServerDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CCalibSubset::CCalibSubset()
{
	//�˴��������calib subsetdata body���ȵĴ���Ҫ����
	//�յ��������������ĽṹCActTable������
	bodyLen = 0;
	//AfxMessageBox(_T("�˴��������calib subset subsetdata body���ȵĴ���Ҫ�����յ��������������ĽṹCActTable������"));
	cmdLen = 0;
	cmdType = NET_RETURN_SNGLACQ_DATAREADY;
}

CCalibSubset::CCalibSubset(BUF_TYPE* bf, ULONG len):CFrontData(bf, len)
{
	//�˴��������calib subsetdata body���ȵĴ���Ҫ����
	//�յ��������������ĽṹCActTable������
	bodyLen = 0;
	//AfxMessageBox(_T("�˴��������calib subset subsetdata body���ȵĴ���Ҫ�����յ��������������ĽṹCActTable������"));
	cmdLen = 0;
	cmdType = NET_RETURN_SNGLACQ_DATAREADY;
}


void CCalibSubset::setBodyLength(CCalibParameter *ccp) {
	bodyLen = ccp->getTotalCalibSubsetDataSize();
	cmdLen = bodyLen + SOCK_RECEIVE_HEADER_LEN;
}

void CCalibSubset::setCommandHeader(CCalibParameter *ccp, CSubsetDataAssister *assist) {
	//
	setBodyLength(ccp);
	//must called after setBodyLength
	setHeader(cmdType, cmdLen);
}
void CCalibSubset::setSubsetData(CCalibParameter *ccp, CSubsetDataAssister *assist) {
	//
	//for debug begin
	char bt[4096];
	BUF_TYPE *b;
	ULONG *shead;
	//short status, time;
	short head[2];
	UINT sz;

	
	b = NULL;
	//����Calib���ݵ�����shortͷ
	b = assist->dataFileBuf->getNextCalibSubsetDataPointer();
	shead = (ULONG*)b;
	head[0] = (short)shead[0];//USHORT
	head[1] = (short)shead[1];
	if (head[1]<=0)
	{
		head[1]=1;
	}
	
	sprintf(bt, "CalibSubsetData>> subset head orig status:0x%lx=%ld,orig time:0x%lx=%ld;", 
		shead[0],shead[0],shead[1],shead[1]);
	sprintf(bt, "%s rtn status:0x%lx=%ld,rtn time:0x%lx=%ld to 0x%lx\n", 
		bt, head[0],head[0],head[1],head[1], (ULONG)pBuf);
	
	setData((BUF_TYPE*)head, sizeof(short)*2);
	
	//����Subset������
	sz = ccp->getRawCalibSubsetDataSize();
	b = b + sizeof(ULONG)*2;
	
	sprintf(bt, "%s body of 0x%lx=%ld(sz=%d) to 0x%lx=%ld\n", bt, (ULONG)b, (ULONG)b, sz, (ULONG)pBuf, (ULONG)pBuf);
	
	setData(b, sz);
	
	
	//for debug begin
	assist->dataFileBuf->m_pdlg->log.Write(bt, strlen(bt));
	assist->dataFileBuf->m_pdlg->log.Flush();
	//for debug end
}

CCalibSubset::~CCalibSubset()
{

}

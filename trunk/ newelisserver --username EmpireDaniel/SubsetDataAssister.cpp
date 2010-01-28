// SubsetDataAssister.cpp: implementation of the CSubsetDataAssister class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ELISTestServer.h"
//#include "ELISTestServerDlg.h"
#include "SubsetDataAssister.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

class CELISTestServerDlg;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSubsetDataAssister::CSubsetDataAssister()
{
	assist.shareOfCommonBuffer = NULL;
	assist.subsetNumPerReturn = NULL;
	assist.totalSizeOfSubsetsPerReturn = NULL;

	heads = NULL;

	created = FALSE;
}

CSubsetDataAssister::~CSubsetDataAssister()
{
	clear();
}

void CSubsetDataAssister::clear()
{
	if(assist.shareOfCommonBuffer != NULL) delete []assist.shareOfCommonBuffer;
	if(assist.subsetNumPerReturn !=  NULL) delete []assist.subsetNumPerReturn;
	if(assist.totalSizeOfSubsetsPerReturn != NULL) delete []assist.totalSizeOfSubsetsPerReturn;
	
	if(heads != NULL) delete []heads;

	created = FALSE;
}

void CSubsetDataAssister::create(UINT actNum)
{
	clear();
	ASSERT(actNum > 0);

	assist.shareOfCommonBuffer = new float[actNum];
	assist.subsetNumPerReturn = new UINT[actNum];
	assist.totalSizeOfSubsetsPerReturn = new UINT[actNum];
	heads = new RtcBLOCK_DATA_HEADER[actNum];

	this->actNum = actNum;
	created = TRUE;
}


void CSubsetDataAssister::Save(CFile &log) {
	char bout[4096];
	ULONG i;

	sprintf(bout, "logTimerElapse:%d\n", assist.logTimerElapse);
	log.Write(bout, strlen(bout));

	sprintf(bout, "subsetNumPerReturn[%d~%d]:", 0, actNum-1);
	for(i = 0; i < actNum-1; i++) {
		sprintf(bout, "%s%d,", bout, assist.subsetNumPerReturn[i]);
	}
	sprintf(bout, "%s%d\n", bout, assist.subsetNumPerReturn[i]);
	log.Write(bout, strlen(bout));
	
	sprintf(bout, "totalSizeOfSubsetsPerReturn[%d~%d]:", 0, actNum-1);
	for(i = 0; i < actNum-1; i++) {
		sprintf(bout, "%s%d,", bout, assist.totalSizeOfSubsetsPerReturn[i]);
	}
	sprintf(bout, "%s%d\n", bout, assist.totalSizeOfSubsetsPerReturn[i]);
	log.Write(bout, strlen(bout));

	sprintf(bout, "shareOfCommonBuffer[%d~%d]:", 0, actNum-1);
	for(i = 0; i < actNum-1; i++) {
		sprintf(bout, "%s%f,", bout, assist.shareOfCommonBuffer[i]);
	}
	sprintf(bout, "%s%f\n", bout, assist.shareOfCommonBuffer[i]);
	log.Write(bout, strlen(bout));
	log.Flush();
}

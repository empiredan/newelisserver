// WorkModeSetter.cpp: implementation of the CWorkModeSetter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ELISTestServer.h"
#include "WorkModeSetter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CWorkModeSetter::init() {
	changeDepth = FALSE;
	changeTime = FALSE;
	returnSubsetData = FALSE;
	depthSign = 0;

	//old3Mode = NET_CMD_NA;
	//old2Mode = NET_CMD_NA;
	oldMode = NET_CMD_NA;
	mode = NET_CMD_NA;
	
	old3Direction = -2;
	old2Direction = -2;
	oldDirection = -2;
	direction = -2;
}
CWorkModeSetter::CWorkModeSetter()
{
	init();
}

CWorkModeSetter::~CWorkModeSetter()
{

}
void CWorkModeSetter::fillWorkMode(BUF_TYPE *buf, ULONG len) {
	UINT32 *t;
	t = (UINT32*)buf;
	
	//old3Mode = old2Mode;
	//old2Mode = oldMode;
	if (mode != RtcSYS_IDLE_CMD)
	{
		oldMode = mode;
	}
	
	mode = ntohl(t[0]);

	old3Direction = old2Direction;
	old2Direction = oldDirection;
	oldDirection = direction;
	direction = ntohl(t[1]);
}

void CWorkModeSetter::fillStandByTimeInterval(BUF_TYPE *buf, ULONG len) {
	UINT32 *t;
	t = (UINT32*)buf;
	//StandbyTime interval
	standByTimeInterval = ntohl(t[0]);
}
void CWorkModeSetter::fillDeactivated(BUF_TYPE *buf, ULONG len) {
	UINT32 *t;
	t = (UINT32*)buf;
	//RtcSYS_DEACTIVE_CMD
	deactivated = ntohl(t[0]);
}
void CWorkModeSetter::fillRecStop(BUF_TYPE *buf, ULONG len) {
	UINT32 *t;
	t = (UINT32*)buf;
	//RtcSYS_RECSTOP_CMD
	recstop = ntohl(t[0]);
}
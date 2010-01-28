#include "stdafx.h"
#include "ActTable.h"

#include "Utils.h"
#include "commands.h"

#include "ELISTestServerDlg.h"
#include "ELISTestServer.h"

CActTable::CActTable()
{
	pSaList = NULL;
	
}

CActTable::~CActTable(void)
{
	if(pSaList != NULL) {
		delete [] pSaList;
	}
}

void CActTable::MaximumCommonSampleRate(int dpthandtime[]) {
	ULONG i;
	int *dpthrate;
	int *timerate;

	ASSERT(actNum > 0);
	dpthrate =  new int[actNum];
	timerate = new int[actNum];
	for(i = 0; i < actNum; i++) {
		dpthrate[i] = pSaList[i].depthSampleRate;
		timerate[i] = pSaList[i].timeInterval;
	}
	//dpthandtime[0] = CUtils::lcm(dpthrate, actNum);
	//dpthandtime[1] = CUtils::lcm(timerate, actNum);
}


//CActTable* CActTable::AllocateActTable(ULONG actNum, unsigned char *buf, int len) {
CActTable* CActTable::AllocateActTable(ULONG actNum, unsigned char *buf, int len) {
	//这里的buf是去掉命令头的buf，是实际数据真正的起始未知
	//长度也是减去命令头的长度
	//actNum在调用前由调用者从socket接收到的buf中解析出来
	CActTable *rtn;
	ULONG *f;
	ASSERT(actNum >= 0);
	
	rtn = new CActTable();
	f = (ULONG*)buf;
	rtn->actNum = actNum;
	rtn->nDepthInterruptMode = ntohl(f[1]);
	rtn->pSaList = new RTCSubset[actNum];

	memcpy(rtn->pSaList, buf+2*sizeof(ULONG), len-2*sizeof(ULONG));
	return rtn;
}
//CActTable* CActTable::AllocateActTable(unsigned char *buf, int len) {
CActTable* CActTable::AllocateActTable(unsigned char *buf, int len) {
	//这里的buf是去掉命令头的buf，是实际数据真正的起始未知
	//长度也是减去命令头的长度
	//actNum在这里解析
	ULONG actNum;
	ULONG *t;
	t = (ULONG*)buf;
	actNum = ntohl(t[0]);
	return AllocateActTable(actNum, buf, len);
}

void CActTable::buildSubsetDataAssister(CSubsetDataAssister *assist, float speed, UINT workState, int measure) {
	assist->create(actNum);
	reBuildSubsetDataAssister(assist, speed, workState, measure);
}
void CActTable::reBuildSubsetDataAssister(CSubsetDataAssister *assist, float speed, UINT workState, int measure) {
	ULONG i;
	//ULONG lcm, gcd;

	UINT lcmgcd[2];


	calculateLCMGCD(lcmgcd);
	
	lcm = lcmgcd[0];
	gcd = lcmgcd[1];

	UINT totalSubsets;

	speed/= 60;

	if(workState == RtcSYS_STANDBY_CMD) {//time模式
		assist->assist.logTimerElapse = lcm;
		totalSubsets = 0;
		for(i = 0; i < actNum; i++) {
			assist->assist.subsetNumPerReturn[i] = lcm/pSaList[i].timeInterval;
			assist->assist.totalSizeOfSubsetsPerReturn[i] = assist->assist.subsetNumPerReturn[i]*subsetSize(i);
			totalSubsets += assist->assist.totalSizeOfSubsetsPerReturn[i];
		}
		for(i = 0; i < actNum; i++) {
			assist->assist.shareOfCommonBuffer[i] = ((float)assist->assist.totalSizeOfSubsetsPerReturn[i])/((float)totalSubsets);
		}
	} else if(workState == RtcSYS_RECSTART_CMD) {//depth模式--depth模式下最好将速度调小些，主频低于1.6GHz的最好<0.5ft/s
		if (measure == nDepthInterruptMode)
		{
			assist->assist.logTimerElapse = 1000/(speed*gcd)+0.5;
		} 
		else
		{
			if (nDepthInterruptMode)
			{
				assist->assist.logTimerElapse = 1000*METRIC_DU/(speed*IMPERIAL_DU*gcd)+0.5;
			} 
			else
			{
				assist->assist.logTimerElapse = 1000*IMPERIAL_DU/(speed*METRIC_DU*gcd)+0.5;
			}
		}
		//assist->assist.logTimerElapse = ceil(1000/(speed*gcd));//(UINT)(1000.0/(speed*((float)gcd)));
		totalSubsets = 0;
		for(i = 0; i < actNum; i++) {
			assist->assist.subsetNumPerReturn[i] = pSaList[i].depthSampleRate/gcd;
			assist->assist.totalSizeOfSubsetsPerReturn[i] = assist->assist.subsetNumPerReturn[i]*subsetSize(i);
			totalSubsets += assist->assist.totalSizeOfSubsetsPerReturn[i];
		}
		for(i = 0; i < actNum; i++) {
			assist->assist.shareOfCommonBuffer[i] = ((float)assist->assist.totalSizeOfSubsetsPerReturn[i])/((float)totalSubsets);
		}
	} else {
		//AfxMessageBox(_T("buildSubsetDataAssister, wrong "));
	}
	//initialize header
	for(i = 0; i < actNum; i++) {
		assist->heads[i].toolAddr = pSaList[i].toolAddress;
		assist->heads[i].subset = pSaList[i].subsetNo;
		assist->heads[i].subsetCnt = assist->assist.subsetNumPerReturn[i];
		assist->heads[i].currentDepth = 0;
		assist->heads[i].dataSize = subsetSize(i);
		assist->heads[i].actSwitch = 1;
		assist->heads[i].currentTime = 0;
	}
}

void CActTable::calculateLCMGCD(UINT lcmgcd[]) {
	UINT *depth = new UINT[actNum];
	UINT *time = new UINT[actNum];

	UINT i;
	for(i = 0; i < actNum; i++) {
		depth[i] = pSaList[i].depthSampleRate;
		time[i] = pSaList[i].timeInterval;
	}
	lcmgcd[0] = CUtils::GetCommonMultipler(time, actNum);
	lcmgcd[1] = CUtils::GetCommonDivider(depth, actNum);

	ASSERT(lcmgcd[0] > 0);
	ASSERT(lcmgcd[1] > 0);

	delete []depth;
	delete []time;
} 

UINT CActTable::getLogTimerElapse(CSubsetDataAssister *assist, float speed, UINT workState, int measure) {
	UINT rtn;

	UINT lcmgcd[2];

	calculateLCMGCD(lcmgcd);

	speed/= 60;

	if(workState == RtcSYS_STANDBY_CMD) {//time模式
		assist->assist.logTimerElapse = lcmgcd[0];
	} else if(workState == RtcSYS_RECSTART_CMD) {//depth模式
		if (measure == nDepthInterruptMode)
		{
			assist->assist.logTimerElapse = 1000/(speed*gcd)+0.5;
		} 
		else
		{
			if (nDepthInterruptMode)
			{
				assist->assist.logTimerElapse = 1000*METRIC_DU/(speed*IMPERIAL_DU*gcd)+0.5;
			} 
			else
			{
				assist->assist.logTimerElapse = 1000*IMPERIAL_DU/(speed*METRIC_DU*gcd)+0.5;
			}
		}
		//assist->assist.logTimerElapse = (UINT)(1000.0/(speed*((float)lcmgcd[1])));
	}
	rtn = assist->assist.logTimerElapse;
	return rtn;
}

void CActTable::Save(CFile &log) {
	char bout[4096];
	ULONG i;

	sprintf(bout, "ActTable.actNum:%d,nDepthInterruptMode:%d\n", actNum,nDepthInterruptMode);
	log.Write(bout, strlen(bout));

	sprintf(bout, "");
	for(i = 0; i < actNum; i++) {
		sprintf(bout, "%spSaList[%d]\t", bout, i);
		sprintf(bout, "%sactNo:%d,", bout, pSaList[i].actNo);
		sprintf(bout, "%sswitchOn:%d,", bout, pSaList[i].switchOn);
		sprintf(bout, "%sactDone:%d,", bout, pSaList[i].actDone);
		sprintf(bout, "%sdepthSampleRate:%d,", bout, pSaList[i].depthSampleRate);
		sprintf(bout, "%stimeInterval:%d,", bout, pSaList[i].timeInterval);
		sprintf(bout, "%stoolAddress:%d,", bout, pSaList[i].toolAddress);
		sprintf(bout, "%ssubsetNo:%d,", bout, pSaList[i].subsetNo);
		sprintf(bout, "%sdelay:%d,", bout, pSaList[i].delay);
		sprintf(bout, "%sm2Length:%d,", bout, pSaList[i].m2Length);
		sprintf(bout, "%sm5Length:%d,", bout, pSaList[i].m5Length);
		sprintf(bout, "%sm7Length:%d\n", bout, pSaList[i].m7Length);
	}
	log.Write(bout, strlen(bout));
	log.Flush();
}
/*
void CActTable::setParentDlg(CELISTestServerDlg *dlg)
{
	m_dlg = dlg;
}
*/


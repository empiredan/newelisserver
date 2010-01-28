#if !defined(COMMANDHANDLER_H)
#define COMMANDHANDLER_H
#include "stdafx.h"
#include <afxmt.h>

#include "Data.h"
#include "Queue.h"
#include "TimerDef.h"

#include "ActTable.h"

#include "CalibParameter.h"
#include "WorkModeSetter.h"


#include "DepthPanelReadTensionAngle.h"
#include "DepthPanelTensionFactor.h"
#include "DPMDisplayParameter.h"
#include "SingleACQDataReady.h"
#include "SubsetData.h"
#include "CalibSubset.h"
#include "UpholeSendDataReady.h"
#include "WorkMode.h"


#pragma once

class CELISTestServerDlg;

class CCommandHandler
{
public:
	CCommandHandler();
	virtual ~CCommandHandler(void);
public:
	HANDLE worker;
	DWORD wid;
	CELISTestServerDlg *dlg;
	BOOL finish;
	ULONG headSize;
public:
	//用这个函数结束线程的while循环
	void setFinish(BOOL op) {
		finish = op;
	}
	void setParentWindow(CELISTestServerDlg *dlg) {
		this->dlg = dlg;
	}
	//这个函数启动线程
	void start();
	//这个函数是线程的工作函数
	static DWORD __stdcall handle(LPVOID param);
	//LPTHREAD_START_ROUTINE handle;
//以下函数定义各种命令的处理
public:
	void NetCmd_InitServiceTable(CMasterData *d);
	void NetCmd_CalibPara(CMasterData *d);
	void NetCmd_CalibStart(CMasterData *d);
	void NetCmd_CalibStop(CMasterData *d);
	void NetCmd_CtrlWorkState(CMasterData *d);
	void NetCmd_SetStandbyTimeInterval(CMasterData *d);
	void NetCmd_CtrlRecStop(CMasterData *d);
	void NetCmd_CtrlActSwitch(CMasterData *d);
	void NetCmd_CtrlActDeactivate(CMasterData *d);
	
	void NetCmd_InitConfigParameter(CMasterData *d);
	void NetCmd_CtrlStandbytimeInterval(CMasterData *d);
	void NetCmd_CtrlRecstop(CMasterData *d);
	void NetCmd_CtrlACTSwitch(CMasterData *d);
	void NetCmd_SnglCtl(CMasterData *d);
	void NetCmd_DepthInternal(CMasterData *d);
	void NetCmd_DepthDirection(CMasterData *d);
	void NetCmd_DepthSpeed(CMasterData *d);
	void NetCmd_TrueDepth(CMasterData *d);
	void NetCmd_CorrectedDepth(CMasterData *d);
	void NetCmd_ManualDepthCorrection(CMasterData *d);
	void NetCmd_AutoDepthCorrection(CMasterData *d);
	void NetCmd_DepthLock(CMasterData *d);
	void NetCmd_DepthUnlock(CMasterData *d);
	void NetCmd_DepthTensionFactor(CMasterData *d);
	void NetCmd_DepthTensionAngle(CMasterData *d);
	void NetCmd_DepthCHT(CMasterData *d);
	//void (CMasterData *d);
	//void (CMasterData *d);
};


#endif

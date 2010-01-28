#include "stdafx.h"
#include "CommandHandler.h"
#include "Data.h"
#include "Queue.h"
#include "ELISTestServerDlg.h"

#include "commands.h"
#include "TimerDef.h"

//class CELISTestServerDlg;
CCommandHandler::CCommandHandler() {
	this->dlg = dlg;
	finish = FALSE;
	headSize = SOCK_RECEIVE_HEADER_LEN;
}
CCommandHandler::~CCommandHandler(void)
{
}

void CCommandHandler::start() {
	worker = CreateThread(NULL, 
							0,
							&CCommandHandler::handle,
							this,		//传递对象指针作为线程函数LPVOID param参数
							0,			//start immediately after create
							&wid);
}

DWORD CCommandHandler::handle(LPVOID param) {
	CCommandHandler *handler = (CCommandHandler *)param;
	CMasterData *d;
	CDataQueue<CMasterData> *q;
	ULONG cmdtype;
	ULONG *buft;
	
	//
	//char t[512];
	q = handler->dlg->getMasterDataQueue();
	
	while(!handler->finish) {
		d = q->deQueue();
		//d是CMasterData类型的数据，在入队列之前，已经把
		//CMasterData的buf的内容设置为接收到的全部内容。
		//因此这里根据第一个long字的命令类型判断后，
		//dispatch即可
		//sprintf_s(t, "Command Handler handle:::buf[0]=%d,buf[1]=%d", ntohl(d->buf[0]), ntohl(d->buf[1]));
		//AfxMessageBox(_T(t));
		if( d == NULL) {
			continue;
		}
		//在这里处理各种命令，调用相应的
		//命令解析函数来做。
		buft = (ULONG*)d->buf;
		cmdtype = ntohl(buft[0]);
		
		switch(cmdtype) {
			case NET_CMDTYPE_INIT_SERVICE_TABLE:
				//call NetCmd_InitServiceTable(*cmdBuffer)
				handler->NetCmd_InitServiceTable(d);
			break;
			case NET_CMDTYPE_INIT_CONFIG_PARAMETER:
				handler->NetCmd_InitConfigParameter(d);
			break;
			case NET_CMDTYPE_CTRL_DEACTIVATE:
				handler->NetCmd_CtrlActDeactivate(d);
			break;
			case NET_CMDTYPE_CTRL_WORK_STATE:
				handler->NetCmd_CtrlWorkState(d);
			break;
			case NET_CMDTYPE_CTRL_STANDBYTIME_INTERVAL:
				handler->NetCmd_CtrlStandbytimeInterval(d);
			break;
			case NET_CMDTYPE_CTRL_RECSTOP:
				handler->NetCmd_CtrlRecstop(d);
			break;
			case NET_CMDTYPE_CTRL_ACTSWITCH:
				handler->NetCmd_CtrlACTSwitch(d);
			break;
			//以下为刻度命令类型
			case NET_CMDTYPE_CALIB_PARA:
				handler->NetCmd_CalibPara(d);
			break;
			case NET_CMDTYPE_CALIB_START:
				handler->NetCmd_CalibStart(d);
			break;
			case NET_CMDTYPE_CALIB_STOP:
				handler->NetCmd_CalibStop(d);
			break;
			//单次控制命令
			case NET_CMDTYPE_SNGL_CTLCMD:
				handler->NetCmd_SnglCtl(d);
			break;
			//设置深度中断为内部或外部,
			//命令长度：4个字节，命令：0为内部，1为外部中断
			case NET_CMDTYPE_DEPTH_INTERNAL:
				handler->NetCmd_DepthInternal(d);
			break;
			//内部方式下，设置方向,
			//命令长度：4个字节，命令： 0为上，1为下
			case NET_CMDTYPE_DEPTH_DIRECTION:
				handler->NetCmd_DepthDirection(d);
			break;
			//在内部方式下，设置速度
			//命令长度：4个字节，命令：速度值
			case NET_CMDTYPE_DEPTH_SPEED:
				handler->NetCmd_DepthSpeed(d);
			break;
			//设置真实深度
			//命令长度：4个字节，命令：深度值，DU数
			case NET_CMDTYPE_DEPTH_TRUEDEPTH:
				handler->NetCmd_TrueDepth(d);
			break;
			//设置深度
			//命令长度：4个字节，命令：深度值，DU数
			case NET_CMDTYPE_DEPTH_CORRECTEDDEPTH:
				handler->NetCmd_CorrectedDepth(d);
			break;
			//手动校正（加减）深度
			//命令长度：8个字节，前4 byte为加减命令：0为减，1为加，
			//后4 byte为公英制命令，0为英制， 1为公制
			case NET_CMDTYPE_DEPTH_MANUALDEPTHCORR:
				handler->NetCmd_ManualDepthCorrection(d);
			break;
			//自动校正（加减）深度
			//命令长度：8个字节，前4 byte为控制命令：0为停止校正，1为开始校正，
			//后4 byte为校正间隔
			case NET_CMDTYPE_DEPTH_AUTODEPTHCORR:
				handler->NetCmd_AutoDepthCorrection(d);
			break;
			//深度板锁定
			//命令长度：4个字节，命令：1
			case NET_CMDTYPE_DEPTH_LOCK:
				handler->NetCmd_DepthLock(d);
			break;
			//深度板解锁
			//命令长度：4个字节，命令：0
			case NET_CMDTYPE_DEPTH_UNLOCK:
				handler->NetCmd_DepthUnlock(d);
			break;
			//通知发送张力因数
			//命令长度：4个字节，命令：1
			//返回：4个字节，高16位maxTension，低16位offsetTension，
			//返回类型：NET_RETURN_DEPTHPANEL_READTENSIONFACTOR
			case NET_CMDTYPE_DEPTH_TENSIONFACTOR:
				handler->NetCmd_DepthTensionFactor(d);
			break;
			//通知发送张力角度
			//命令长度：4个字节，命令：1
			//返回：4个字节，张力角度，
			//返回类型：NET_RETURN_DEPTHPANEL_READTENSIONANGLE
			case NET_CMDTYPE_DEPTH_TENSIONANGLE:
				handler->NetCmd_DepthTensionAngle(d);
			break;
			//通知发送CHT
			//命令长度：4个字节，命令：CHT值
			case NET_CMDTYPE_DEPTH_CHT:
				handler->NetCmd_DepthCHT(d);
			break;
			/*
			case :
			break;
			case :
			break;
			case :
			break;
			case :
			break;
			case :
			break;
			case :
			break;
			case :
			break;
			case :
			break;
			case :
			break;
			case :
			break;
			case :
			break;
			case :
			break;
			case :
			break;
			case :
			break;
			case :
			break;
			case :
			break;
			case :
			break;
			case :
			break;
			case :
			break;
			case :
			break;
			case :
			break;
			case :
			break;
			*/
			default:
				char logdata[1024];
				sprintf(logdata, "___UNKNOWN_COMMAND____:%lx\n",cmdtype);
				handler->dlg->log.Write(logdata, strlen(logdata));
				handler->dlg->log.Flush();
			break;
		}

		delete d;

	}
	
	return 0;
}

void CCommandHandler::NetCmd_InitServiceTable(CMasterData *d) {
	//AfxMessageBox(_T("Implement me, NetCmd_InitServiceTable"));

	CActTable *tb;
	//int rate[2];
	UINT logTimerInterval;

	logTimerInterval = 5000;

	tb = CActTable::AllocateActTable((d->buf)+headSize, d->contentlen-headSize);
	tb->ConvertData();
	dlg->SetACTTable(tb);
	tb->Save(dlg->log);

	
	//别忘了在这里要delete CMasterData类型的指针d。
	//因为原则上，这里把这个收到的前端机发送过来的数据
	//处理完毕，就不会再使用了，要把它删除掉。
	//这个CMasterData对象是在Dialog中通过socket收到之际，
	//new 出来放到队列中的！！！
	//每个这样的处理函数都需要这样做。
	//如果这里的数据需要作为长期使用的参数保存，那么
	//应该把它们通过对应的数据结构拷贝出去。
	//delete d;
	
	char logdata[1024];
	sprintf(logdata, "CCommandHandler::InitServiceTable\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}

void CCommandHandler::NetCmd_CalibPara(CMasterData *d) {
	BUF_TYPE *bodyBuf;
	ULONG bodyLen;

	ULONG *head;
	ULONG cmdType, totalLen;

	head = (ULONG*)d->buf;
	cmdType = ntohl(head[0]);
	totalLen = ntohl(head[1]);

	bodyLen = totalLen - headSize;
	bodyBuf = d->buf + headSize;

	//AfxMessageBox(_T(""));
	CCalibParameter *ccp = new CCalibParameter(bodyBuf, bodyLen);
	dlg->SetCalibParameter(ccp);
	
	//char logdata[1024];
	//sprintf(logdata, "CCommandHandler::NetCmd_CalibPara\n");
	//dlg->log.Write(logdata, strlen(logdata));
	//dlg->log.Flush();
}
void CCommandHandler::NetCmd_CalibStart(CMasterData *d) {
	BUF_TYPE *bodyBuf;
	ULONG bodyLen;

	ULONG *head;
	ULONG cmdType, totalLen;

	head = (ULONG*)d->buf;
	cmdType = ntohl(head[0]);
	totalLen = ntohl(head[1]);

	bodyLen = totalLen - headSize;
	bodyBuf = d->buf + headSize;
/*
	CCalibSubset *rtn = NULL;
	rtn = new CCalibSubset();
	//
	rtn->setCommandHeader(calibpara, m_subsetAssister);
	rtn->setSubsetData(calibpara, m_subsetAssister);
	dlg->getFrontDataQueue()->enQueue(rtn);
	//return calibsubset;*/
	/*CCalibSubset *ccss  = */dlg->getCalibSubset();
	
	//dlg->getFrontDataQueue()->enQueue(ccss);

	//char logdata[1024];
	//sprintf(logdata, "CCommandHandler::NetCmd_CalibStart, returned one calib subset data\n");
	//dlg->log.Write(logdata, strlen(logdata));
	//dlg->log.Flush();
}
void CCommandHandler::NetCmd_CalibStop(CMasterData *d) {
	BUF_TYPE *bodyBuf;
	ULONG bodyLen;

	ULONG *head;
	ULONG cmdType, totalLen;

	head = (ULONG*)d->buf;
	cmdType = ntohl(head[0]);
	totalLen = ntohl(head[1]);

	bodyLen = totalLen - headSize;
	bodyBuf = d->buf + headSize;

	//dlg->EnableCreateLog(TRUE);
	//dlg->EnableStopLog(FALSE);

	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_CalibStop\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandler::NetCmd_CtrlWorkState(CMasterData *d) {
	BUF_TYPE *bodyBuf;
	ULONG bodyLen;

	ULONG *head;
	ULONG cmdType, totalLen;

	UINT32 *conts;
	ULONG *rtnh;
	UINT32 oldmode;


	head = (ULONG*)d->buf;
	cmdType = ntohl(head[0]);
	totalLen = ntohl(head[1]);

	bodyLen = totalLen - headSize;
	bodyBuf = d->buf + headSize;

	oldmode = dlg->wms->mode;
	dlg->wms->fillWorkMode(bodyBuf, bodyLen);


	CWorkMode *wm = new CWorkMode();
	wm->setData((BUF_TYPE*)&dlg->wms->mode, sizeof(UINT32));
	char logdata[1024];
	sprintf(logdata, "NetCmd_CtrlWorkState,received cmd:%lx, state:%lx, direction:%d\n",
		cmdType, dlg->wms->mode, dlg->wms->direction);
	dlg->log.Write(logdata, strlen(logdata));
	rtnh = (ULONG*)wm->buf;
	conts = (UINT32*)(wm->buf+2*sizeof(ULONG));
	sprintf(logdata, "NetCmd_CtrlWorkState,return cmd:%lx,size:%d,conts:%lx\n",rtnh[0], rtnh[1], conts[0]);


	dlg->log.Write(logdata, strlen(logdata));
	//sprintf(logdata, "NetCmd_CtrlWorkState,在fillWorkmode之后，应该执行一个更新界面上");
	//sprintf(logdata, "工作状态，方向等元素的命令,要在CELISTestServerDlg中添加相应的变量");
	//sprintf(logdata, "和接口函数\n");
	dlg->log.Flush();

	dlg->getFrontDataQueue()->enQueue(wm);

	//在反馈了应答之后，应该执行一个更新界面上
	//工作状态，方向等元素的命令
	//要在CELISTestServerDlg中添加相应的变量
	//和接口函数
	dlg->SetCurrentWorkState();//这里有bug，要检查091213
	dlg->SetDirection();

	//最后执行工作状态改变后在新状态下应该做的事
	dlg->HandleWorkStateChange();
}
void CCommandHandler::NetCmd_SetStandbyTimeInterval(CMasterData *d) {
	BUF_TYPE *bodyBuf;
	ULONG bodyLen;

	ULONG *head;
	ULONG cmdType, totalLen;

	head = (ULONG*)d->buf;
	cmdType = ntohl(head[0]);
	totalLen = ntohl(head[1]);

	bodyLen = totalLen - headSize;
	bodyBuf = d->buf + headSize;

	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_SetStandbyTimeInterval\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandler::NetCmd_CtrlRecStop(CMasterData *d) {
	BUF_TYPE *bodyBuf;
	ULONG bodyLen;

	ULONG *head;
	ULONG cmdType, totalLen;

	head = (ULONG*)d->buf;
	cmdType = ntohl(head[0]);
	totalLen = ntohl(head[1]);

	bodyLen = totalLen - headSize;
	bodyBuf = d->buf + headSize;

	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_CtrlRecStop\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandler::NetCmd_CtrlActSwitch(CMasterData *d) {
	BUF_TYPE *bodyBuf;
	ULONG bodyLen;

	ULONG *head;
	ULONG cmdType, totalLen;

	head = (ULONG*)d->buf;
	cmdType = ntohl(head[0]);
	totalLen = ntohl(head[1]);

	bodyLen = totalLen - headSize;
	bodyBuf = d->buf + headSize;

	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_CtrlActSwitch\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandler::NetCmd_CtrlActDeactivate(CMasterData *d) {
	BUF_TYPE *bodyBuf;
	ULONG bodyLen;

	ULONG *head;
	ULONG cmdType, totalLen;

	head = (ULONG*)d->buf;
	cmdType = ntohl(head[0]);
	totalLen = ntohl(head[1]);

	bodyLen = totalLen - headSize;
	bodyBuf = d->buf + headSize;

	dlg->wms->init();
	dlg->StopLogTimer();


	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_CtrlActDeactivate\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}


void CCommandHandler::NetCmd_InitConfigParameter(CMasterData *d) {
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_InitConfigParameter\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandler::NetCmd_CtrlStandbytimeInterval(CMasterData *d) {
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_CtrlStandbytimeInterval\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandler::NetCmd_CtrlRecstop(CMasterData *d) {
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_CtrlRecstop\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandler::NetCmd_CtrlACTSwitch(CMasterData *d) {
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_CtrlACTSwitch\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandler::NetCmd_SnglCtl(CMasterData *d) {
	char logdata[1024];
	sprintf(logdata, "implement me CCommandHandler::NetCmd_SnglCtl\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();

	/*ULONG *t;
	t = (ULONG*)d->buf;
	ULONG cmd = ntohl(t[0]);
	CUpholeSendDataReady *dd = new CUpholeSendDataReady((BUF_TYPE*)&cmd, sizeof(ULONG));
	dlg->getFrontDataQueue()->enQueue(dd);*/
}
void CCommandHandler::NetCmd_DepthInternal(CMasterData *d) {
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_DepthInternal\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandler::NetCmd_DepthDirection(CMasterData *d) {
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_DepthDirection\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandler::NetCmd_DepthSpeed(CMasterData *d) {
	BUF_TYPE *bodyBuf;
	ULONG bodyLen;
	
	ULONG *head;
	ULONG cmdType, totalLen;
	
	head = (ULONG*)d->buf;
	cmdType = ntohl(head[0]);
	totalLen = ntohl(head[1]);
	
	bodyLen = totalLen - headSize;
	bodyBuf = d->buf + headSize;
	dlg->setSpeed(bodyBuf, bodyLen);

	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_DepthSpeed\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandler::NetCmd_TrueDepth(CMasterData *d) {
	BUF_TYPE *bodyBuf;
	ULONG bodyLen;
	
	ULONG *head;
	ULONG cmdType, totalLen;
	
	head = (ULONG*)d->buf;
	cmdType = ntohl(head[0]);
	totalLen = ntohl(head[1]);
	
	bodyLen = totalLen - headSize;
	bodyBuf = d->buf + headSize;
	dlg->setTrueDepth(bodyBuf, bodyLen);
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_TrueDepth\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandler::NetCmd_CorrectedDepth(CMasterData *d) {
	BUF_TYPE *bodyBuf;
	ULONG bodyLen;
	
	ULONG *head;
	ULONG cmdType, totalLen;
	
	head = (ULONG*)d->buf;
	cmdType = ntohl(head[0]);
	totalLen = ntohl(head[1]);
	
	bodyLen = totalLen - headSize;
	bodyBuf = d->buf + headSize;
	dlg->setCorrectedDepth(bodyBuf, bodyLen);
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_CorrectedDepth\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandler::NetCmd_ManualDepthCorrection(CMasterData *d) {
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_ManualDepthCorrection\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandler::NetCmd_AutoDepthCorrection(CMasterData *d) {
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_AutoDepthCorrection\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandler::NetCmd_DepthLock(CMasterData *d) {
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_DepthLock\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandler::NetCmd_DepthUnlock(CMasterData *d) {
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_DepthUnlock\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandler::NetCmd_DepthTensionFactor(CMasterData *d) {
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_DepthTensionFactor\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandler::NetCmd_DepthTensionAngle(CMasterData *d) {
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_DepthTensionAngle\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandler::NetCmd_DepthCHT(CMasterData *d) {
	/*char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_DepthCHT\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
	*/
}

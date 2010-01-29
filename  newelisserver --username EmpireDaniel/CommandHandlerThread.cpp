// CommandHandlerThread.cpp : implementation file
//

#include "stdafx.h"
#include "elistestserver.h"
#include "CommandHandlerThread.h"

//#include "commands.h"
//#include "Data.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommandHandlerThread

IMPLEMENT_DYNCREATE(CCommandHandlerThread, CWinThread)

CCommandHandlerThread::CCommandHandlerThread()
{
}

CCommandHandlerThread::~CCommandHandlerThread()
{
}

BOOL CCommandHandlerThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CCommandHandlerThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CCommandHandlerThread, CWinThread)
	//{{AFX_MSG_MAP(CCommandHandlerThread)
	ON_THREAD_MESSAGE(WM_COMMAND_DATA, OnCommand)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommandHandlerThread message handlers
VOID CCommandHandlerThread::OnCommand(WPARAM wParam, LPARAM lParam)
{
	CMasterData * mData = (CMasterData *)lParam;
	ULONG cmdtype;
	ULONG * buft;

	ASSERT(mData != NULL);
	
	PreProcessMasterData(mData);	
		
	switch(m_cmdType) {

		case NET_CMDTYPE_INIT_SERVICE_TABLE:
			NetCmd_InitServiceTable();
			break;

		case NET_CMDTYPE_INIT_CONFIG_PARAMETER:
			NetCmd_InitConfigParameter();
			break;

		case NET_CMDTYPE_CTRL_DEACTIVATE:
			NetCmd_CtrlActDeactivate();
			break;

		case NET_CMDTYPE_CTRL_WORK_STATE:
			NetCmd_CtrlWorkState();
			break;

		case NET_CMDTYPE_CTRL_STANDBYTIME_INTERVAL:
			NetCmd_CtrlStandbytimeInterval();
			break;

		case NET_CMDTYPE_CTRL_RECSTOP:
			NetCmd_CtrlRecstop();
			break;

		case NET_CMDTYPE_CTRL_ACTSWITCH:
			NetCmd_CtrlACTSwitch();
			break;

		//以下为刻度命令类型
		case NET_CMDTYPE_CALIB_PARA:
			NetCmd_CalibPara();
			break;

		case NET_CMDTYPE_CALIB_START:
			NetCmd_CalibStart();
			break;

		case NET_CMDTYPE_CALIB_STOP:
			NetCmd_CalibStop();
			break;

		//单次控制命令
		case NET_CMDTYPE_SNGL_CTLCMD:
			NetCmd_SnglCtl();
			break;

		//设置深度中断为内部或外部,
		//命令长度：4个字节，命令：0为内部，1为外部中断
		case NET_CMDTYPE_DEPTH_INTERNAL:
			NetCmd_DepthInternal();
			break;

		//内部方式下，设置方向,
		//命令长度：4个字节，命令： 0为上，1为下
		case NET_CMDTYPE_DEPTH_DIRECTION:
			NetCmd_DepthDirection();
			break;

		//在内部方式下，设置速度
		//命令长度：4个字节，命令：速度值
		case NET_CMDTYPE_DEPTH_SPEED:
			NetCmd_DepthSpeed();
			break;

		//设置真实深度
		//命令长度：4个字节，命令：深度值，DU数
		case NET_CMDTYPE_DEPTH_TRUEDEPTH:
			NetCmd_TrueDepth();
			break;

		//设置深度
		//命令长度：4个字节，命令：深度值，DU数
		case NET_CMDTYPE_DEPTH_CORRECTEDDEPTH:
			NetCmd_CorrectedDepth();
			break;

		//手动校正（加减）深度
		//命令长度：8个字节，前4 byte为加减命令：0为减，1为加，
		//后4 byte为公英制命令，0为英制， 1为公制
		case NET_CMDTYPE_DEPTH_MANUALDEPTHCORR:
				NetCmd_ManualDepthCorrection();
			break;

		//自动校正（加减）深度
		//命令长度：8个字节，前4 byte为控制命令：0为停止校正，1为开始校正，
		//后4 byte为校正间隔
		case NET_CMDTYPE_DEPTH_AUTODEPTHCORR:
			NetCmd_AutoDepthCorrection();
			break;

		//深度板锁定
		//命令长度：4个字节，命令：1
		case NET_CMDTYPE_DEPTH_LOCK:
			NetCmd_DepthLock();
			break;
	
		//深度板解锁
		//命令长度：4个字节，命令：0
		case NET_CMDTYPE_DEPTH_UNLOCK:
			NetCmd_DepthUnlock();
			break;

		//通知发送张力因数
		//命令长度：4个字节，命令：1
		//返回：4个字节，高16位maxTension，低16位offsetTension，
		//返回类型：NET_RETURN_DEPTHPANEL_READTENSIONFACTOR
		case NET_CMDTYPE_DEPTH_TENSIONFACTOR:
			NetCmd_DepthTensionFactor();
			break;

		//通知发送张力角度
		//命令长度：4个字节，命令：1
		//返回：4个字节，张力角度，
		//返回类型：NET_RETURN_DEPTHPANEL_READTENSIONANGLE
		case NET_CMDTYPE_DEPTH_TENSIONANGLE:
			NetCmd_DepthTensionAngle();
			break;

		//通知发送CHT
		//命令长度：4个字节，命令：CHT值
		case NET_CMDTYPE_DEPTH_CHT:
			NetCmd_DepthCHT();
			break;
			
		default:
			/*
			char logdata[1024];
			sprintf(logdata, "___UNKNOWN_COMMAND____:%lx\n",cmdtype);
			handler->dlg->log.Write(logdata, strlen(logdata));
			handler->dlg->log.Flush();
			*/
			break;
	}

	delete mData;

	
}

inline void CCommandHandlerThread::PreProcessMasterData(CMasterData *md)
{
	m_cmdType = md->GetCmdType();
	m_totalLen = md->GetTotalLen();
	m_headLen = md->GetHeadLen();
	m_bodyBuf = md->GetBodyBuf();
	m_bodyLen = md->GetBodyLen();
}

void CCommandHandlerThread::NetCmd_InitServiceTable() {
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

void CCommandHandlerThread::NetCmd_CalibPara() {
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
void CCommandHandlerThread::NetCmd_CalibStart() {
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
void CCommandHandlerThread::NetCmd_CalibStop() {
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

void CCommandHandlerThread::NetCmd_CtrlWorkState() {
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

void CCommandHandlerThread::NetCmd_SetStandbyTimeInterval() {
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

void CCommandHandlerThread::NetCmd_CtrlRecStop() {
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

void CCommandHandlerThread::NetCmd_CtrlActSwitch() {
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
void CCommandHandlerThread::NetCmd_CtrlActDeactivate() {
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

/*
void CCommandHandlerThread::NetCmd_InitConfigParameter() {
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_InitConfigParameter\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandlerThread::NetCmd_CtrlStandbytimeInterval() {
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_CtrlStandbytimeInterval\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandlerThread::NetCmd_CtrlRecstop() {
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_CtrlRecstop\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandlerThread::NetCmd_CtrlACTSwitch() {
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_CtrlACTSwitch\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandlerThread::NetCmd_SnglCtl() {
	char logdata[1024];
	sprintf(logdata, "implement me CCommandHandler::NetCmd_SnglCtl\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();

	//ULONG *t;
	//t = (ULONG*)d->buf;
	//ULONG cmd = ntohl(t[0]);
	//CUpholeSendDataReady *dd = new CUpholeSendDataReady((BUF_TYPE*)&cmd, sizeof(ULONG));
	//dlg->getFrontDataQueue()->enQueue(dd);
}
void CCommandHandler::NetCmd_DepthInternal() {
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_DepthInternal\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
void CCommandHandlerThread::NetCmd_DepthDirection() {
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_DepthDirection\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
}
*/
void CCommandHandlerThread::NetCmd_DepthSpeed() {
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
void CCommandHandlerThread::NetCmd_TrueDepth() {
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
void CCommandHandlerThread::NetCmd_CorrectedDepth() {
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
/*
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
*/
void CCommandHandlerThread::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWinThread::OnTimer(nIDEvent);
}

// CommandHandlerThread.cpp : implementation file
//

#include "stdafx.h"
#include "elistestserver.h"
#include "CommandHandlerThread.h"

#include "SocketThread.h"
#include "ELISTestServerDlg.h"
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
	::KillTimer(NULL, m_subsetDataTimerIdentifier);
	::KillTimer(NULL, m_depthDataTimerIdentifier);
	return CWinThread::ExitInstance();
}

void * CCommandHandlerThread::m_pObject;
//UINT CCommandHandlerThread::m_subsetDataTimerIdentifier;
//UINT CCommandHandlerThread::m_depthDataTimerIdentifier;

void CCommandHandlerThread::Init()
{
	m_cmdType = 0;
	m_totalLen = SOCK_RECEIVE_HEADER_LEN;
	m_headLen = SOCK_RECEIVE_HEADER_LEN;
	m_bodyBuf = NULL;
	m_bodyLen = 0;

	m_bufferLen = 0;

	m_timeMS = 0;
	m_speedDUPM = 0;
	m_speedDUPS = 0;
	m_depthDU = 0;

	m_pObject = this;
}

BEGIN_MESSAGE_MAP(CCommandHandlerThread, CWinThread)
	//{{AFX_MSG_MAP(CCommandHandlerThread)
	ON_THREAD_MESSAGE(WM_SOCKET_THREAD_ID, OnSocketThreadID)
	ON_THREAD_MESSAGE(WM_COMMAND_DATA, OnCommand)
	//ON_THREAD_MESSAGE(WM_TIMER, OnTimerProc)
	ON_THREAD_MESSAGE(WM_DATABUF_LEN, OnDataBufLen)
	ON_THREAD_MESSAGE(WM_ALL_ACT_DATAFILE_PATHS, OnAllACTDataFilePaths)
	ON_THREAD_MESSAGE(WM_ACT_DATAFILE_PATH, OnACTDataFilePath)
	ON_THREAD_MESSAGE(WM_CALVER_DATAFILE_PATH, OnCALVERDataFilePath)
	ON_THREAD_MESSAGE(WM_DEPTH_DATA_TIMER, OnDepthDataTimer)
	//ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommandHandlerThread message handlers
VOID CCommandHandlerThread::OnSocketThreadID(WPARAM wParam, LPARAM lParam)
{
	DWORD threadID = (DWORD)lParam;
	m_socketThreadID = threadID;
	Init();
}
VOID CCommandHandlerThread::OnDepthDataTimer(WPARAM wParam, LPARAM lParam)
{
	//::SetTimer((HWND)(GetMainWnd()->GetSafeHwnd()), DEPTH_DATA_TIMER, DEPTH_DATA_TIMER_INTERVAL, (TIMERPROC)OnTimerProc);
	::KillTimer(NULL, m_depthDataTimerIdentifier);
	m_depthDataTimerIdentifier = ::SetTimer(NULL, NULL, DEPTH_DATA_TIMER_INTERVAL, (TIMERPROC)OnTimerProc);
	
}
VOID CALLBACK CCommandHandlerThread::OnTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	CCommandHandlerThread * cmdHandlerThread = (CCommandHandlerThread *)m_pObject;
	if (idEvent == cmdHandlerThread->m_subsetDataTimerIdentifier)
	{
		cmdHandlerThread->SubsetDataTimerProc();
	} 
	else if (idEvent == cmdHandlerThread->m_depthDataTimerIdentifier)
	{
		cmdHandlerThread->DepthDataTimerProc();
	}
	/*
	switch()
	{
	case SUBSET_DATA_TIMER:
		break;
	case DEPTH_DATA_TIMER:
		break;
	default:
		break;
	}*/
	
}
VOID CALLBACK CCommandHandlerThread::SubsetDataTimerProc()
{
	if (m_cWorkMode.GetWorkMode() == RtcSYS_RECSTART_CMD)
	{
		m_depthDU+= m_cACTList.GetDepthDuDelta();
		//m_trueDepthDU+= m_cACTList.GetDepthDuDelta();
		//m_correctedDepthDU+= m_cACTList.GetDepthDuDelta();
		//Send the depth to the Dialog for showing
		::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_DEPTH, NULL, m_depthDU);
	} 
	else if(m_cWorkMode.GetWorkMode() == RtcSYS_RECSTART_CMD 
		    || m_cWorkMode.GetWorkMode() == RtcSYS_STANDBY_CMD)
	{
		m_timeMS+= m_cACTList.GetTimeMSDelta();
		//Send the time to the Dialog for showing
		::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_TIME, NULL, m_timeMS);

		//Send to the socket thread the data ready to be 
		//returned to ELIS Client 
		CFrontData * fData;
		fData = new CFrontData(m_cACTList.GetTotalSubsetDataLen());
		fData->SetHeadOfBuf(NET_RETURN_SUBSETDATA, m_headLen);
		ULONG totalState = 0;
		fData->SetBodyOfBuf((BUF_TYPE *)&totalState, sizeof(ULONG));
		ULONG i;
		for (i = 0; i < m_cACTList.GetACTNum(); i++)
		{
			m_cACTList.SetCurrentDepthOfSubsetData(i, m_depthDU);
			m_cACTList.SetCurrentTimeOfSubsetData(i, m_timeMS);
			fData->SetBodyOfBuf((BUF_TYPE *)m_cACTList.GetRtcBlockDataHeader(i), m_cACTList.GetRtcBlockDataHeaderLen());
			fData->SetBodyOfBuf(m_cDataFileBuffer.GetCurrentPositionOfBlock(i),	m_cACTList.GetAllSubsetsLenOfOneToolSubset(i));
			
			m_cDataFileBuffer.NextPositionOfBlock(i);
		}
		::PostThreadMessage(m_socketThreadID, WM_SEND, NULL, (LPARAM)fData);
	}
	
}
VOID CALLBACK CCommandHandlerThread::DepthDataTimerProc()
{
	//m_dpmDisplayPara.corr_Depth = m_correctedDepthDU;
	//m_dpmDisplayPara.true_Depth = m_trueDepthDU;
	m_dpmDisplayPara.corr_Depth = m_depthDU;
	m_dpmDisplayPara.true_Depth = m_depthDU;
	m_dpmDisplayPara.speed = m_speedDUPM/60;
	m_dpmDisplayPara.totalTension = 5;
	m_dpmDisplayPara.mmd = 0;
	m_dpmDisplayPara.differTension = 2;
	m_dpmDisplayPara.time = m_timeMS;
	m_dpmDisplayPara.nreserved2 = 0;
	
	ULONG totalLen = m_headLen+sizeof(DPM_DISPLAY_PARA);
	CFrontData * fData = new CFrontData(totalLen);
	fData->SetHeadOfBuf(NET_RETURN_DPMPARA, m_headLen);
	fData->SetBodyOfBuf((BUF_TYPE *)&m_dpmDisplayPara, sizeof(DPM_DISPLAY_PARA));

	::PostThreadMessage(m_socketThreadID, WM_SEND, NULL, (LPARAM)fData);
}
void CCommandHandlerThread::WorkModeProc()
{
	
	switch (m_cWorkMode.GetWorkMode())
	{
	case RtcSYS_IDLE_CMD:
		break;
	case RtcSYS_CALIBSTART_CMD:
		break;
	case RtcSYS_TRAINSTART_CMD:
		break;
	case RtcSYS_RECSTART_CMD:
		m_cACTList.SetDepthDuDeltaWithDirection(m_cWorkMode.GetDirection());
		if (m_cWorkMode.GetOldWorkMode() == RtcSYS_RECSTART_CMD)
		{
			break;
		}
	case RtcSYS_STANDBY_CMD:

		if (m_cWorkMode.GetOldWorkMode() != m_cWorkMode.GetWorkMode())
		{
			if (m_cWorkMode.GetOldWorkMode() != NET_CMD_NA)
			{
				//::KillTimer((HWND)(GetMainWnd()->GetSafeHwnd()), SUBSET_DATA_TIMER);
				::KillTimer(NULL, m_subsetDataTimerIdentifier);
			}
			m_cDataFileBuffer.Init(m_cACTList.GetSubsetData());
			//Get the time delta(now the work state should be 
			//STANDBY TIME) and create log timer 
			//so that the subset data can be returned to ELIS client
			m_timeMS = 0;
			::SendMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_TIME, NULL, m_timeMS);
			//::SetTimer((HWND)(GetMainWnd()->GetSafeHwnd()), SUBSET_DATA_TIMER, m_cACTList.GetTimeMSDelta(), (TIMERPROC)OnTimerProc);
			m_subsetDataTimerIdentifier = ::SetTimer(NULL, NULL, m_cACTList.GetTimeMSDelta(), (TIMERPROC)OnTimerProc);
			
		}

		/*if (m_cWorkMode.GetOldWorkMode() == NET_CMD_NA)
		{
			
		} */
		
		break;
	default:
		break;
	}
	
}
VOID CCommandHandlerThread::OnDataBufLen(WPARAM wParam, LPARAM lParam)
{
	ULONG dataFileBufferLen = (ULONG)lParam;
	m_cDataFileBuffer.SetBufferLen(dataFileBufferLen);
}
VOID CCommandHandlerThread::OnAllACTDataFilePaths(WPARAM wParam, LPARAM lParam)
{
	CString * actDataFile = (CString *)lParam;
	m_cDataFileBuffer.SetDataFilePathOfAllBlocks(actDataFile);
		
}
VOID CCommandHandlerThread::OnACTDataFilePath(WPARAM wParam, LPARAM lParam)
{
	ULONG i = (ULONG)wParam;
	CString * actDataFile = (CString *)lParam;
	m_cDataFileBuffer.SetDataFilePathOfBlock(i, *actDataFile);
	
}
VOID CCommandHandlerThread::OnCALVERDataFilePath(WPARAM wParam, LPARAM lParam)
{
	CString * calverDataFile = (CString *)lParam;
}
VOID CCommandHandlerThread::OnCommand(WPARAM wParam, LPARAM lParam)
{
	CMasterData * mData = (CMasterData *)lParam;

	ASSERT(mData != NULL);
	
	PreProcessMasterData(mData);	
		
	switch(m_cmdType) {

		case NET_CMDTYPE_INIT_SERVICE_TABLE:
			NetCmd_InitServiceTable();
			break;

		case NET_CMDTYPE_INIT_CONFIG_PARAMETER:
			//NetCmd_InitConfigParameter();
			break;

		case NET_CMDTYPE_CTRL_DEACTIVATE:
			NetCmd_CtrlActDeactivate();
			break;

		case NET_CMDTYPE_CTRL_WORK_STATE:
			NetCmd_CtrlWorkState();
			break;

		case NET_CMDTYPE_CTRL_STANDBYTIME_INTERVAL:
			//NetCmd_CtrlStandbytimeInterval();
			break;

		case NET_CMDTYPE_CTRL_RECSTOP:
			//NetCmd_CtrlRecstop();
			break;

		case NET_CMDTYPE_CTRL_ACTSWITCH:
			//NetCmd_CtrlACTSwitch();
			break;

		//以下为刻度命令类型
		case NET_CMDTYPE_CALIB_PARA:
			NetCmd_CalibPara();
			break;

		case NET_CMDTYPE_CALIB_START:
			NetCmd_CalibStart();
			break;

		case NET_CMDTYPE_CALIB_STOP:
			//NetCmd_CalibStop();
			break;

		//单次控制命令
		case NET_CMDTYPE_SNGL_CTLCMD:
			//NetCmd_SnglCtl();
			break;

		//设置深度中断为内部或外部,
		//命令长度：4个字节，命令：0为内部，1为外部中断
		case NET_CMDTYPE_DEPTH_INTERNAL:
			//NetCmd_DepthInternal();
			break;

		//内部方式下，设置方向,
		//命令长度：4个字节，命令： 0为上，1为下
		case NET_CMDTYPE_DEPTH_DIRECTION:
			//NetCmd_DepthDirection();
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
				//NetCmd_ManualDepthCorrection();
			break;

		//自动校正（加减）深度
		//命令长度：8个字节，前4 byte为控制命令：0为停止校正，1为开始校正，
		//后4 byte为校正间隔
		case NET_CMDTYPE_DEPTH_AUTODEPTHCORR:
			//NetCmd_AutoDepthCorrection();
			break;

		//深度板锁定
		//命令长度：4个字节，命令：1
		case NET_CMDTYPE_DEPTH_LOCK:
			//NetCmd_DepthLock();
			break;
	
		//深度板解锁
		//命令长度：4个字节，命令：0
		case NET_CMDTYPE_DEPTH_UNLOCK:
			//NetCmd_DepthUnlock();
			break;

		//通知发送张力因数
		//命令长度：4个字节，命令：1
		//返回：4个字节，高16位maxTension，低16位offsetTension，
		//返回类型：NET_RETURN_DEPTHPANEL_READTENSIONFACTOR
		case NET_CMDTYPE_DEPTH_TENSIONFACTOR:
			//NetCmd_DepthTensionFactor();
			break;

		//通知发送张力角度
		//命令长度：4个字节，命令：1
		//返回：4个字节，张力角度，
		//返回类型：NET_RETURN_DEPTHPANEL_READTENSIONANGLE
		case NET_CMDTYPE_DEPTH_TENSIONANGLE:
			//NetCmd_DepthTensionAngle();
			break;

		//通知发送CHT
		//命令长度：4个字节，命令：CHT值
		case NET_CMDTYPE_DEPTH_CHT:
			//NetCmd_DepthCHT();
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
	
	m_cACTList.Init(m_bodyBuf, m_bodyLen);
	m_cDataFileBuffer.SetNumOfBlocks(m_cACTList.GetACTNum());
	
	//Send the "show ACT list" message to Dialog
	::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_ACT_LIST, NULL, (LPARAM)m_cACTList.GetACTList());
	//Get the time delta(now the work state should be 
	//STANDBY TIME) and create log timer and depth timer
	//so that the subset data
	//and depth data can be returned to ELIS client

	
	//别忘了在这里要delete CMasterData类型的指针d。
	//因为原则上，这里把这个收到的前端机发送过来的数据
	//处理完毕，就不会再使用了，要把它删除掉。
	//这个CMasterData对象是在Dialog中通过socket收到之际，
	//new 出来放到队列中的！！！
	//每个这样的处理函数都需要这样做。
	//如果这里的数据需要作为长期使用的参数保存，那么
	//应该把它们通过对应的数据结构拷贝出去。
	//delete d;
	/*
	char logdata[1024];
	sprintf(logdata, "CCommandHandler::InitServiceTable\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
	*/
}

void CCommandHandlerThread::NetCmd_CalibPara() {
	

	/*
	CCalibParameter *ccp = new CCalibParameter(bodyBuf, bodyLen);
	dlg->SetCalibParameter(ccp);
	*/
	//char logdata[1024];
	//sprintf(logdata, "CCommandHandler::NetCmd_CalibPara\n");
	//dlg->log.Write(logdata, strlen(logdata));
	//dlg->log.Flush();
}
void CCommandHandlerThread::NetCmd_CalibStart() {
	
/*
	CCalibSubset *rtn = NULL;
	rtn = new CCalibSubset();
	//
	rtn->setCommandHeader(calibpara, m_subsetAssister);
	rtn->setSubsetData(calibpara, m_subsetAssister);
	dlg->getFrontDataQueue()->enQueue(rtn);
	//return calibsubset;
	dlg->getCalibSubset();
	
	//dlg->getFrontDataQueue()->enQueue(ccss);

	//char logdata[1024];
	//sprintf(logdata, "CCommandHandler::NetCmd_CalibStart, returned one calib subset data\n");
	//dlg->log.Write(logdata, strlen(logdata));
	//dlg->log.Flush();
	*/ 
}
/*
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
*/
void CCommandHandlerThread::NetCmd_CtrlWorkState() {
	

	m_cWorkMode.Init(m_bodyBuf);

	if (m_cWorkMode.GetWorkMode() == RtcSYS_STANDBY_CMD
		|| m_cWorkMode.GetWorkMode() == RtcSYS_RECSTART_CMD)
	{
		m_cACTList.SetWorkMode(m_cWorkMode.GetWorkMode());
	}
	
	//Send the "show work state and direction" message to Dialog

	::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_WORKMODE, NULL, m_cWorkMode.GetWorkMode());
	::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_DIRECTION, NULL, m_cWorkMode.GetDirection());

	//Response for the received "work state" command
	
	CFrontData * fData = new CFrontData(m_cWorkMode.GetTotalWorkModeDataLen());
	fData->SetHeadOfBuf(NET_RETURN_WORKMODE, m_headLen);
	UINT32 workMode;
	int direction;
	fData->SetBodyOfBuf((BUF_TYPE *)&(workMode = m_cWorkMode.GetWorkMode()), sizeof(UINT32));
	fData->SetBodyOfBuf((BUF_TYPE *)&(direction = m_cWorkMode.GetDirection()), sizeof(int));
	::PostThreadMessage(m_socketThreadID, WM_SEND, NULL, (LPARAM)fData);
	
	//Process the updated work mode
	WorkModeProc();

	/*
	dlg->wms->fillWorkMode(bodyBuf, bodyLen);


	
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
	*/
}
/*
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
*/
void CCommandHandlerThread::NetCmd_CtrlActDeactivate() {
	/*

	dlg->wms->init();
	dlg->StopLogTimer();


	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_CtrlActDeactivate\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
	*/
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
	
	long * speed = (long *)m_bodyBuf;
	m_speedDUPM = ntohl(speed[0]);
	m_speedDUPS = m_speedDUPM/60;
	m_cACTList.SetTimeMSDeltaOfDepthMode(m_speedDUPS);
	m_cACTList.SetDpethDUDeltaOfTimeMode(m_speedDUPS);
	::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_SPEED, NULL, m_speedDUPM);
	//Send the "show speed" message to Dialog

	/*
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_DepthSpeed\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
	*/
}
void CCommandHandlerThread::NetCmd_TrueDepth() {
	
	long * trueDepth = (long *)m_bodyBuf;
	m_depthDU = ntohl(trueDepth[0]);
	::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_DEPTH, NULL, m_depthDU);
	//Send the "show true depth" message to Dialog

	/*
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_TrueDepth\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
	*/
}
void CCommandHandlerThread::NetCmd_CorrectedDepth() {
	
	long * correctedDepth = (long *)m_bodyBuf;
	//m_correctedDepthDU = ntohl(correctedDepth[0]);
	m_depthDU = ntohl(correctedDepth[0]);
	::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_DEPTH, NULL, m_depthDU);
	//Send the "show corrected depth" message to Dialog

	/*
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_CorrectedDepth\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
	*/
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
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_DepthCHT\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
	
}

void CCommandHandlerThread::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWinThread::OnTimer(nIDEvent);
}
*/
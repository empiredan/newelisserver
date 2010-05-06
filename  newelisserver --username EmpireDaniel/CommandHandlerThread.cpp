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
//: m_logFile(".\\Elis_Test_Server_Log.txt", CFile::modeCreate|CFile::modeWrite)
{
	m_cmdType = 0;
	m_totalLen = SOCK_RECEIVE_HEADER_LEN;
	m_headLen = SOCK_RECEIVE_HEADER_LEN;
	m_bodyBuf = NULL;
	m_bodyLen = 0;
	
	m_bufferLen = 0;
	
	//m_writeAllBlocksEnabled = FALSE;
	
	m_timeMS = 0;
	m_speedDUPM = 0;
	m_speedDUPS = 0;
	m_depthDU = 0;
	m_isReturnSubsetDataEnabled = FALSE;
	
	m_pObject = this;
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
	KillSubsetDataTimer();
	KillDepthDataTimer();
	//m_logFile.Close();
	return CWinThread::ExitInstance();
}

//Public static member variables
//CEvent CCommandHandlerThread::writeAllBlocksEvent;

//Private static member variables
void * CCommandHandlerThread::m_pObject;
CCriticalSection CCommandHandlerThread::m_timerLock;
//UINT CCommandHandlerThread::m_subsetDataTimerIdentifier;
//UINT CCommandHandlerThread::m_depthDataTimerIdentifier;

/*
void CCommandHandlerThread::Init()
{
	m_cmdType = 0;
	m_totalLen = SOCK_RECEIVE_HEADER_LEN;
	m_headLen = SOCK_RECEIVE_HEADER_LEN;
	m_bodyBuf = NULL;
	m_bodyLen = 0;

	m_bufferLen = 0;

	//m_writeAllBlocksEnabled = FALSE;

	m_timeMS = 0;
	m_speedDUPM = 0;
	m_speedDUPS = 0;
	m_depthDU = 0;

	m_pObject = this;

}
*/
BEGIN_MESSAGE_MAP(CCommandHandlerThread, CWinThread)
	//{{AFX_MSG_MAP(CCommandHandlerThread)
	ON_THREAD_MESSAGE(WM_SOCKET_THREAD_ID, OnSocketThreadID)
	ON_THREAD_MESSAGE(WM_COMMAND_DATA, OnCommand)
	//ON_THREAD_MESSAGE(WM_TIMER, OnTimerProc)
	ON_THREAD_MESSAGE(WM_DATABUF_LEN, OnDataBufLen)
	ON_THREAD_MESSAGE(WM_ACT_DATAFILE_ROOT_PATH, OnACTDataFileRootPath)
	ON_THREAD_MESSAGE(WM_CALVER_DATAFILE_ROOT_PATH, OnCALVERDataFileRootPath)
	ON_THREAD_MESSAGE(WM_ALL_ACT_DATAFILE_PATHS, OnAllACTDataFilePaths)
	ON_THREAD_MESSAGE(WM_ACT_DATAFILE_PATH, OnACTDataFilePath)
	ON_THREAD_MESSAGE(WM_CALVER_DATAFILE_PATH, OnCALVERDataFilePath)

	ON_THREAD_MESSAGE(WM_SET_DEPTH_DATA_TIMER, OnDepthDataTimerSetted)
	ON_THREAD_MESSAGE(WM_SUBSET_DATA_TIMER, OnSubsetDataTimer)
	ON_THREAD_MESSAGE(WM_DEPTH_DATA_TIMER, OnDepthDataTimer)
	ON_THREAD_MESSAGE(WM_ENABLE_RETURN_SUBSET_DATA, OnReturnSubsetDataEnabled)
	//ON_THREAD_MESSAGE(WM_PAUSE_RETURN_SUBSET_DATA, OnPauseReturnSubsetData)
	//ON_MESSAGE(WM_SUBSET_DATA_TIMER, OnSubsetDataTimer)
	//ON_MESSAGE(WM_DEPTH_DATA_TIMER, OnDepthDataTimer)
	//ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommandHandlerThread message handlers
VOID CCommandHandlerThread::OnSocketThreadID(WPARAM wParam, LPARAM lParam)
{
	DWORD threadID = (DWORD)lParam;
	m_socketThreadID = threadID;
	//Init();
}
VOID CCommandHandlerThread::OnDepthDataTimerSetted(WPARAM wParam, LPARAM lParam)
{
	KillDepthDataTimer();
	SetDepthDataTimer();
}
/*
VOID CALLBACK CCommandHandlerThread::TimerProcWrapper(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	CCommandHandlerThread * cmdHandlerThread = (CCommandHandlerThread * )m_pObject;
	cmdHandlerThread->TimerProc(hwnd, uMsg, idEvent, dwTime);

}
*/
VOID CALLBACK CCommandHandlerThread::TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	m_timerLock.Lock();
	CCommandHandlerThread * cmdHandlerThread = (CCommandHandlerThread * )m_pObject;
	switch(idEvent)
	{
	case SUBSET_DATA_TIMER:
		::PostThreadMessage(cmdHandlerThread->m_nThreadID, WM_SUBSET_DATA_TIMER, NULL, NULL);
		break;
	case DEPTH_DATA_TIMER:
		::PostThreadMessage(cmdHandlerThread->m_nThreadID, WM_DEPTH_DATA_TIMER, NULL, NULL);
		break;
	}
	m_timerLock.Unlock();
	
	//::EnterCriticalSection(&m_timerLock);
	/*m_timerLock.Lock();
	CCommandHandlerThread * cmdHandlerThread = (CCommandHandlerThread *)m_pObject;
	if (idEvent == cmdHandlerThread->m_subsetDataTimerIdentifier)
	{
		cmdHandlerThread->SubsetDataTimerProc();
	} 
	else if (idEvent == cmdHandlerThread->m_depthDataTimerIdentifier)
	{
		cmdHandlerThread->DepthDataTimerProc();
	}
	m_timerLock.Unlock();*/
	//::LeaveCriticalSection(&m_timerLock);

}
VOID CCommandHandlerThread::OnSubsetDataTimer(WPARAM wParam, LPARAM lParam)
{
	/*if (m_cWorkMode.GetWorkMode() == RtcSYS_RECSTART_CMD)
	{
		m_depthDU+= m_cACTList.GetDepthDuDelta();
		//m_trueDepthDU+= m_cACTList.GetDepthDuDelta();
		//m_correctedDepthDU+= m_cACTList.GetDepthDuDelta();
		//Send the depth to the Dialog for showing
		::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_DEPTH, NULL, m_depthDU);
	} 
	else if(m_cWorkMode.GetWorkMode() == RtcSYS_RECSTART_CMD || m_cWorkMode.GetWorkMode() == RtcSYS_STANDBY_CMD)
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
	}*/
	switch (m_cWorkMode.GetWorkMode())
	{
	case RtcSYS_RECSTART_CMD:
		m_depthDU+= m_cACTList.GetDepthDuDelta();
		//m_trueDepthDU+= m_cACTList.GetDepthDuDelta();
		//m_correctedDepthDU+= m_cACTList.GetDepthDuDelta();
		//Send the depth to the Dialog for showing
		::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_DEPTH, NULL, m_depthDU);
	case RtcSYS_STANDBY_CMD:
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
			//char logContent[8192];
			//sprintf(logContent, "Assembling begun now. \n");
			for (i = 0; i < m_cACTList.GetACTNum(); i++)
			{
				m_cACTList.SetCurrentDepthOfSubsetData(i, m_depthDU);
				m_cACTList.SetCurrentTimeOfSubsetData(i, m_timeMS);
				//sprintf(logContent, "%sAssembling subset in the Block numbered by %d. ", logContent, i);
				fData->SetBodyOfBuf((BUF_TYPE *)m_cACTList.GetRtcBlockDataHeader(i), m_cACTList.GetRtcBlockDataHeaderLen());
				//sprintf(logContent, "%sBegin to write data. ", logContent);
				fData->SetBodyOfBuf(m_cDataFileBuffer.GetCurrentPositionOfBlock(i),	m_cACTList.GetAllSubsetsLenOfOneToolSubset(i));
				//sprintf(logContent, "%sChange for the next position in this Block. ", logContent);
				m_cDataFileBuffer.NextPositionOfBlock(i);
				//sprintf(logContent, "%sAssembling subset finished now.\n", logContent);

			}
			//m_logFile.Write(logContent, strlen(logContent));
			//m_logFile.Flush();
			::PostThreadMessage(m_socketThreadID, WM_SEND, NULL, (LPARAM)fData);
		}
		break;
	}
}
VOID CCommandHandlerThread::OnDepthDataTimer(WPARAM wParam, LPARAM lParam)
{
	//m_dpmDisplayPara.corr_Depth = m_correctedDepthDU;
	//m_dpmDisplayPara.true_Depth = m_trueDepthDU;
	m_dpmDisplayPara.corr_Depth = m_depthDU;
	m_dpmDisplayPara.true_Depth = m_depthDU;
	m_dpmDisplayPara.speed = m_speedDUPM;
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
void CCommandHandlerThread::ResetCurrentTiming()
{
	m_timeMS = 0;
	::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_TIME, NULL, m_timeMS);
}
void CCommandHandlerThread::WorkModeProc()
{
	
	switch (m_cWorkMode.GetWorkMode())
	{
	case RtcSYS_IDLE_CMD:
		break;
	case RtcSYS_CALIBSTART_CMD:
		KillSubsetDataTimer();
		KillDepthDataTimer();
		break;
	case RtcSYS_TRAINSTART_CMD:
		KillSubsetDataTimer();
		break;
	case RtcSYS_RECSTART_CMD:
		m_cACTList.SetDepthDuDeltaWithDirection(m_cWorkMode.GetDirection());
		/*
		if (m_cWorkMode.GetOldWorkMode() == RtcSYS_RECSTART_CMD)
		{
			break;
		}
		*/
	case RtcSYS_STANDBY_CMD:
		m_cACTList.SetWorkMode(m_cWorkMode.GetWorkMode());
		if (m_cWorkMode.GetWorkMode() != m_cWorkMode.GetOldWorkMode()
			|| m_cWorkMode.GetWorkMode() == RtcSYS_STANDBY_CMD)
		{
			/**
			* Try to kill the SUBSET DATA TIMER if exists
			**/
			//if (m_cWorkMode.GetOldWorkMode() != RtcSYS_NA_CMD)
			KillSubsetDataTimer();
			
			/**
			* Reset the time by zero showed in the Dialog
			**/
			//m_timeMS = 0;
			//::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_TIME, NULL, m_timeMS);
			ResetCurrentTiming();
			
			/**
			* Get the data file buffer initiated,
			* then write the data of ".dat" file to the related block 
			**/
			m_cDataFileBuffer.Init(m_cACTList.GetSubsetData());
			CELISTestServerDlg::m_accessDataFileMutex.Lock();
			m_cDataFileBuffer.SetDataFilePathByRootPath(m_actDataFileRootPath);
			m_cDataFileBuffer.WriteAllBlocks();
			CELISTestServerDlg::m_accessDataFileMutex.Unlock();
			
			/**
			* Get the time delta(now the work state should be 
			* STANDBY TIME) and create log timer 
			* so that the subset data can be returned to ELIS client
			**/
			if (m_cWorkMode.GetOldWorkMode() == RtcSYS_NA_CMD)
			{
				m_isReturnSubsetDataEnabled = TRUE;
			}
			
		}
		if (m_isReturnSubsetDataEnabled)
		{
			SetSubsetDataTimer();
			::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_ENABLE_START_LOG, NULL, FALSE);
			::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_ENABLE_PAUSE_LOG, NULL, TRUE);
		}
		else
		{
			::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_ENABLE_START_LOG, NULL, TRUE);
			::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_ENABLE_PAUSE_LOG, NULL, FALSE);
		}
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
VOID CCommandHandlerThread::OnACTDataFileRootPath(WPARAM wParam, LPARAM lParam)
{
	m_actDataFileRootPath = *((CString *)lParam);
}
VOID CCommandHandlerThread::OnCALVERDataFileRootPath(WPARAM wParam, LPARAM lParam)
{
	m_calverDataFileRootPath = *((CString *)lParam);
}
VOID CCommandHandlerThread::OnAllACTDataFilePaths(WPARAM wParam, LPARAM lParam)
{
	CString * actDataFile = (CString *)lParam;
	m_cDataFileBuffer.SetDataFilePathOfAllBlocks(actDataFile);
	//m_writeAllBlocksEnabled = TRUE;
		
}
VOID CCommandHandlerThread::OnACTDataFilePath(WPARAM wParam, LPARAM lParam)
{
	ULONG i = (ULONG)wParam;
	CString * actDataFile = (CString *)lParam;
	m_cDataFileBuffer.SetDataFilePathOfBlock(*actDataFile, i);
	
}
VOID CCommandHandlerThread::OnCALVERDataFilePath(WPARAM wParam, LPARAM lParam)
{
	CString * calverDataFile = (CString *)lParam;
	m_cDataFileBuffer.SetDataFilePathOfBlock(*calverDataFile);
}
VOID CCommandHandlerThread::OnReturnSubsetDataEnabled(WPARAM wParam, LPARAM lParam)
{
	m_isReturnSubsetDataEnabled = (BOOL)lParam;
	/**
	* The current work mode must be STANDBY or RECORD !!!
	* Because the two button of START and PAUSE are valid
	* only in the two kinds of work mode.
	**/
	if (m_isReturnSubsetDataEnabled)
	{
		switch (m_cWorkMode.GetOldWorkMode())
		{
		
		case RtcSYS_RECSTART_CMD:
			if (m_cWorkMode.GetWorkMode() == m_cWorkMode.GetOldWorkMode())
			{
				SetSubsetDataTimer();
				break;
			}
			
		case RtcSYS_STANDBY_CMD:
			//m_timeMS = 0;
			//::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_TIME, NULL, m_timeMS);
			ResetCurrentTiming();

		case RtcSYS_SAME_STANDBY_CMD:
			SetSubsetDataTimer();
			break;

		default:
			break;
		}
	} 
	else
	{
		switch (m_cWorkMode.GetWorkMode())
		{
		case RtcSYS_RECSTART_CMD:
			m_cWorkMode.SetOldWorkMode(RtcSYS_RECSTART_CMD);
			KillSubsetDataTimer();
			break;
		case RtcSYS_STANDBY_CMD:
			m_cWorkMode.SetOldWorkMode(RtcSYS_SAME_STANDBY_CMD);
			KillSubsetDataTimer();
			break;
		default:
			break;
		}
		
	}
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
			NetCmd_CtrlRecStop();
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
			NetCmd_CalibStop();
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

void CCommandHandlerThread::NetCmd_InitServiceTable() 
{
	
	m_cACTList.Init(m_bodyBuf, m_bodyLen);
	m_cDataFileBuffer.SetNumOfBlocks(m_cACTList.GetACTNum());

	//Send the "show ACT list" message to Dialog
	::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_ACT_LIST, NULL, (LPARAM)m_cACTList.GetACTList());
	::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_ENABLE_DATA_BUFFER_SIZE_BUTTON, NULL, (LPARAM)FALSE);
	/*
	char logdata[1024];
	sprintf(logdata, "CCommandHandler::InitServiceTable\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
	*/
}

void CCommandHandlerThread::NetCmd_CalibPara() 
{
	m_cCalib.Init(m_bodyBuf, m_bodyLen, m_cACTList.GetACTList());
	m_cDataFileBuffer.Init(m_cCalib.GetCalibData());
	CELISTestServerDlg::m_accessDataFileMutex.Lock();
	m_cDataFileBuffer.SetDataFilePathByRootPath(m_calverDataFileRootPath);
	m_cDataFileBuffer.WriteBlock(m_cCalib.GetBlockNo());
	CELISTestServerDlg::m_accessDataFileMutex.Unlock();
	//Send the "show calib parameter" message to Dialog
	::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_CALVER_LIST, NULL, (LPARAM)m_cCalib.GetCalibData());
	::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_ENABLE_CALVER_ROOT_FOLDER_BUTTON, NULL, (LPARAM)FALSE);

	
	/*
	char logdata[1024];
	sprintf(logdata, "CCommandHandler::NetCmd_CalibPara\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
	*/
}
void CCommandHandlerThread::NetCmd_CalibStart() 
{
	CFrontData * fData = new CFrontData(m_cCalib.GetTotalCalibDataLen());
	fData->SetHeadOfBuf(NET_RETURN_SNGLACQ_DATAREADY, m_headLen);
	fData->SetBodyOfBuf((BUF_TYPE *)m_cDataFileBuffer.GetCurrentPositionOfBlock(m_cCalib.GetBlockNo()), m_cCalib.GetSubsetLen());
	m_cDataFileBuffer.NextPositionOfBlock(m_cCalib.GetBlockNo());
	::PostThreadMessage(m_socketThreadID, WM_SEND, NULL, (LPARAM)fData);
	
	/*
	//char logdata[1024];
	//sprintf(logdata, "CCommandHandler::NetCmd_CalibStart, returned one calib subset data\n");
	//dlg->log.Write(logdata, strlen(logdata));
	//dlg->log.Flush();
	*/ 
}
void CCommandHandlerThread::NetCmd_CalibStop() 
{
	::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_ENABLE_CALVER_ROOT_FOLDER_BUTTON, NULL, (LPARAM)TRUE);
	SetDepthDataTimer();
	//m_cDataFileBuffer.ResetBlock(m_cCalib.GetBlockNo());
	/*
	char logdata[1024];
	sprintf(logdata, "Implement me!! CCommandHandler::NetCmd_CalibStop\n");
	dlg->log.Write(logdata, strlen(logdata));
	dlg->log.Flush();
	*/
}

void CCommandHandlerThread::NetCmd_CtrlWorkState() {

	m_cWorkMode.Init(m_bodyBuf);
	/*
	if (m_cWorkMode.GetWorkMode() == RtcSYS_STANDBY_CMD
		|| m_cWorkMode.GetWorkMode() == RtcSYS_RECSTART_CMD)
	{
		m_cACTList.SetWorkMode(m_cWorkMode.GetWorkMode());
	}
	*/	
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
*/
void CCommandHandlerThread::NetCmd_CtrlRecStop() {
	KillSubsetDataTimer();
	//ResetCurrentTiming();
}
/*
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

	KillSubsetDataTimer();
	m_cWorkMode.SetWorkMode(RtcSYS_NA_CMD);
	::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_ENABLE_DATA_BUFFER_SIZE_BUTTON, NULL, (LPARAM)TRUE);
	/*
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

	if (m_isReturnSubsetDataEnabled)
	{
		switch (m_cWorkMode.GetWorkMode())
		{
		case RtcSYS_RECSTART_CMD:
			
		case RtcSYS_STANDBY_CMD:
			KillSubsetDataTimer();
			SetSubsetDataTimer();
			break;
		default:
			break;
		}
	}
	
	
	//Send the "show speed" message to Dialog
	::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_SPEED, NULL, m_speedDUPM);

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
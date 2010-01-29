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

		//����Ϊ�̶���������
		case NET_CMDTYPE_CALIB_PARA:
			NetCmd_CalibPara();
			break;

		case NET_CMDTYPE_CALIB_START:
			NetCmd_CalibStart();
			break;

		case NET_CMDTYPE_CALIB_STOP:
			NetCmd_CalibStop();
			break;

		//���ο�������
		case NET_CMDTYPE_SNGL_CTLCMD:
			NetCmd_SnglCtl();
			break;

		//��������ж�Ϊ�ڲ����ⲿ,
		//����ȣ�4���ֽڣ����0Ϊ�ڲ���1Ϊ�ⲿ�ж�
		case NET_CMDTYPE_DEPTH_INTERNAL:
			NetCmd_DepthInternal();
			break;

		//�ڲ���ʽ�£����÷���,
		//����ȣ�4���ֽڣ���� 0Ϊ�ϣ�1Ϊ��
		case NET_CMDTYPE_DEPTH_DIRECTION:
			NetCmd_DepthDirection();
			break;

		//���ڲ���ʽ�£������ٶ�
		//����ȣ�4���ֽڣ�����ٶ�ֵ
		case NET_CMDTYPE_DEPTH_SPEED:
			NetCmd_DepthSpeed();
			break;

		//������ʵ���
		//����ȣ�4���ֽڣ�������ֵ��DU��
		case NET_CMDTYPE_DEPTH_TRUEDEPTH:
			NetCmd_TrueDepth();
			break;

		//�������
		//����ȣ�4���ֽڣ�������ֵ��DU��
		case NET_CMDTYPE_DEPTH_CORRECTEDDEPTH:
			NetCmd_CorrectedDepth();
			break;

		//�ֶ�У�����Ӽ������
		//����ȣ�8���ֽڣ�ǰ4 byteΪ�Ӽ����0Ϊ����1Ϊ�ӣ�
		//��4 byteΪ��Ӣ�����0ΪӢ�ƣ� 1Ϊ����
		case NET_CMDTYPE_DEPTH_MANUALDEPTHCORR:
				NetCmd_ManualDepthCorrection();
			break;

		//�Զ�У�����Ӽ������
		//����ȣ�8���ֽڣ�ǰ4 byteΪ�������0ΪֹͣУ����1Ϊ��ʼУ����
		//��4 byteΪУ�����
		case NET_CMDTYPE_DEPTH_AUTODEPTHCORR:
			NetCmd_AutoDepthCorrection();
			break;

		//��Ȱ�����
		//����ȣ�4���ֽڣ����1
		case NET_CMDTYPE_DEPTH_LOCK:
			NetCmd_DepthLock();
			break;
	
		//��Ȱ����
		//����ȣ�4���ֽڣ����0
		case NET_CMDTYPE_DEPTH_UNLOCK:
			NetCmd_DepthUnlock();
			break;

		//֪ͨ������������
		//����ȣ�4���ֽڣ����1
		//���أ�4���ֽڣ���16λmaxTension����16λoffsetTension��
		//�������ͣ�NET_RETURN_DEPTHPANEL_READTENSIONFACTOR
		case NET_CMDTYPE_DEPTH_TENSIONFACTOR:
			NetCmd_DepthTensionFactor();
			break;

		//֪ͨ���������Ƕ�
		//����ȣ�4���ֽڣ����1
		//���أ�4���ֽڣ������Ƕȣ�
		//�������ͣ�NET_RETURN_DEPTHPANEL_READTENSIONANGLE
		case NET_CMDTYPE_DEPTH_TENSIONANGLE:
			NetCmd_DepthTensionAngle();
			break;

		//֪ͨ����CHT
		//����ȣ�4���ֽڣ����CHTֵ
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

	
	//������������Ҫdelete CMasterData���͵�ָ��d��
	//��Ϊԭ���ϣ����������յ���ǰ�˻����͹���������
	//������ϣ��Ͳ�����ʹ���ˣ�Ҫ����ɾ������
	//���CMasterData��������Dialog��ͨ��socket�յ�֮�ʣ�
	//new �����ŵ������еģ�����
	//ÿ�������Ĵ���������Ҫ��������
	//��������������Ҫ��Ϊ����ʹ�õĲ������棬��ô
	//Ӧ�ð�����ͨ����Ӧ�����ݽṹ������ȥ��
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
	//sprintf(logdata, "NetCmd_CtrlWorkState,��fillWorkmode֮��Ӧ��ִ��һ�����½�����");
	//sprintf(logdata, "����״̬�������Ԫ�ص�����,Ҫ��CELISTestServerDlg�������Ӧ�ı���");
	//sprintf(logdata, "�ͽӿں���\n");
	dlg->log.Flush();

	dlg->getFrontDataQueue()->enQueue(wm);

	//�ڷ�����Ӧ��֮��Ӧ��ִ��һ�����½�����
	//����״̬�������Ԫ�ص�����
	//Ҫ��CELISTestServerDlg�������Ӧ�ı���
	//�ͽӿں���
	dlg->SetCurrentWorkState();//������bug��Ҫ���091213
	dlg->SetDirection();

	//���ִ�й���״̬�ı������״̬��Ӧ��������
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

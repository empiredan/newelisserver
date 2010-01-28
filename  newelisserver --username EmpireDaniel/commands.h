//���紫�����Ͷ���
#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#define DEFAULT_BUF_LEN 50000
typedef BYTE BUF_TYPE; 
#define SOCK_RECEIVE_HEADER_LEN (2*sizeof(ULONG))

#define IMPERIAL_DU ((LONG)30480)
#define METRIC_DU   ((LONG)100000)


/*!  command type*/
#define NET_CMD_NA									((ULONG)0x0)
/*! type mask */
#define NET_CMD_MASK								( (ULONG)0xff00 )
/*! ����������  */
#define NET_CMDTYPE_SUB_MASK						( (ULONG)0xff )

/*! _______________________________��������һ����ʼ������__________________________________*/
#define NET_CMDTYPE_INIT							((ULONG)0x00000000)  ���ݰ����ͣ����ݺ���
/*! _______________________________�������Ͷ�����������______________________________________*/
#define NET_CMDTYPE_CTRL							((ULONG)0x00000100)  
#define NET_CMDTYPE_SCOPE_SETTING   				((UINT32)0x00000300)  //ʾ��������
#define NET_CMDTYPE_CALIB							((UINT32)0x00000400)  //�̶�����
#define NET_CMDTYPE_GRCCL 							((UINT32)0x00000500)  //���̲⾮
#define NET_CMDTYPE_TOOL_CMD 						((UINT32)0x00000600)  //���ο�������
#define NET_CMDTYPE_DEPTHPANEL						((UINT32)0x00000700)  //��Ȱ��������
#define NET_CMDTYPE_TRAIN							((UINT32)0x00000800)  //ѵ������	

/*! ע�⣺��������ĸ�ʽΪ���������ͣ����ֽڣ�������ȣ����ֽڣ���������������ֽڣ���  */

/*! _______________________________��������һ��������_______________________________________*/
/*! ��ʼ������������ */
/*!  �������·������Activeʱ�·�*/
/*!  ����ȣ�������ȣ� ��������*/
/*!  ���أ�subsetBuff���������ͣ�NET_RETURN_SUBSET_DATA  	*/	
#define NET_CMDTYPE_INIT_SERVICE_TABLE  					((ULONG)0x00000001)

/*!  �������·����ò���,��Active�·�������·�StandbyTimeInterval���·�*/
/*!  ����ȣ�sizeof(RtcSYS_CONFIG_PARAMETER)�� ���RtcSYS_CONFIG_PARAMETER*/
#define NET_CMDTYPE_INIT_CONFIG_PARAMETER  				((ULONG)0x00000002)

/*! �������������� */
/*! ���������ù���״̬
	����ȣ�8���ֽڣ� ���ǰ4���ֽ�RtcSYS_IDLE_CMD��RtcSYS_STANDBY_CMD��RtcSYS_RECSTART_CMD��
	RtcSYS_CALIBSTART_CMD��RtcSYS_TRAINSTART_CMD֮һ����4�ֽ�Ϊ����0Ϊ�ϣ�1Ϊ��*/
#define NET_CMDTYPE_CTRL_WORK_STATE  					((ULONG)0x00000101)

/*! ����������STANDBYTIME_INTERVAL
	����ȣ�4���ֽڣ� ���StandbyTime interval ��ֵ*/
#define NET_CMDTYPE_CTRL_STANDBYTIME_INTERVAL  			((ULONG)0x00000102)


/*! ����������ϵͳ����
	����ȣ�4���ֽڣ� ���RtcSYS_DEACTIVE_CMD*/
#define NET_CMDTYPE_CTRL_DEACTIVATE  					((ULONG)0x00000103)



/*! �����������⾮��¼��������STANDBYTIME״̬
	����ȣ�4���ֽڣ� RtcSYS_RECSTOP_CMD*/
#define NET_CMDTYPE_CTRL_RECSTOP  					    ((ULONG)0x00000104)

/*! 
	��������̬����ACT
    ����ȣ�8���ֽ�, ��һ��˫��ΪACTNO �ڶ���˫���ǿ��� */
#define NET_CMDTYPE_CTRL_ACTSWITCH  					((ULONG)0x00000105)

/*! _______________________________���������壺�̶�����______________________________________*/

/*! �̶����������� */

/*! ��������ʼ�̶�
	����ȣ�4���ֽڣ� ���RtcSYS_SNGLACQ_CMD
	���أ�unsigned char* ����ָ�룬����ΪCService::GetSubsetSize(ACT *pAct)�ķ���ֵ���������ͣ�NET_RETURN_SNGLACQ_DATAREADY*/
#define NET_CMDTYPE_CALIB_START									((ULONG)0x00000401)

/*! �������̶����ò���
	����ȣ�sizeof(CALIB_CMD_PARA)�� ���CALIB_CMD_PARA
*/
#define NET_CMDTYPE_CALIB_PARA							((ULONG)0x00000402)

/*! ������ֹͣ�̶�
	����ȣ�4 �� ���RtcSYS_CALIBSTOP_CMD*/
#define NET_CMDTYPE_CALIB_STOP								((ULONG)0x00000403)


/*! _______________________________���������ߣ����ο�������__________________________________*/


/*! ���ο������������� */

/*! ���������ο�������
	�����:sizeof(CMD)    ���CMD *pCmd
	���أ������������Ҫ�󷵻�ֵ�����з���ֵ������ֵΪCMD���ͣ��������ͣ�NET_RETURN_UPHOLESEND_DATAREADY*/
/*#define NET_CMDTYPE_SNGL_LACQ 						((ULONG)0x00000601)*/

/*!  ���������ο�������*/
/*!  ����ȣ�4���ֽڣ����RtcSYS_SNGLCMD_CMD*/
#define NET_CMDTYPE_SNGL_CTLCMD 								((ULONG)0x00000602)


/*! _______________________________�������Ͱˣ���ȿ�������__________________________________*/

/*! ��ȿ������������� */

/*! ��������������ж�Ϊ�ڲ����ⲿ
	����ȣ�4���ֽڣ����0Ϊ�ڲ���1Ϊ�ⲿ�ж�*/
#define NET_CMDTYPE_DEPTH_INTERNAL                       ((ULONG)0x00000701)


/*! ���������ڲ���ʽ�£����÷���
	����ȣ�4���ֽڣ���� 0Ϊ�ϣ�1Ϊ��*/
#define NET_CMDTYPE_DEPTH_DIRECTION 					  ((ULONG)0x00000702)

/*! ���������ڲ���ʽ�£������ٶ�
	����ȣ�4���ֽڣ�����ٶ�ֵ*/
#define NET_CMDTYPE_DEPTH_SPEED 						  ((ULONG)0x00000703)

/*! ������������ʵ���
	����ȣ�4���ֽڣ�������ֵ��DU��*/
#define NET_CMDTYPE_DEPTH_TRUEDEPTH 					  ((ULONG)0x00000704)

/*! �������������
	����ȣ�4���ֽڣ�������ֵ��DU��*/
#define NET_CMDTYPE_DEPTH_CORRECTEDDEPTH 				  ((ULONG)0x00000705)

/*! �������ֶ�У�����Ӽ������
	����ȣ�8���ֽڣ�ǰ4 byteΪ�Ӽ����0Ϊ����1Ϊ�ӣ� ��4 byteΪ��Ӣ�����0ΪӢ�ƣ� 1Ϊ����*/
#define NET_CMDTYPE_DEPTH_MANUALDEPTHCORR   				      ((ULONG)0x00000706)

/*! �������Զ�У�����Ӽ������
	����ȣ�8���ֽڣ�ǰ4 byteΪ�������0ΪֹͣУ����1Ϊ��ʼУ���� ��4 byteΪУ�����*/
#define NET_CMDTYPE_DEPTH_AUTODEPTHCORR   				      ((ULONG)0x00000707)

/*! ��������Ȱ�����
	����ȣ�4���ֽڣ����1*/
#define NET_CMDTYPE_DEPTH_LOCK     					  ((ULONG)0x00000708)

/*! ��������Ȱ����
	����ȣ�4���ֽڣ����0*/
#define NET_CMDTYPE_DEPTH_UNLOCK   					  ((ULONG)0x00000709)

/*! ������֪ͨ������������
	����ȣ�4���ֽڣ����1
	���أ�4���ֽڣ���16λmaxTension����16λoffsetTension���������ͣ�NET_RETURN_DEPTHPANEL_READTENSIONFACTOR*/
#define NET_CMDTYPE_DEPTH_TENSIONFACTOR            ((ULONG)0x0000070A)

/*! ������֪ͨ���������Ƕ�
	����ȣ�4���ֽڣ����1
	���أ�4���ֽڣ������Ƕȣ��������ͣ�NET_RETURN_DEPTHPANEL_READTENSIONANGLE*/
#define NET_CMDTYPE_DEPTH_TENSIONANGLE				((ULONG)0x0000070B)


/*! ������֪ͨ����CHT
	����ȣ�4���ֽڣ����CHTֵ*/

#define NET_CMDTYPE_DEPTH_CHT				((ULONG)0x0000070C)





/*! ____________________________ǰ�˻��ϴ���������______________________________*/

/*!  ��STANDBY_TIME��RECORD״̬�£����ص�SUBSET���ݣ���С�ɷ�������*/
#define NET_RETURN_SUBSETDATA								 ((ULONG)0x00010000)

/*!  RTC�õ������״̬���������״̬�ı�ɹ��󣬷���RTC�Ĺ���״̬*/
#define NET_RETURN_WORKMODE    							 ((ULONG)0x00020000)
//�⾮״̬�������ػ���״̬�л���Ӧ�𣬺��һ����������

/*!  ��Ȱ巵�ز���, ��������ΪDPM_DISPLAY_PARA�ṹ*/
#define	NET_RETURN_DPMPARA							 ((ULONG)0x00030000)			
//�����Ϣ����ʱ���ͣ��������л����н��ʱ���Զ�����



/*!  ��ӦNET_CMDTYPE_TRAIN_START������verify�ĳɹ��������粻�ɹ��ͷ��أ�1��*/
/* ���ɼ�Ϊ��оģʽʱ���������״̬��ʾ��о��ѵ���Ƿ�ɹ�������3���֡�*/
/* ��һ���ֱ�ʾ�Ƿ�ɹ��� ���������ǳɹ�ʱ�����õ�GR��CCL���ż�ֵ����mvΪ��λ�� */

/*! ���ɷ���ֵ
	��ӦNET_CMDTYPE_SNGL_LACQ�� ����unsigned char* ����ָ�룬����ΪCService::GetSubsetSize(ACT *pAct)�ķ���ֵ*/
#define	NET_RETURN_SNGLACQ_DATAREADY						 ((ULONG)0x00060000)

/*! �����������Ƶķ���ֵ
	��ӦNET_CMDTYPE_SNGL_CTLCMD�������������Ҫ�󷵻�ֵ�����з���ֵ������ֵΪCMD����*/
#define NET_RETURN_UPHOLESEND_DATAREADY                      ((ULONG)0x00070000)



/*! ��Ȱ巵����������
	��ӦNET_CMDTYPE_DEPTHP_TENSIONFACTOR������4���ֽڣ���16λmaxTension����16λoffsetTension*/
#define NET_RETURN_DEPTHPANEL_READTENSIONFACTOR				 ((ULONG)0x00080000)  


/*���ٹ��죬�������ȣ�4���ֽڣ�����������״̬��1Ϊ���ٹ��죬0Ϊ��������*/
#define	NET_RETURN_SPEEDFAST								 ((ULONG)0x00090000)


/*! ��Ȱ巵�������Ƕ�
	��ӦNET_CMDTYPE_DEPTHP_TENSIONANGLE������4���ֽڣ������Ƕ�*/
#define NET_RETURN_DEPTHPANEL_READTENSIONANGLE				 	 ((ULONG)0x000A0000)







//����ΪһЩ����״̬�Ķ���

#define NO_CMD					((ULONG)0)

/*working modes*/
#define MODE_IDLE				((ULONG)1)

#define MODE_STANDBY 			((ULONG)2)
#define MODE_RECORD				((ULONG)3)
#define MODE_CALIB				((ULONG)4)
#define MODE_TRAIN				((ULONG)5)
#define MODE_DULOAD				((ULONG)6)


#define RtcSYS_DEACTIVE_CMD			((UINT32)0x01000000)		/*system DEACTIVE*/
#define RtcSYS_IDLE_CMD				((UINT32)0x02000000)		/*system idle*/
/*the subset commands must be loaded into  before asserting this cmd*/
#define RtcSYS_STANDBY_CMD			((UINT32)0x03000000)		/*system standby, enter STANDBY mode*/
/*assert RtcSYS_RECSTART_CMD only in STANDBY mode. RtcSYS_RECSTOP_CMD can only be asserted
  in RECORD mode and will set system back to STANDBY mode.*/
#define RtcSYS_RECSTART_CMD			((UINT32)0x04000000)		/*record start, enter RECORD mode*/
#define RtcSYS_RECSTOP_CMD			((UINT32)0x04100000)		/*record stop, exit RECORD mode to STANDBY*/
/*trainning can only be started in STANDBY mode.*/
#define RtcSYS_TRAINSTART_CMD		((UINT32)0x05000000)		/*train start, enter TRAIN mode*/
#define RtcSYS_TRAINSTOP_CMD  		((UINT32)0x05100000)		/*train stop, exit TRAIN mode to STANDBY*/
/*calibration can only be started in STANDBY mode. */
#define RtcSYS_CALIBSTART_CMD		((UINT32)0x06000000)		/*calib start, enter CALIB mode*/
#define RtcSYS_CALIBSTOP_CMD		((UINT32)0x06100000)		/*calib stop, exit CALIB mode to STANDBY*/


/*set channel gain commands. lower 8bit contains the gain*/
#define RtcSYS_SETM2GAIN_CMD		((UINT32)0x10000000)		/*set M2 channel gain*/
#define RtcSYS_SETM5GAIN_CMD		((UINT32)0x11000000)		/*set M5 channel gain*/
#define RtcSYS_SETM7GAIN_CMD		((UINT32)0x12000000)		/*set M7 channel gain*/
#define RtcSYS_SETWTSGAIN_CMD		((UINT32)0x13000000)		/*set WTS CMD gain*/
#define RtcSYS_SETSSGAIN_CMD		((UINT32)0x14000000)		/*set SS gain*/

#define RtcSYS_M2TRAIN_CMD			((UINT32)0x22000000)		/*train M2*/
#define RtcSYS_M5TRAIN_CMD			((UINT32)0x25000000)		/*train M5*/
#define RtcSYS_M7TRAIN_CMD			((UINT32)0x27000000)		/*train M7*/
#define RtcSYS_SSTRAIN_CMD			((UINT32)0x29000000)		/*train Single Conductor*/
/*--------------------------------------------------------------------------------------
 Commands for Scope signal selection:
 Format:		RtcSYS_SCPxSEL_CMD | RtcSCOPEx_SIGNAL
--------------------------------------------------------------------------------------*/
#define RtcSYS_SCP1SEL_CMD			((UINT32)0x31000000)						/*select signal for scope ch1*/
#define RtcSYS_SCP2SEL_CMD			((UINT32)0x32000000)	/*select signal for scope ch1*/

/*--------------------------------------------------------------------------------------
 Command for Scope trigger selection.
 Format:		RtcSYS_SCPTRIG_CMD | WTS command
--------------------------------------------------------------------------------------*/
#define RtcSYS_SCPTRIG_CMD			((UINT32)0x33000000)	/*select trigger*/


#define RtcSYS_SNGLACQ_CMD			((UINT32)0x40000000)


#define RtcM2TO				((UINT32)0x20)
#define RtcM2NVM				((UINT32)0x10)
#define RtcM5TO				((UINT32)0x8)
#define RtcM5NVM				((UINT32)0x4)
#define RtcM7TO				((UINT32)0x2)
#define RtcM7NVM				((UINT32)0x1)


/*definitions of uphole/downhole wireline communication channels*/
#define RtcCH_M2	   	((UINT32)0)							
#define RtcCH_M5			((UINT32)0x00100000)
#define RtcCH_M7			((UINT32)0x00200000)
#define RtcCH_SS			((UINT32)0x00300000)
#define RtcCH_WTSCMD		((UINT32)0x00400000)				/*downhole WTS channel*/


#endif
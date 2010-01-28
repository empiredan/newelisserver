//网络传输类型定义
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
/*! 子类型掩码  */
#define NET_CMDTYPE_SUB_MASK						( (ULONG)0xff )

/*! _______________________________命令类型一：初始化命令__________________________________*/
#define NET_CMDTYPE_INIT							((ULONG)0x00000000)  数据包类型，数据含义
/*! _______________________________命令类型二：控制命令______________________________________*/
#define NET_CMDTYPE_CTRL							((ULONG)0x00000100)  
#define NET_CMDTYPE_SCOPE_SETTING   				((UINT32)0x00000300)  //示波器设置
#define NET_CMDTYPE_CALIB							((UINT32)0x00000400)  //刻度命令
#define NET_CMDTYPE_GRCCL 							((UINT32)0x00000500)  //工程测井
#define NET_CMDTYPE_TOOL_CMD 						((UINT32)0x00000600)  //单次控制命令
#define NET_CMDTYPE_DEPTHPANEL						((UINT32)0x00000700)  //深度板控制命令
#define NET_CMDTYPE_TRAIN							((UINT32)0x00000800)  //训练命令	

/*! 注意：网络命令的格式为：命令类型（４字节）＋命令长度（４字节）＋命令参数（４字节）　  */

/*! _______________________________命令类型一的子类型_______________________________________*/
/*! 初始化命令子类型 */
/*!  描述：下发服务表，Active时下发*/
/*!  命令长度：服务表长度， 命令：服务表*/
/*!  返回：subsetBuff，返回类型：NET_RETURN_SUBSET_DATA  	*/	
#define NET_CMDTYPE_INIT_SERVICE_TABLE  					((ULONG)0x00000001)

/*!  描述：下发配置参数,在Active下发服务表及下发StandbyTimeInterval后下发*/
/*!  命令长度：sizeof(RtcSYS_CONFIG_PARAMETER)， 命令：RtcSYS_CONFIG_PARAMETER*/
#define NET_CMDTYPE_INIT_CONFIG_PARAMETER  				((ULONG)0x00000002)

/*! 控制命令子类型 */
/*! 描述：设置工作状态
	命令长度：8个字节， 命令：前4个字节RtcSYS_IDLE_CMD、RtcSYS_STANDBY_CMD、RtcSYS_RECSTART_CMD、
	RtcSYS_CALIBSTART_CMD、RtcSYS_TRAINSTART_CMD之一，后4字节为方向，0为上，1为下*/
#define NET_CMDTYPE_CTRL_WORK_STATE  					((ULONG)0x00000101)

/*! 描述：设置STANDBYTIME_INTERVAL
	命令长度：4个字节， 命令：StandbyTime interval 的值*/
#define NET_CMDTYPE_CTRL_STANDBYTIME_INTERVAL  			((ULONG)0x00000102)


/*! 描述：结束系统工作
	命令长度：4个字节， 命令：RtcSYS_DEACTIVE_CMD*/
#define NET_CMDTYPE_CTRL_DEACTIVATE  					((ULONG)0x00000103)



/*! 描述：结束测井记录，并进入STANDBYTIME状态
	命令长度：4个字节， RtcSYS_RECSTOP_CMD*/
#define NET_CMDTYPE_CTRL_RECSTOP  					    ((ULONG)0x00000104)

/*! 
	描述：动态开关ACT
    命令长度：8个字节, 第一个双字为ACTNO 第二个双字是开关 */
#define NET_CMDTYPE_CTRL_ACTSWITCH  					((ULONG)0x00000105)

/*! _______________________________命令类型五：刻度命令______________________________________*/

/*! 刻度命令子类型 */

/*! 描述：开始刻度
	命令长度：4个字节， 命令：RtcSYS_SNGLACQ_CMD
	返回：unsigned char* 类型指针，长度为CService::GetSubsetSize(ACT *pAct)的返回值，返回类型：NET_RETURN_SNGLACQ_DATAREADY*/
#define NET_CMDTYPE_CALIB_START									((ULONG)0x00000401)

/*! 描述：刻度配置参数
	命令长度：sizeof(CALIB_CMD_PARA)， 命令：CALIB_CMD_PARA
*/
#define NET_CMDTYPE_CALIB_PARA							((ULONG)0x00000402)

/*! 描述：停止刻度
	命令长度：4 ， 命令：RtcSYS_CALIBSTOP_CMD*/
#define NET_CMDTYPE_CALIB_STOP								((ULONG)0x00000403)


/*! _______________________________命令类型七：单次控制命令__________________________________*/


/*! 单次控制命令子类型 */

/*! 描述：单次控制命令
	命令长度:sizeof(CMD)    命令：CMD *pCmd
	返回：如果控制命令要求返回值，则有返回值，返回值为CMD类型，返回类型：NET_RETURN_UPHOLESEND_DATAREADY*/
/*#define NET_CMDTYPE_SNGL_LACQ 						((ULONG)0x00000601)*/

/*!  描述：单次控制命令*/
/*!  命令长度：4个字节，命令：RtcSYS_SNGLCMD_CMD*/
#define NET_CMDTYPE_SNGL_CTLCMD 								((ULONG)0x00000602)


/*! _______________________________命令类型八：深度控制命令__________________________________*/

/*! 深度控制命令子类型 */

/*! 描述：设置深度中断为内部或外部
	命令长度：4个字节，命令：0为内部，1为外部中断*/
#define NET_CMDTYPE_DEPTH_INTERNAL                       ((ULONG)0x00000701)


/*! 描述：在内部方式下，设置方向
	命令长度：4个字节，命令： 0为上，1为下*/
#define NET_CMDTYPE_DEPTH_DIRECTION 					  ((ULONG)0x00000702)

/*! 描述：在内部方式下，设置速度
	命令长度：4个字节，命令：速度值*/
#define NET_CMDTYPE_DEPTH_SPEED 						  ((ULONG)0x00000703)

/*! 描述：设置真实深度
	命令长度：4个字节，命令：深度值，DU数*/
#define NET_CMDTYPE_DEPTH_TRUEDEPTH 					  ((ULONG)0x00000704)

/*! 描述：设置深度
	命令长度：4个字节，命令：深度值，DU数*/
#define NET_CMDTYPE_DEPTH_CORRECTEDDEPTH 				  ((ULONG)0x00000705)

/*! 描述：手动校正（加减）深度
	命令长度：8个字节，前4 byte为加减命令：0为减，1为加， 后4 byte为公英制命令，0为英制， 1为公制*/
#define NET_CMDTYPE_DEPTH_MANUALDEPTHCORR   				      ((ULONG)0x00000706)

/*! 描述：自动校正（加减）深度
	命令长度：8个字节，前4 byte为控制命令：0为停止校正，1为开始校正， 后4 byte为校正间隔*/
#define NET_CMDTYPE_DEPTH_AUTODEPTHCORR   				      ((ULONG)0x00000707)

/*! 描述：深度板锁定
	命令长度：4个字节，命令：1*/
#define NET_CMDTYPE_DEPTH_LOCK     					  ((ULONG)0x00000708)

/*! 描述：深度板解锁
	命令长度：4个字节，命令：0*/
#define NET_CMDTYPE_DEPTH_UNLOCK   					  ((ULONG)0x00000709)

/*! 描述：通知发送张力因数
	命令长度：4个字节，命令：1
	返回：4个字节，高16位maxTension，低16位offsetTension，返回类型：NET_RETURN_DEPTHPANEL_READTENSIONFACTOR*/
#define NET_CMDTYPE_DEPTH_TENSIONFACTOR            ((ULONG)0x0000070A)

/*! 描述：通知发送张力角度
	命令长度：4个字节，命令：1
	返回：4个字节，张力角度，返回类型：NET_RETURN_DEPTHPANEL_READTENSIONANGLE*/
#define NET_CMDTYPE_DEPTH_TENSIONANGLE				((ULONG)0x0000070B)


/*! 描述：通知发送CHT
	命令长度：4个字节，命令：CHT值*/

#define NET_CMDTYPE_DEPTH_CHT				((ULONG)0x0000070C)





/*! ____________________________前端机上传命令类型______________________________*/

/*!  在STANDBY_TIME或RECORD状态下，返回的SUBSET数据，大小由服务表决定*/
#define NET_RETURN_SUBSETDATA								 ((ULONG)0x00010000)

/*!  RTC得到地面的状态设置命令后，状态改变成功后，返回RTC的工作状态*/
#define NET_RETURN_WORKMODE    							 ((ULONG)0x00020000)
//测井状态，对主控机的状态切换的应答，后跟一个命令内容

/*!  深度板返回参数, 返回类型为DPM_DISPLAY_PARA结构*/
#define	NET_RETURN_DPMPARA							 ((ULONG)0x00030000)			
//深度信息，定时发送，根据上行或下行结合时间自动计算



/*!  响应NET_CMDTYPE_TRAIN_START，返回verify的成功次数，如不成功就返回－1，*/
/* 当采集为单芯模式时，这个返回状态表示单芯的训练是否成功，返回3个字。*/
/* 第一个字表示是否成功， 后两个字是成功时所设置的GR和CCL的门槛值，以mv为单位。 */

/*! 单采返回值
	响应NET_CMDTYPE_SNGL_LACQ， 返回unsigned char* 类型指针，长度为CService::GetSubsetSize(ACT *pAct)的返回值*/
#define	NET_RETURN_SNGLACQ_DATAREADY						 ((ULONG)0x00060000)

/*! 单次仪器控制的返回值
	响应NET_CMDTYPE_SNGL_CTLCMD，如果控制命令要求返回值，则有返回值，返回值为CMD类型*/
#define NET_RETURN_UPHOLESEND_DATAREADY                      ((ULONG)0x00070000)



/*! 深度板返回张力因子
	响应NET_CMDTYPE_DEPTHP_TENSIONFACTOR，返回4个字节，高16位maxTension，低16位offsetTension*/
#define NET_RETURN_DEPTHPANEL_READTENSIONFACTOR				 ((ULONG)0x00080000)  


/*测速过快，参数长度：4个字节，参数：测速状态，1为测速过快，0为测速正常*/
#define	NET_RETURN_SPEEDFAST								 ((ULONG)0x00090000)


/*! 深度板返回张力角度
	响应NET_CMDTYPE_DEPTHP_TENSIONANGLE，返回4个字节，张力角度*/
#define NET_RETURN_DEPTHPANEL_READTENSIONANGLE				 	 ((ULONG)0x000A0000)







//以下为一些工作状态的定义

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
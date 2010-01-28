#ifndef DPMDISPLAYPARAMETER_H
#define DPMDISPLAYPARAMETER_H

#pragma once

#include "commands.h"
#include "Data.h"

class CDPMDisplayParameter:public CFrontData
{
public:
	typedef struct {
		int corr_Depth;
		int true_Depth;
		int speed;
		int totalTension;		//总张力
		int mmd;
		int differTension;		//差分张力
		UINT  time;					//时间
		int   nreserved2;			//保留
	} DPM_DISPLAY_PARA;
	DPM_DISPLAY_PARA ddp;
public:
	CDPMDisplayParameter(void);
	CDPMDisplayParameter(BUF_TYPE* bf, ULONG len);
	virtual ~CDPMDisplayParameter(void);
public:
	virtual void setData(DPM_DISPLAY_PARA &dp);
};

#endif
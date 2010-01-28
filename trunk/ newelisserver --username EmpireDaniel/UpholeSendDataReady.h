#pragma once
#include "data.h"

class CUpholeSendDataReady :public CFrontData
{
public:
	CUpholeSendDataReady(void);
	CUpholeSendDataReady(BUF_TYPE* bf, ULONG len);
	virtual ~CUpholeSendDataReady(void);
};

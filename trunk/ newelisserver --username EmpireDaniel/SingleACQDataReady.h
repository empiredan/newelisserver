#pragma once
#include "data.h"

class CSingleACQDataReady :
	public CFrontData
{
public:
	CSingleACQDataReady(void);
	CSingleACQDataReady(BUF_TYPE* bf, ULONG len);
	virtual ~CSingleACQDataReady(void);
};

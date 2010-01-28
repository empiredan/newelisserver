#pragma once
#include "data.h"

class CSpeedFast :
	public CFrontData
{
public:
	CSpeedFast(void);
	CSpeedFast(BUF_TYPE* bf, ULONG len);
	virtual ~CSpeedFast(void);
};

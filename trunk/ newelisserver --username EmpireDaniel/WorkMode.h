#pragma once
#include "data.h"

class CWorkMode :public CFrontData
{
public:
	CWorkMode(void);
	CWorkMode(BUF_TYPE* bf, ULONG len);
	virtual ~CWorkMode(void);
public:
	void setData(BUF_TYPE *bf, ULONG len);
};

#pragma once
#include "data.h"

class CDepthPanelReadTensionAngle :public CFrontData
{
public:
	CDepthPanelReadTensionAngle(void);
	CDepthPanelReadTensionAngle(BUF_TYPE* bf, ULONG len);
	virtual ~CDepthPanelReadTensionAngle(void);
};

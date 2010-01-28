#pragma once
#include "data.h"

class CDepthPanelTensionFactor :	public CFrontData
{
public:
	CDepthPanelTensionFactor(void);
	CDepthPanelTensionFactor(BUF_TYPE* bf, ULONG len);
	virtual ~CDepthPanelTensionFactor(void);
};

#include "stdafx.h"
#include "DPMDisplayParameter.h"

CDPMDisplayParameter::CDPMDisplayParameter(void)
{
	bodyLen = sizeof(DPM_DISPLAY_PARA);
	cmdLen = bodyLen + SOCK_RECEIVE_HEADER_LEN;
	cmdType = NET_RETURN_DPMPARA;
}
CDPMDisplayParameter::CDPMDisplayParameter(BUF_TYPE* bf, ULONG len):CFrontData(bf, len)
{
	bodyLen = sizeof(DPM_DISPLAY_PARA);
	cmdLen = bodyLen + SOCK_RECEIVE_HEADER_LEN;
	cmdType = NET_RETURN_DPMPARA;
}

CDPMDisplayParameter::~CDPMDisplayParameter(void)
{
}

void CDPMDisplayParameter::setData(DPM_DISPLAY_PARA &dp) {
	setHeader(getCmdType(), getCmdLength());
	
	CData::setData((BUF_TYPE*)&(dp), sizeof(CDPMDisplayParameter::DPM_DISPLAY_PARA));
}


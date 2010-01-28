#if !defined(MESSAGESENDER_H)
#define MESSAGESENDER_H

#include "stdafx.h"
#include <afxmt.h>

#include "Data.h"
#include "Queue.h"

#pragma once

class CELISTestServerDlg;

class CMessageSender
{
public:
	CMessageSender(void);
	virtual ~CMessageSender(void);

public:
	HANDLE worker;
	DWORD wid;
	CELISTestServerDlg *dlg;
	BOOL finish;
public:
	void setFinish(BOOL op) {
		finish = op;
	}
	void setParentWindow(CELISTestServerDlg *dlg) {
		this->dlg = dlg;
	}
	void start();
	static DWORD __stdcall handle(LPVOID param);

};

#endif

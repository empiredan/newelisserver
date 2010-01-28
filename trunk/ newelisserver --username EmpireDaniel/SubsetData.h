#ifndef SUBSETDATA_H
#define SUBSETDATA_H
#pragma once

#include "Data.h"
#include "ActTable.h"


#include "SubsetDataAssister.h"

class CSubsetData :	public CFrontData
{
public:
	CSubsetData(void);
	CSubsetData(BUF_TYPE* bf, ULONG len);
	virtual ~CSubsetData(void);
public:
	void setData(BUF_TYPE *bf, ULONG len);
	void setCommandHeader(CSubsetDataAssister *assist);
	void setSubsetData(CSubsetDataAssister *assist, CActTable *acttab);

private:
	void setBodyLength(CSubsetDataAssister *assist);
public:
	void Save(CSubsetDataAssister*assist, CFile &log);
};

#endif
// DataFileBuffer.h: interface for the CDataFileBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAFILEBUFFER_H__45AAC7E8_2A0D_44F4_817B_7A08A9066CED__INCLUDED_)
#define AFX_DATAFILEBUFFER_H__45AAC7E8_2A0D_44F4_817B_7A08A9066CED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "commands.h"

typedef struct{
	//float percentageOfBlock;
	ULONG blockLen;
	BUF_TYPE * headOfBlock;
	BUF_TYPE * curPosOfBlock;
	CString dataFilePath;
	//ULONG dataFileLen;
	CFile dataFile;
	//ULONG statusTypeLen;
	ULONG curPosOfDataFile;
}Block;

class CDataFileBuffer  
{
public:
	CDataFileBuffer();
	virtual ~CDataFileBuffer();
//Attributes
private:
	BUF_TYPE * m_buffer;
	ULONG m_bufferLen;
	ULONG m_numOfBlocks;
	Block * m_blocks;
	ULONG m_dataFileHeadLen;

//Operations
public:
	void Init(ULONG bufLen, ULONG numOfBlocks, float * percentageOfBlock, \
	ULONG * returnedSectionLenForOneTime, CString * filePath);
	void WriteAllBlocks();
	inline void WriteBlock(ULONG i);
	inline void WriteBlocksByReadFile(ULONG i);
	inline void WriteBlockByRandomNumber(ULONG i);
	inline void SetDataFilePathOfAllBlocks(CString * filePath){
		for (ULONG i = 0; i < m_numOfBlocks; i++)
		{
			m_blocks[i].dataFilePath = filePath[i];
		}
	}
	inline void SetDataFilePathOfBlock(ULONG i, CString filePath){
		m_blocks[i].dataFilePath = filePath; 
	}
	/*
	inline void SetStatusTypeLen(ULONG i, ULONG typelen){
		m_blocks[i].statusTypeLen = typelen;
	}*/
};

#endif // !defined(AFX_DATAFILEBUFFER_H__45AAC7E8_2A0D_44F4_817B_7A08A9066CED__INCLUDED_)

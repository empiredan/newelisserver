// DataFileBuffer.h: interface for the CDataFileBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAFILEBUFFER_H__45AAC7E8_2A0D_44F4_817B_7A08A9066CED__INCLUDED_)
#define AFX_DATAFILEBUFFER_H__45AAC7E8_2A0D_44F4_817B_7A08A9066CED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "commands.h"
#include "ACTList.h"


typedef struct{
	//float percentageOfBlock;
	int subsetLen;
	ULONG allSubsetsOfOneToolSubset;
	ULONG blockLen;
	BUF_TYPE * headOfBlock;
	BUF_TYPE * curPosOfBlock;
	CString dataFilePath;
	//ULONG dataFileLen;
	CFile dataFile;
	long status;
	long time;
	ULONG statusTypeLen;
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
	inline void SetNumOfBlocks(ULONG nBlocks){
		m_numOfBlocks = nBlocks;
		if (m_blocks)
		{
			delete []m_blocks;
		}
		m_blocks = new BUF_TYPE[m_numOfBlocks];
	}
	inline void SetBufferLen(ULONG bufLen){
		m_bufferLen = bufLen;
		if (m_buffer)
		{
			delete []m_buffer;
		}
		m_buffer = new BUF_TYPE[bufLen];
	}
	void Init(SubsetData * subsetData);
	inline BUF_TYPE * GetCurrentPositionOfBlock(ULONG i){
		return m_blocks[i].curPosOfBlock;
	}
	void NextPositionOfBlock(ULONG i){
		m_blocks[i].curPosOfBlock+= m_blocks[i].allSubsetsOfOneToolSubset;
		if (m_blocks[i].curPosOfBlock >= m_blocks[i].headOfBlock+m_blocks[i].blockLen)
		{
			WriteBlock(i);
			m_blocks[i].curPosOfBlock = m_blocks[i].headOfBlock;
		}
	}
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
	
	inline void SetStatusTypeLen(ULONG i, ULONG typelen){
		m_blocks[i].statusTypeLen = typelen;
	}
};

#endif // !defined(AFX_DATAFILEBUFFER_H__45AAC7E8_2A0D_44F4_817B_7A08A9066CED__INCLUDED_)

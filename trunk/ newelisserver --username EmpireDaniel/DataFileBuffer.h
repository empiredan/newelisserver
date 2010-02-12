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
#include "Calib.h"

typedef struct{
	//float percentageOfBlock;
	int subsetLen;
	ULONG allSubsetsLenOfOneToolSubset;
	long status;
	long time;
	CString dataFilePath;
}SubsetData_Attr;

typedef struct{
	int subsetLen;
	short status;
	short time;
	CString dataFilePath;
}CalibData_Attr;


typedef struct{
	//From the class CACTList
	SubsetData_Attr subsetDataAttr;
	
	//From the class CCalib
	CalibData_Attr calibDataAttr;
	
	//Related to the block itself(memory)
	ULONG blockLen;
	BUF_TYPE * headOfBlock;
	BUF_TYPE * curPosOfBlock;
	BUF_TYPE * tailOfBlock;//The next position of real tail

	//Related to the file(disk) read to the block
	//ULONG dataFileLen;
	CFile dataFile;
	ULONG curPosOfDataFile;
	ULONG statusTypeLen;
	
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

	//Two modes: Subset and Calib
	ULONG m_mode;//0: Subset,   1:Calib

//Operations
public:
	inline void SetMode(ULONG mode){
		m_mode = mode;
	}
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
	void Init(ULONG i, CalibData * subsetData);
	inline BUF_TYPE * GetCurrentPositionOfBlock(ULONG i){
		return m_blocks[i].curPosOfBlock;
	}
	void NextPositionOfBlock(ULONG i){
		m_blocks[i].curPosOfBlock+= m_blocks[i].allSubsetsLenOfOneToolSubset;
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
			m_blocks[i].subsetDataAttr.dataFilePath = filePath[i];
		}
	}
	inline void SetDataFilePathOfBlock(ULONG i, CString filePath){
		if (!m_mode)
		{
			m_blocks[i].subsetDataAttr.dataFilePath = filePath; 
		} 
		else
		{
			m_blocks[i].calibDataAttr.dataFilePath = filePath;
		}
		
	}
	
	inline void SetStatusTypeLen(ULONG i, ULONG typelen){
		m_blocks[i].statusTypeLen = typelen;
	}
};

#endif // !defined(AFX_DATAFILEBUFFER_H__45AAC7E8_2A0D_44F4_817B_7A08A9066CED__INCLUDED_)

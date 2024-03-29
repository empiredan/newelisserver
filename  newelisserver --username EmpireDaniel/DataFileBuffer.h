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
/*
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
*/

typedef struct{
	//From the class CACTList
	SubsetData subsetData;
	
	//From the class CCalib
	//CalibData calibData;
	
	//Related to the block itself(memory)
	ULONG blockLen;
	BUF_TYPE * headOfBlock;
	BUF_TYPE * curPosOfBlock;
	//BUF_TYPE * tailOfBlock;//The next position of real tail
	ULONG realUsedBlockLen;

	//Related to the file(disk) read to the block
	//ULONG dataFileLen;
	CString subsetDataFilePath;
	//CString calibDataFilePath;
	CFile dataFile;
	ULONG curPosOfDataFile;
	//ULONG statusTypeLen;
	
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
	CString m_actDataFileRootPath;
	CString m_calverDataFileRootPath;
	Block * m_blocks;
	ULONG m_dataFileHeadLen;

	//Calib
	CalibData m_calibData;
	CString m_calibDataFilePath;

	//Two modes: Subset and Calib
	UINT32 m_mode;//0: Subset,   1:Calib

//Operations
public:
	inline void SetMode(UINT32 mode){
		m_mode = mode;
	}
	inline void SetNumOfBlocks(ULONG nBlocks){
		m_numOfBlocks = nBlocks;
		if (m_blocks)
		{
			delete [] m_blocks;
			m_blocks = NULL;
		}
		m_blocks = new Block[m_numOfBlocks];
	}
	inline void SetBufferLen(ULONG bufLen){
		m_bufferLen = bufLen;
		if (m_buffer)
		{
			delete [] m_buffer;
			m_buffer = NULL;
		}
		m_buffer = new BUF_TYPE[bufLen];
	}
	void Init(SubsetData * sData);
	inline void ResetBlock(ULONG i){

		m_mode = 0;

		m_blocks[i].curPosOfBlock = m_blocks[i].headOfBlock;
		
		m_blocks[i].realUsedBlockLen = m_blocks[i].blockLen;
		
		m_blocks[i].curPosOfDataFile = m_dataFileHeadLen;
	}
	void Init(CalibData * cData);
	inline BUF_TYPE * GetCurrentPositionOfBlock(ULONG i){
		return m_blocks[i].curPosOfBlock;
	}
	void NextPositionOfBlock(ULONG i){
		if (!m_mode)
		{
			m_blocks[i].curPosOfBlock+= m_blocks[i].subsetData.allSubsetsLenOfOneToolSubset;
			
		}
		else
		{
			m_blocks[i].curPosOfBlock+= m_calibData.subsetLen;
		}

		if (m_blocks[i].curPosOfBlock >= m_blocks[i].headOfBlock+m_blocks[i].realUsedBlockLen)
		{
			WriteBlock(i);
			m_blocks[i].curPosOfBlock = m_blocks[i].headOfBlock;
		}
		
	}
	void WriteAllBlocks();
	inline void WriteBlock(ULONG i);
	inline void WriteBlocksByReadFile(ULONG i);
	inline void WriteBlockByRandomNumber(ULONG i);
	void SetDataFilePathByRootPath(CString  rootPath);
	inline void SetDataFilePathOfAllBlocks(CString * filePath){
		for (ULONG i = 0; i < m_numOfBlocks; i++)
		{
			SetDataFilePathOfBlock(filePath[i], i);
		}
	}
	inline void SetDataFilePathOfBlock(CString filePath = "", ULONG i = 0){

		if (!m_mode)
		{
			m_blocks[i].subsetDataFilePath = filePath; 
		} 
		else
		{
			m_calibDataFilePath = filePath;
		}
		
	}
	/*
	inline void SetStatusTypeLen(ULONG i, ULONG typelen){
		m_blocks[i].statusTypeLen = typelen;
	}*/
};

#endif // !defined(AFX_DATAFILEBUFFER_H__45AAC7E8_2A0D_44F4_817B_7A08A9066CED__INCLUDED_)

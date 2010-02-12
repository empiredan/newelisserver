// DataFileBuffer.cpp: implementation of the CDataFileBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "elistestserver.h"
#include "DataFileBuffer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataFileBuffer::CDataFileBuffer()
{

}

CDataFileBuffer::~CDataFileBuffer()
{
	if (m_buffer)
	{
		delete []m_buffer;
	}
	if (m_blocks)
	{
		delete []m_blocks;
	}

}


void CDataFileBuffer::Init(SubsetData * subsetData)
{

	m_mode = 0;

	//m_blocks[0] = m_buffer;
	m_dataFileHeadLen = 3*sizeof(UINT32);
	BUF_TYPE * curPosOfBuffer = m_buffer;
	for (ULONG i = 0; i < m_numOfBlocks; i++)
	{
		m_blocks[i].subsetLen = subsetData[i].rtcBlockDataHeader.dataSize;

		m_blocks[i].allSubsetsLenOfOneToolSubset = subsetData[i].allSubsetsLenOfOneToolSubset;
		
		m_blocks[i].blockLen = ((float)m_bufferLen*subsetData[i].percentateOfDataFileBuf	\
		/subsetData[i].allSubsetsLenOfOneToolSubset)*subsetData[i].allSubsetsLenOfOneToolSubset;

		m_blocks[i].headOfBlock = curPosOfBuffer;

		m_blocks[i].curPosOfBlock = curPosOfBuffer;

		m_blocks[i].dataFilePath = "";

		m_blocks[i].status = subsetData[i].status;

		m_blocks[i].time = subsetData[i].time;
		
		m_blocks[i].statusTypeLen = sizeof(long);

		m_blocks[i].curPosOfDataFile = m_dataFileHeadLen;

	}
}

void CDataFileBuffer::WriteAllBlocks()
{
	for (ULONG i = 0; i< m_bufferLen; i++)
	{
		WriteBlock(i);
	}
}
inline void CDataFileBuffer::WriteBlock(ULONG i)
{
	CFileException fileException;
	if (m_blocks[i].dataFilePath != "")
	{
		if (m_blocks[i].dataFile.Open(m_blocks[i].dataFilePath, \
			CFile::modeRead, &fileException)
		{
			WriteBlocksByReadFile(i);
		} 
		else
		{
			WriteBlockByRandomNumber(i);
		}
		m_blocks[i].dataFile.Close();
	}
	else
	{
		WriteBlockByRandomNumber(i);
	}
}
inline void CDataFileBuffer::WriteBlocksByReadFile(ULONG i)
{
	ULONG dataFileLen = m_blocks[i].dataFile.GetLength();
	if (m_blocks[i].curPosOfDataFile <= m_dataFileHeadLen)
	{
		if (m_blocks[i].curPosOfDataFile + m_blocks[i].blockLen <= \
			dataFileLen)
		{
			m_blocks[i].dataFile.Seek(m_blocks[i].curPosOfDataFile, \
			CFile::begin);

			m_blocks[i].dataFile.Read(m_blocks[i].headOfBlock, \
			m_blocks[i].blockLen);

			m_blocks[i].curPosOfDataFile+= m_blocks[i].blockLen;
		} 
		else
		{
			m_blocks[i].dataFile.Seek(m_blocks[i].curPosOfDataFile, \
				CFile::begin);
			
			m_blocks[i].dataFile.Read(m_blocks[i].headOfBlock, \
			dataFileLen - m_blocks[i].curPosOfDataFile);
		}
	} 
	else
	{
		if (m_blocks[i].curPosOfDataFile + m_blocks[i].blockLen <= \
			dataFileLen)
		{
			m_blocks[i].dataFile.Seek(m_blocks[i].curPosOfDataFile, \
				CFile::begin);
			
			m_blocks[i].dataFile.Read(m_blocks[i].headOfBlock, \
				m_blocks[i].blockLen);
			
			m_blocks[i].curPosOfDataFile+= m_blocks[i].blockLen;
		} 
		else
		{
			ULONG firstReadLen = dataFileLen - m_blocks[i].curPosOfDataFile;
			ULONG secondReadLen = m_blocks[i].blockLen - firstReadLen;
			m_blocks[i].dataFile.Seek(m_blocks[i].curPosOfDataFile, \
			CFile::begin);
			
			m_blocks[i].dataFile.Read(m_blocks[i].headOfBlock, \
			firstReadLen);
			
			m_blocks[i].curPosOfDataFile = m_dataFileHeadLen;

			m_blocks[i].dataFile.Seek(m_blocks[i].curPosOfDataFile, \
			CFile::begin);
			
			m_blocks[i].dataFile.Read(m_blocks[i].headOfBlock	\
			+firstReadLen, secondReadLen);
			
			m_blocks[i].curPosOfDataFile+= secondReadLen;
		}
	}
}
inline void CDataFileBuffer::WriteBlockByRandomNumber(ULONG i)
{
	ULONG statusTypeLen = sizeof(long);

	BUF_TYPE * blockEnd = m_blocks[i].headOfBlock+m_blocks[i].blockLen;

	for (BUF_TYPE * pBlock = m_blocks[i].headOfBlock; pBlock < blockEnd; )
	{
		memcpy(pBlock, (BUF_TYPE *)m_blocks[i].status, statusTypeLen);
		pBlock+= statusTypeLen;
		memcpy(pBlock, (BUF_TYPE *)m_blocks[i].time, statusTypeLen);
		pBlock+= statusTypeLen;
		BUF_TYPE * subsetEnd = pBlock+(m_blocks[i].subsetLen-2*statusTypeLen);
		for ( ; pBlock < subsetEnd; pBlock++)
		{
			memset(pBlock, rand()%256, 1);
		}
	}

}

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
	m_dataFileHeadLen = 3*sizeof(UINT32);
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


void CDataFileBuffer::Init(SubsetData * sData)
{
	m_mode = 0;

	//m_blocks[0] = m_buffer;
	m_dataFileHeadLen = 3*sizeof(UINT32);
	BUF_TYPE * curPosOfBuffer = m_buffer;
	for (ULONG i = 0; i < m_numOfBlocks; i++)
	{
		m_blocks[i].subsetData = sData[i];
		
		m_blocks[i].blockLen = (m_bufferLen*m_blocks[i].subsetData.percentateOfDataFileBuf	\
		/m_blocks[i].subsetData.allSubsetsLenOfOneToolSubset)*m_blocks[i].subsetData.allSubsetsLenOfOneToolSubset;

		m_blocks[i].headOfBlock = curPosOfBuffer;

		m_blocks[i].curPosOfBlock = curPosOfBuffer;

		curPosOfBuffer+= m_blocks[i].blockLen;

		m_blocks[i].realUsedBlockLen = m_blocks[i].blockLen;

		m_blocks[i].subsetDataFilePath = "";

		m_blocks[i].curPosOfDataFile = m_dataFileHeadLen;
	}
}

void CDataFileBuffer::Init(ULONG i, CalibData * cData)
{

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
	if (!m_mode)
	{
		if (m_blocks[i].subsetDataFilePath != "")
		{
			if (m_blocks[i].dataFile.Open(m_blocks[i].subsetDataFilePath, \
				CFile::modeRead, &fileException))
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
	else
	{
		if (m_blocks[i].calibDataFilePath != "")
		{
			if (m_blocks[i].dataFile.Open(m_blocks[i].calibDataFilePath, \
				CFile::modeRead, &fileException))
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
	
	
}
inline void CDataFileBuffer::WriteBlocksByReadFile(ULONG i)
{
	ULONG dataFileLen = m_blocks[i].dataFile.GetLength();
	if (m_blocks[i].curPosOfDataFile <= m_dataFileHeadLen)
	{
		if (m_blocks[i].curPosOfDataFile + m_blocks[i].realUsedBlockLen <= \
			dataFileLen)
		{
			m_blocks[i].dataFile.Seek(m_blocks[i].curPosOfDataFile, \
			CFile::begin);

			m_blocks[i].dataFile.Read(m_blocks[i].headOfBlock, \
			m_blocks[i].realUsedBlockLen);

			m_blocks[i].curPosOfDataFile+= m_blocks[i].realUsedBlockLen;
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
		if (m_blocks[i].curPosOfDataFile + m_blocks[i].realUsedBlockLen <= \
			dataFileLen)
		{
			m_blocks[i].dataFile.Seek(m_blocks[i].curPosOfDataFile, \
				CFile::begin);
			
			m_blocks[i].dataFile.Read(m_blocks[i].headOfBlock, \
				m_blocks[i].realUsedBlockLen);
			
			m_blocks[i].curPosOfDataFile+= m_blocks[i].realUsedBlockLen;
		} 
		else
		{
			ULONG firstReadLen = dataFileLen - m_blocks[i].curPosOfDataFile;
			ULONG secondReadLen = m_blocks[i].realUsedBlockLen - firstReadLen;
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
	ULONG statusTypeLen;
	BUF_TYPE * blockEnd = m_blocks[i].headOfBlock+m_blocks[i].realUsedBlockLen;
	if (!m_mode)
	{
		statusTypeLen = sizeof(m_blocks[i].subsetData.status);	
		
		for (BUF_TYPE * pBlock = m_blocks[i].headOfBlock; pBlock < blockEnd; )
		{
			memcpy(pBlock, (BUF_TYPE *)m_blocks[i].subsetData.status, statusTypeLen);
			pBlock+= statusTypeLen;
			memcpy(pBlock, (BUF_TYPE *)m_blocks[i].subsetData.time, statusTypeLen);
			pBlock+= statusTypeLen;
			BUF_TYPE * subsetEnd = pBlock+(m_blocks[i].subsetData.rtcBlockDataHeader.dataSize-2*statusTypeLen);
			for ( ; pBlock < subsetEnd; pBlock++)
			{
				memset(pBlock, rand()%256, 1);
			}
		}
	} 
	else
	{
		statusTypeLen = sizeof(m_blocks[i].calibData.status);
		
		for (BUF_TYPE * pBlock = m_blocks[i].headOfBlock; pBlock < blockEnd; )
		{
			memcpy(pBlock, (BUF_TYPE *)m_blocks[i].calibData.status, statusTypeLen);
			pBlock+= statusTypeLen;
			memcpy(pBlock, (BUF_TYPE *)m_blocks[i].calibData.time, statusTypeLen);
			pBlock+= statusTypeLen;
			/*
			BUF_TYPE * subsetEnd = pBlock+(m_blocks[i].calibData-2*statusTypeLen);
			for ( ; pBlock < subsetEnd; pBlock++)
			{
				memset(pBlock, rand()%256, 1);
			}
			*/
		}
	}
	

}

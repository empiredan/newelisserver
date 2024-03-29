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
	m_buffer = NULL;
	m_bufferLen = 10*1024*1024;
	m_numOfBlocks = 0;
	m_blocks = NULL;
	m_dataFileHeadLen = 3*sizeof(UINT32);
	m_actDataFileRootPath = "";
	m_calverDataFileRootPath = "";
	m_mode = 0;
}

CDataFileBuffer::~CDataFileBuffer()
{
	if (m_buffer)
	{
		delete [] m_buffer;
		m_buffer = NULL;
	}
	if (m_blocks)
	{
		delete [] m_blocks;
		m_blocks = NULL;
	}

}


void CDataFileBuffer::Init(SubsetData * sData)
{
	m_mode = 0;

	//m_blocks[0] = m_buffer;
	m_dataFileHeadLen = 3*sizeof(UINT32);
	BUF_TYPE * curPosOfBuf = m_buffer;
	for (ULONG i = 0; i < m_numOfBlocks; i++)
	{
		m_blocks[i].subsetData = sData[i];
		
		m_blocks[i].blockLen = ((ULONG)(m_bufferLen*m_blocks[i].subsetData.percentateOfDataFileBuf)	\
		/m_blocks[i].subsetData.allSubsetsLenOfOneToolSubset)*m_blocks[i].subsetData.allSubsetsLenOfOneToolSubset;

		m_blocks[i].headOfBlock = curPosOfBuf;

		m_blocks[i].curPosOfBlock = curPosOfBuf;

		curPosOfBuf+= m_blocks[i].blockLen;

		m_blocks[i].realUsedBlockLen = m_blocks[i].blockLen;

		m_blocks[i].subsetDataFilePath = "";

		m_blocks[i].curPosOfDataFile = m_dataFileHeadLen;
	}
}

void CDataFileBuffer::Init(CalibData * cData)
{
	m_mode = 1;

	m_calibData = *cData;

	ULONG i = m_calibData.blockNo;

	m_blocks[i].curPosOfBlock = m_blocks[i].headOfBlock;

	m_blocks[i].realUsedBlockLen = (m_blocks[i].blockLen/m_calibData.subsetLen)*m_calibData.subsetLen;

	m_blocks[i].curPosOfDataFile = m_dataFileHeadLen;
}


void CDataFileBuffer::WriteAllBlocks()
{
	for (ULONG i = 0; i < m_numOfBlocks; i++)
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
		if (m_calibDataFilePath != "")
		{
			if (m_blocks[i].dataFile.Open(m_calibDataFilePath, \
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
			memcpy(pBlock, (BUF_TYPE *)&m_blocks[i].subsetData.status, statusTypeLen);
			pBlock+= statusTypeLen;
			memcpy(pBlock, (BUF_TYPE *)&m_blocks[i].subsetData.time, statusTypeLen);
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
		statusTypeLen = sizeof(m_calibData.status);
		
		for (BUF_TYPE * pBlock = m_blocks[i].headOfBlock; pBlock < blockEnd; )
		{
			memcpy(pBlock, (BUF_TYPE *)&m_calibData.status, statusTypeLen);
			pBlock+= statusTypeLen;
			memcpy(pBlock, (BUF_TYPE *)&m_calibData.time, statusTypeLen);
			pBlock+= statusTypeLen;
			BUF_TYPE * subsetEnd = pBlock+(m_calibData.subsetLen-2*statusTypeLen);
			for ( ; pBlock < subsetEnd; pBlock++)
			{
				memset(pBlock, rand()%256, 1);
			}
		
		}
	}
	

}

void CDataFileBuffer::SetDataFilePathByRootPath(CString rootPath)
{
	CFileFind dataFileFind;
	BOOL isRootPathFinded;
	if (!m_mode)
	{
		m_actDataFileRootPath = rootPath;
		isRootPathFinded = dataFileFind.FindFile(m_actDataFileRootPath+"\\*.dat");
		if (isRootPathFinded)
		{
			for (ULONG i = 0; i < m_numOfBlocks; i++)
			{
				BOOL isDataFilePathFinded = dataFileFind.FindFile(m_actDataFileRootPath+"\\*.dat");
				while(isDataFilePathFinded)
				{
					isDataFilePathFinded = dataFileFind.FindNextFile();
					CString dataFilePath = dataFileFind.GetFilePath();
					
					UINT32 dataFileHeader[3];
					BUF_TYPE dataFileHeaderBuf[sizeof(UINT32)*3];
					CFile dataFile(dataFilePath, CFile::modeRead);
					dataFile.Read(dataFileHeaderBuf, sizeof(UINT32)*3);
					dataFile.Close();
					
					memcpy(dataFileHeader, dataFileHeaderBuf, sizeof(UINT32)*3);
					UINT32 toolADDR = dataFileHeader[0];
					UINT32 subsetNo = dataFileHeader[1];
					UINT32 dataFileType = dataFileHeader[2];
					if (toolADDR == m_blocks[i].subsetData.rtcBlockDataHeader.toolAddr
						&& subsetNo == m_blocks[i].subsetData.rtcBlockDataHeader.subset
						&& dataFileType == 0)
					{
						m_blocks[i].subsetDataFilePath = dataFilePath;
						break;
					}//if
				}//while
				
			}//for
		}//if
	} 
	else
	{
		m_calverDataFileRootPath = rootPath;
		isRootPathFinded = dataFileFind.FindFile(m_calverDataFileRootPath+"\\*.dat");
		if (isRootPathFinded)
		{
			BOOL isDataFilePathFinded = dataFileFind.FindFile(m_calverDataFileRootPath+"\\*.dat");
			while(isDataFilePathFinded)
			{
				isDataFilePathFinded = dataFileFind.FindNextFile();
				CString dataFilePath = dataFileFind.GetFilePath();
				
				UINT32 dataFileHeader[3];
				BUF_TYPE dataFileHeaderBuf[sizeof(UINT32)*3];
				CFile dataFile(dataFilePath, CFile::modeRead);
				dataFile.Read(dataFileHeaderBuf, sizeof(UINT32)*3);
				dataFile.Close();
				
				memcpy(dataFileHeader, dataFileHeaderBuf, sizeof(UINT32)*3);
				UINT32 toolADDR = dataFileHeader[0];
				UINT32 subsetNo = dataFileHeader[1];
				UINT32 dataFileType = dataFileHeader[2];
				ULONG i = m_calibData.blockNo;
				if (toolADDR == (UINT32)m_calibData.rtcSubset.toolAddress
					&& subsetNo == (UINT32)m_calibData.rtcSubset.subsetNo
					&& dataFileType == 1)
				{
					m_calibDataFilePath = dataFilePath;
					break;
				}//if
			}//while
		}
	}
	
	dataFileFind.Close();
	
}
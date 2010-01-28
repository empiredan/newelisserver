// DataFileBuf.h: interface for the CDataFileBuf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAFILEBUF_H__32B6A05D_26E5_4A82_B9D1_825BD44433A5__INCLUDED_)
#define AFX_DATAFILEBUF_H__32B6A05D_26E5_4A82_B9D1_825BD44433A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "commands.h"

class CELISTestServerDlg;
class CDataFileBuf  
{
public:
	CDataFileBuf(CELISTestServerDlg* dlg);
	virtual ~CDataFileBuf();
public:
	typedef struct {
		ULONG dbufsz;
		ULONG curpos; //relative to the 3-UINT32 sized header 
		CFile df;
		BUF_TYPE *dbhead;
		BUF_TYPE *dbcur;
		BOOL fileExists;
	}Buffer;
public:
	Buffer *bf;//actNum个元素
	BUF_TYPE *store;
	ULONG m_dataFileBufSize;//buf大小
	CELISTestServerDlg* m_pdlg; 
	//ULONG* m_realUsedBlockSize;
	UINT m_actNum;
public:
	void resetCurrentPointer(UINT i);
	void resetCurrentPointer();
	UINT getCalVerBufBlockIndex();
	UINT m_calverBufBlockIndex;
	BUF_TYPE* getNextCalibSubsetDataPointer();
	void create(ULONG bufsize, UINT actnum);
	void createStore(ULONG bufsize);
	void createLayout(UINT actnum);
	void clearStore();
	void clearLayout();
	void layout();
	void clear();
	void fillWithDataFile();
	void fillWithDataFile(UINT i, CString &file);
	void fillWithDataFile(UINT i);

	BUF_TYPE* getNextDataPointer(UINT i);
private:
	void increase(UINT i, UINT disp, int size_status);
	void fillInWithRandomData(UINT i, int size_status);
	void fillIn(CString &filePath, UINT i, int size_status);
	void Save(CFile &log);
};

#endif // !defined(AFX_DATAFILEBUF_H__32B6A05D_26E5_4A82_B9D1_825BD44433A5__INCLUDED_)

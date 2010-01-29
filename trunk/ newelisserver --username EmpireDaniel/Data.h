// Data.h: interface for the CData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_H__6047988D_24DC_4EAF_9AFD_044DD1AC6898__INCLUDED_)
#define AFX_DATA_H__6047988D_24DC_4EAF_9AFD_044DD1AC6898__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CData  
{
public:
	CData();
	virtual ~CData();

};

//主控机上发给前端机的数据
class CMasterData:public CData {
public:
	CMasterData();
	CMasterData(ULONG cmdtype, ULONG totallen, ULONG headlen, BUF_TYPE * bodybuf, ULONG bodylen);
	virtual ~CMasterData();
	inline ULONG GetCmdType(){return m_cmdType;}
	inline ULONG GetTotalLen(){return m_totalLen;}
	inline ULONG GetHeadLen(){return m_headLen;}
	inline BUF_TYPE * GetBodyBuf(){return m_bodyBuf;}
	inline ULONG GetBodyLen(){return m_bodyLen;}
private:
	//Head of received data
	ULONG m_cmdType;
	ULONG m_totalLen;
	//Size of head
	ULONG m_headLen;
	//Body of received data	
	BUF_TYPE * m_bodyBuf;
	//Head of body
	ULONG m_bodyLen;
};

//前端机上准备的要发给主控机的数据
class CFrontData:public CData {
public:
	CFrontData();
	CFrontData(BUF_TYPE* bf, ULONG len);
	virtual ~CFrontData();
	
public:
	ULONG cmdType;
	ULONG cmdLen;
	ULONG bodyLen;
public:
	ULONG getCmdLength() {
		return cmdLen;
	}
	ULONG getCmdType() {
		return cmdType;
	}
	ULONG getBodyLength() {
		return bodyLen;
	}
};

#endif // !defined(AFX_DATA_H__6047988D_24DC_4EAF_9AFD_044DD1AC6898__INCLUDED_)

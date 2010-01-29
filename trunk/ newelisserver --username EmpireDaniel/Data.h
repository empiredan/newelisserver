#if !defined(DATA_H)
#define DATA_H

#include "commands.h"
//#include "DPMDisplayParameter.h"

class CData {
public:
	CData();
	CData(BUF_TYPE* bf, ULONG len);
	virtual ~CData();
public:
	virtual void setData(BUF_TYPE* bf, ULONG len);
	//确保buf的总长度够用
	//调完这个函数buflen的大小应>=sizeNeeded
	virtual void assureCapacity(ULONG sizeNeeded);
	//设置buf的前两个ULONG，
	//设置完这个命令头后，pBuf会前进2×sizeof(ULONG)
	//contentlen也会相应增加为2×sizeof(ULONG)
	virtual void setHeader(ULONG cmdType, ULONG cmdLen);
	//void setBuf(BUF_TYPE* b);
	//void setBufLen(ULONG bl);
public:
	//pBuf作为临时指针，指向buf中的第一个空位置
	BUF_TYPE *buf, *pBuf;
	//buflen是buf的有效长度，contentlen应该小于等于buflen
	ULONG buflen;
	//contentlen记录当前buf中已经有多少有效数据了
	ULONG contentlen;
	//CString buf;

};


//主控机上发给前端机的数据
class CMasterData:public CData {
public:
	CMasterData();
	CMasterData(BUF_TYPE* bf, ULONG len);
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


#endif

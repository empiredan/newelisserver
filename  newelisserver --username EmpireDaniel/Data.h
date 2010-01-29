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
	//ȷ��buf���ܳ��ȹ���
	//�����������buflen�Ĵ�СӦ>=sizeNeeded
	virtual void assureCapacity(ULONG sizeNeeded);
	//����buf��ǰ����ULONG��
	//�������������ͷ��pBuf��ǰ��2��sizeof(ULONG)
	//contentlenҲ����Ӧ����Ϊ2��sizeof(ULONG)
	virtual void setHeader(ULONG cmdType, ULONG cmdLen);
	//void setBuf(BUF_TYPE* b);
	//void setBufLen(ULONG bl);
public:
	//pBuf��Ϊ��ʱָ�룬ָ��buf�еĵ�һ����λ��
	BUF_TYPE *buf, *pBuf;
	//buflen��buf����Ч���ȣ�contentlenӦ��С�ڵ���buflen
	ULONG buflen;
	//contentlen��¼��ǰbuf���Ѿ��ж�����Ч������
	ULONG contentlen;
	//CString buf;

};


//���ػ��Ϸ���ǰ�˻�������
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

//ǰ�˻���׼����Ҫ�������ػ�������
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

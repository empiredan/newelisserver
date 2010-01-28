#include "stdafx.h"
#include "Data.h"


CData::CData() {
	buf = new BUF_TYPE[DEFAULT_BUF_LEN];
	buflen = DEFAULT_BUF_LEN;
	pBuf = buf;
	contentlen = 0;
}

CData::CData(BUF_TYPE* bf, ULONG len) {
	this->buf = NULL;
	this->pBuf = NULL;
	contentlen = 0;
	buflen = 0;

	if(len < DEFAULT_BUF_LEN) {
		assureCapacity(DEFAULT_BUF_LEN);
	}
	setData(bf, len);
}

CData::~CData() {
	if(buf != NULL) {
		delete []buf;
	}
}

void CData::assureCapacity(ULONG sizeNeeded) {
	ULONG rem = buflen - contentlen;
	BUF_TYPE *buft;
	if(this->buf == NULL || rem < sizeNeeded) {
		buft = new BUF_TYPE[contentlen + sizeNeeded];
		ASSERT(buft != NULL);
		this->buflen = contentlen + sizeNeeded;

		if(this->buf != NULL) {
			if(contentlen > 0) {
				memcpy(buft, buf, contentlen);
			}
			delete []this->buf;
		} else {//这不应该出现,contentlen必须已经是0
			contentlen = 0;
		}

		this->buf = buft;
		this->pBuf = (this->buf + contentlen);
	}
}

void CData::setData(BUF_TYPE* bf, ULONG len) {
	assureCapacity(len);
	memcpy(this->pBuf, bf, len);
	pBuf += len;
	contentlen += len;

	//ULONG *t, t0, t1;
	//t = (ULONG*)this->buf;
	//t0 = ntohl(t[0]);
	//t1 = ntohl(t[1]);
}

void CData::setHeader(ULONG cmdType, ULONG cmdLen) {
	ULONG *t;
	
	assureCapacity(SOCK_RECEIVE_HEADER_LEN);
	
	t = (ULONG*)pBuf;
	t[0] = cmdType;
	t[1] = cmdLen;

	pBuf += SOCK_RECEIVE_HEADER_LEN;
	contentlen += SOCK_RECEIVE_HEADER_LEN;
}


//CMasterData----------------
CMasterData::CMasterData():CData() {
}
CMasterData::CMasterData(BUF_TYPE* bf, ULONG len):CData(bf, len) {
}
CMasterData::~CMasterData() {
	//CData::~CData();
}

//CFrontData----------------
CFrontData::CFrontData():CData() {
}
CFrontData::CFrontData(BUF_TYPE* bf, ULONG len):CData(bf, len) {
}
CFrontData::~CFrontData() {
	//CData::~CData();
}
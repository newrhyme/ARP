// IPLayer.h: interface for the CIPLayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_)
#define AFX_IPLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"

class CIPLayer
	: public CBaseLayer
{
private:
	inline void ResetHeader();

public:
	CIPLayer(char* name);
	virtual ~CIPLayer();
	void SetSrcIPAddress(unsigned char* srcIP);
	void SetDstIPAddress(unsigned char* dstIP);

	BOOL Send(unsigned char* ppayload, int nlength);
	BOOL Receive(unsigned char* ppayload);

	typedef struct _IP_HEADER {
		unsigned char srcIP[4];
		unsigned char dstIP[4];
		unsigned char ip_data[IP_DATA_SIZE];
	} IP_HEADER, *PIP_HEADER;

protected:
	IP_HEADER m_sHeader;
};

#endif // !defined(AFX_IPLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_)
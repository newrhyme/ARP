// ARPLayer.h: interface for the CARPLayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARPLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_)
#define AFX_ARPLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"
#include <vector>
#include <tuple>


class CARPLayer
	: public CBaseLayer
{
private:
	inline void ResetHeader();

public:
	CARPLayer(char* name);
	virtual ~CARPLayer();

	void SetSrcIPAddress(unsigned char* srcIP);
	void SetDstIPAddress(unsigned char* dstIP);
	void SetEnetSrcAddress(unsigned char* srcEthAddr);
	void SetEnetDstAddress(unsigned char* dstEthAddr);
	void SetOperation(unsigned int arp_operation);

	unsigned char* GetSrcIPAddress();
	unsigned char* GetDstIPAddress();
	unsigned char* GetEnetSrcAddress();
	unsigned char* GetEnetDstAddress();
	unsigned int GetOperation();

	BOOL Send(unsigned char* ppayload, int nlength);
	BOOL Receive(unsigned char* ppayload);

	typedef struct _ARP_HEADER
	{
		unsigned short arp_hardType;
		unsigned short arp_protType;
		unsigned char arp_hardSize;
		unsigned char arp_protSize;
		unsigned short arp_operation;
		ETHERNET_ADDR arp_srcEthAddr;
		IP_ADDR arp_srcIPAddr;
		ETHERNET_ADDR arp_dstEthAddr;
		IP_ADDR arp_dstIPAddr;
	} ARP_HEADER, * PARP_HEADER;
	
	vector<tuple<unsigned char*, unsigned char*, BOOL>> arp_table;

protected:
	ARP_HEADER m_sHeader;
};

#endif // !defined(AFX_ARPLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_)
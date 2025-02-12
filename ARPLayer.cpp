// EthernetLayer.cpp: implementation of the CEthernetLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pch.h"
#include "ARPLayer.h"
#include "EthernetLayer.h"
#include "ARP_Group3Dlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARPLayer::CARPLayer(char* pName)
    : CBaseLayer(pName)
{
	ResetHeader();
	arp_table.clear();
}

CARPLayer::~CARPLayer()
{
}

void CARPLayer::ResetHeader()
{
	m_sHeader.arp_hardType = htons(0x0001);
	m_sHeader.arp_protType = htons(0x0800);
	m_sHeader.arp_hardSize = 6;
	m_sHeader.arp_protSize = 4;
	m_sHeader.arp_operation = 0x0000;
	memset(m_sHeader.arp_srcEthAddr.addrs, 0, 6);
	memset(m_sHeader.arp_dstEthAddr.addrs, 0, 6);
	memset(m_sHeader.arp_srcIPAddr.addrs, 0, 4);
	memset(m_sHeader.arp_dstIPAddr.addrs, 0, 4);
}

BOOL CARPLayer::Send(unsigned char* ppayload, int nlength)
{
	BOOL bSuccess = FALSE;
	unsigned char BROADCAST[6] = { 0xff, };

	// Ethernet Frame Type을 ARP로 설정
	((CEthernetLayer*)mp_UnderLayer)->SetFrameType(htons(0x0806));
	
	//ARP Frame의 Opcode를 Request(1)로 설정
	SetOperation(htons(ARP_OP_REQUEST));
	SetEnetDstAddress(BROADCAST);
	bSuccess = mp_UnderLayer->Send((unsigned char*) & m_sHeader, sizeof(ARP_HEADER));
	return bSuccess;
}


BOOL CARPLayer::Receive(unsigned char* ppayload)
{
	PARP_HEADER pFrame = (PARP_HEADER)ppayload;

	unsigned char BROADCAST[6];
	memset(BROADCAST, 0xff, 6);

	// 자기 자신에게 보내서 수신받은 경우 arp_table에 만들지 않음
	if (memcmp(pFrame->arp_srcEthAddr.addrs, m_sHeader.arp_srcEthAddr.addrs, 6) == 0) {
		return FALSE;
	}

	BOOL bSuccess = FALSE;
	// Request인 경우
	if (htons(pFrame->arp_operation) == ARP_OP_REQUEST) {
		BOOL exist = FALSE;
		// IP Address를 기준으로 이미 있는지 검사
		for (auto& entry : arp_table) {
			if (memcmp(get<0>(entry), pFrame->arp_srcIPAddr.addrs, 4) == 0) {
				memcpy(get<1>(entry), pFrame->arp_srcEthAddr.addrs, 6);
				get<2>(entry) = TRUE;
				exist = TRUE;
				break;
			}
		}

		// table에 존재하지 않은 경우 arp_table에 저장
		if (!exist) {
			unsigned char* ipCopy = (unsigned char*)malloc(4);
			unsigned char* macCopy = (unsigned char*)malloc(6);
			memcpy(ipCopy, pFrame->arp_srcIPAddr.addrs, 4);
			memcpy(macCopy, pFrame->arp_srcEthAddr.addrs, 6);
			arp_table.push_back(make_tuple(ipCopy, macCopy, TRUE));
		}

		// opcode를 Reply로 변경
		SetOperation(htons(ARP_OP_REPLY));
		// Target의 IP Address와 MAC Address를 pFrame의 src로 변경
		SetEnetDstAddress(pFrame->arp_srcEthAddr.addrs);
		SetDstIPAddress(pFrame->arp_srcIPAddr.addrs);
		
		// 다시 Ethernet Layer로 전달
		bSuccess = mp_UnderLayer->Send((unsigned char*)&m_sHeader, sizeof(m_sHeader));

		// Dlg에서 Select 버튼을 누를 때 ARP 프레임에서 나의 Src IP Address와 MAC 주소가 저장됨.
	}
	
	return bSuccess;
}


void CARPLayer::SetEnetSrcAddress(unsigned char* srcEnetAddr)
{
	memcpy(&m_sHeader.arp_srcEthAddr.addrs, srcEnetAddr, 6);
}

void CARPLayer::SetEnetDstAddress(unsigned char* dstEnetAddr)
{
	memcpy(&m_sHeader.arp_dstEthAddr.addrs, dstEnetAddr, 6);
}

void CARPLayer::SetSrcIPAddress(unsigned char* srcIP)
{
	memcpy(m_sHeader.arp_srcIPAddr.addrs, srcIP, 4);
}

void CARPLayer::SetDstIPAddress(unsigned char* dstIP)
{
	memcpy(m_sHeader.arp_dstIPAddr.addrs, dstIP, 4);
}

void CARPLayer::SetOperation(unsigned int arp_operation)
{
	m_sHeader.arp_operation = arp_operation;
}

unsigned char* CARPLayer::GetSrcIPAddress()
{
	return m_sHeader.arp_srcIPAddr.addrs;
}

unsigned char* CARPLayer::GetDstIPAddress()
{
	return m_sHeader.arp_dstIPAddr.addrs;
}

unsigned char* CARPLayer::GetEnetSrcAddress()
{
	return m_sHeader.arp_srcEthAddr.addrs;
}

unsigned char* CARPLayer::GetEnetDstAddress()
{
	return m_sHeader.arp_dstEthAddr.addrs;
}

unsigned int CARPLayer::GetOperation()
{
	return m_sHeader.arp_operation;
}